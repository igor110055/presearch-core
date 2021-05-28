/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_RECENT_TABS_SUB_MENU_MODEL_H_
#define PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_RECENT_TABS_SUB_MENU_MODEL_H_

#include "chrome/browser/ui/toolbar/recent_tabs_sub_menu_model.h"

class Browser;

namespace ui {
class AcceleratorProvider;
}

class PresearchRecentTabsSubMenuModel : public RecentTabsSubMenuModel {
 public:
  PresearchRecentTabsSubMenuModel(ui::AcceleratorProvider* accelerator_provider,
                         Browser* browser);

  ~PresearchRecentTabsSubMenuModel() override;

  void ExecuteCommand(int command_id, int event_flags) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchRecentTabsSubMenuModel);
};

#endif  // PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_RECENT_TABS_SUB_MENU_MODEL_H_
