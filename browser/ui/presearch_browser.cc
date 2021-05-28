/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/presearch_browser.h"

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "presearch/browser/ui/presearch_browser_window.h"
#include "presearch/browser/ui/sidebar/sidebar.h"
#include "presearch/browser/ui/sidebar/sidebar_controller.h"
#include "presearch/browser/ui/sidebar/sidebar_utils.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#endif

PresearchBrowser::PresearchBrowser(const CreateParams& params) : Browser(params) {
#if BUILDFLAG(ENABLE_SIDEBAR)
  if (!sidebar::CanUseSidebar(profile()) || !is_type_normal())
    return;
  // Below call order is important.
  // When reaches here, Sidebar UI is setup in PresearchBrowserView but
  // not initialized. It's just empty because sidebar controller/model is not
  // ready yet. PresearchBrowserView is instantiated by the ctor of Browser.
  // So, initializing sidebar controller/model here and then ask to initialize
  // sidebar UI. After that, UI will be updated for model's change.
  sidebar_controller_.reset(new sidebar::SidebarController(this, profile()));
  sidebar_controller_->SetSidebar(presearch_window()->InitSidebar());
#endif
}

PresearchBrowser::~PresearchBrowser() = default;

void PresearchBrowser::ScheduleUIUpdate(content::WebContents* source,
                                    unsigned changed_flags) {
  Browser::ScheduleUIUpdate(source, changed_flags);

#if BUILDFLAG(ENABLE_SIDEBAR)
  if (tab_strip_model_->GetIndexOfWebContents(source) == TabStripModel::kNoTab)
    return;

  // We need to update sidebar UI only when current active tab state is changed.
  if (changed_flags & content::INVALIDATE_TYPE_URL) {
    if (source == tab_strip_model_->GetActiveWebContents()) {
      if (sidebar_controller_)
        sidebar_controller_->sidebar()->UpdateSidebar();
    }
  }
#endif
}

void PresearchBrowser::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  Browser::OnTabStripModelChanged(tab_strip_model, change, selection);

#if BUILDFLAG(ENABLE_SIDEBAR)
  // We need to update sidebar UI whenever active tab is changed.
  if (selection.active_tab_changed() && sidebar_controller_)
    sidebar_controller_->sidebar()->UpdateSidebar();
#endif
}

PresearchBrowserWindow* PresearchBrowser::presearch_window() {
  return static_cast<PresearchBrowserWindow*>(window_);
}
