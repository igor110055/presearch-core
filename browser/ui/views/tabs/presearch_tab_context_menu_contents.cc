/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/tabs/presearch_tab_context_menu_contents.h"

#include "presearch/browser/ui/tabs/presearch_tab_menu_model.h"
#include "presearch/browser/ui/views/tabs/presearch_browser_tab_strip_controller.h"
#include "chrome/browser/defaults.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sessions/tab_restore_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "components/sessions/core/tab_restore_service.h"
#include "ui/views/controls/menu/menu_runner.h"

PresearchTabContextMenuContents::PresearchTabContextMenuContents(
    Tab* tab,
    PresearchBrowserTabStripController* controller,
    int index)
    : tab_(tab),
      browser_(const_cast<Browser*>(controller->browser())),
      controller_(controller) {
  model_ = std::make_unique<PresearchTabMenuModel>(
      this, controller->model(), index);
  restore_service_ =
      TabRestoreServiceFactory::GetForProfile(browser_->profile());
  menu_runner_ = std::make_unique<views::MenuRunner>(
      model_.get(),
      views::MenuRunner::HAS_MNEMONICS | views::MenuRunner::CONTEXT_MENU);
}

PresearchTabContextMenuContents::~PresearchTabContextMenuContents() {
}

void PresearchTabContextMenuContents::Cancel() {
  controller_ = nullptr;
}

void PresearchTabContextMenuContents::RunMenuAt(
    const gfx::Point& point,
    ui::MenuSourceType source_type) {
  menu_runner_->RunMenuAt(tab_->GetWidget(), nullptr,
                          gfx::Rect(point, gfx::Size()),
                          views::MenuAnchorPosition::kTopLeft, source_type);
}

bool PresearchTabContextMenuContents::IsCommandIdChecked(int command_id) const {
  return false;
}

bool PresearchTabContextMenuContents::IsCommandIdEnabled(int command_id) const {
  if (IsPresearchCommandId(command_id))
    return IsPresearchCommandIdEnabled(command_id);

  return controller_->IsCommandEnabledForTab(
      static_cast<TabStripModel::ContextMenuCommand>(command_id),
      tab_);
}

bool PresearchTabContextMenuContents::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) const {
  if (IsPresearchCommandId(command_id))
    return false;

  int browser_cmd;
  return TabStripModel::ContextMenuCommandToBrowserCommand(command_id,
                                                           &browser_cmd) ?
      tab_->GetWidget()->GetAccelerator(browser_cmd, accelerator) : false;
}

void PresearchTabContextMenuContents::ExecuteCommand(int command_id,
                                                 int event_flags) {
  if (IsPresearchCommandId(command_id))
    return ExecutePresearchCommand(command_id);

  // Executing the command destroys |this|, and can also end up destroying
  // |controller_|. So stop the highlights before executing the command.
  controller_->ExecuteCommandForTab(
      static_cast<TabStripModel::ContextMenuCommand>(command_id),
      tab_);
}

bool PresearchTabContextMenuContents::IsPresearchCommandIdEnabled(
    int command_id) const {
  switch (command_id) {
    case PresearchTabMenuModel::CommandRestoreTab:
      return restore_service_ && (!restore_service_->IsLoaded() ||
                                  !restore_service_->entries().empty());
      break;
    case PresearchTabMenuModel::CommandBookmarkAllTabs:
      if (browser_) {
        return browser_defaults::bookmarks_enabled &&
               chrome::CanBookmarkAllTabs(browser_);
      }
      break;
    default:
      NOTREACHED();
      break;
  }

  return false;
}

void PresearchTabContextMenuContents::ExecutePresearchCommand(int command_id) {
  switch (command_id) {
    case PresearchTabMenuModel::CommandRestoreTab:
      chrome::RestoreTab(browser_);
      return;
    case PresearchTabMenuModel::CommandBookmarkAllTabs:
      chrome::BookmarkAllTabs(browser_);
      return;
    default:
      NOTREACHED();
      return;
  }
}

bool PresearchTabContextMenuContents::IsPresearchCommandId(int command_id) const {
  return command_id > PresearchTabMenuModel::CommandStart &&
         command_id < PresearchTabMenuModel::CommandLast;
}
