/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_PRESEARCH_BROWSER_H_
#define PRESEARCH_BROWSER_UI_PRESEARCH_BROWSER_H_

#include <memory>

#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/browser.h"

#if BUILDFLAG(ENABLE_SIDEBAR)
namespace sidebar {
class SidebarController;
}  // namespace sidebar

class PresearchBrowserWindow;
#endif

class PresearchBrowser : public Browser {
 public:
  explicit PresearchBrowser(const CreateParams& params);
  ~PresearchBrowser() override;

  PresearchBrowser(const PresearchBrowser&) = delete;
  PresearchBrowser& operator=(const PresearchBrowser&) = delete;

  // Browser overrides:
  void ScheduleUIUpdate(content::WebContents* source,
                        unsigned changed_flags) override;
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;

#if BUILDFLAG(ENABLE_SIDEBAR)
  sidebar::SidebarController* sidebar_controller() {
    return sidebar_controller_.get();
  }
#endif

  PresearchBrowserWindow* presearch_window();

 private:
#if BUILDFLAG(ENABLE_SIDEBAR)
  std::unique_ptr<sidebar::SidebarController> sidebar_controller_;
#endif
};

#endif  // PRESEARCH_BROWSER_UI_PRESEARCH_BROWSER_H_
