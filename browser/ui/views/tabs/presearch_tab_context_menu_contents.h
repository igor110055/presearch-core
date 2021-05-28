/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_TABS_PRESEARCH_TAB_CONTEXT_MENU_CONTENTS_H_
#define PRESEARCH_BROWSER_UI_VIEWS_TABS_PRESEARCH_TAB_CONTEXT_MENU_CONTENTS_H_

#include <memory>

#include "ui/base/models/simple_menu_model.h"
#include "ui/base/ui_base_types.h"

class PresearchBrowserTabStripController;
class PresearchTabMenuModel;
class Browser;
class Tab;

namespace sessions {
class TabRestoreService;
}  // namespace sessions

namespace views {
class MenuRunner;
}  // namespace views

class PresearchTabContextMenuContents : public ui::SimpleMenuModel::Delegate {
 public:
  PresearchTabContextMenuContents(Tab* tab,
                              PresearchBrowserTabStripController* controller,
                              int index);
  ~PresearchTabContextMenuContents() override;

  void Cancel();

  void RunMenuAt(const gfx::Point& point, ui::MenuSourceType source_type);

  // ui::SimpleMenuModel::Delegate overrides:
  bool IsCommandIdChecked(int command_id) const override;
  bool IsCommandIdEnabled(int command_id) const override;
  bool GetAcceleratorForCommandId(int command_id,
                                  ui::Accelerator* accelerator) const override;
  void ExecuteCommand(int command_id, int event_flags) override;

 private:
  bool IsPresearchCommandIdEnabled(int command_id) const;
  void ExecutePresearchCommand(int command_id);
  bool IsPresearchCommandId(int command_id) const;

  std::unique_ptr<PresearchTabMenuModel> model_;
  std::unique_ptr<views::MenuRunner> menu_runner_;

  Tab* tab_;
  Browser* browser_;
  sessions::TabRestoreService* restore_service_ = nullptr;
  PresearchBrowserTabStripController* controller_;

  DISALLOW_COPY_AND_ASSIGN(PresearchTabContextMenuContents);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_TABS_PRESEARCH_TAB_CONTEXT_MENU_CONTENTS_H_
