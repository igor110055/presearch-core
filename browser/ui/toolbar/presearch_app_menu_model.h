/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_APP_MENU_MODEL_H_
#define PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_APP_MENU_MODEL_H_

#include <memory>
#include <vector>

#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/toolbar/app_menu_model.h"
#include "ui/base/models/simple_menu_model.h"

class PresearchAppMenuModel : public AppMenuModel {
 public:
  PresearchAppMenuModel(ui::AcceleratorProvider* provider,
                    Browser* browser,
                    AppMenuIconController* app_menu_icon_controller = nullptr);
  ~PresearchAppMenuModel() override;

  PresearchAppMenuModel(const PresearchAppMenuModel&) = delete;
  PresearchAppMenuModel& operator=(const PresearchAppMenuModel&) = delete;

 private:
  // AppMenuModel overrides:
  void Build() override;

  // void InsertPresearchMenuItems();
  void InsertAlternateProfileItems();
  int GetIndexOfPresearchRewardsItem() const;
  int GetIndexOfPresearchAdBlockItem() const;
  int GetIndexOfPresearchSyncItem() const;
#if BUILDFLAG(ENABLE_SIDEBAR)
  int GetIndexOfPresearchSidebarItem() const;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ui::SimpleMenuModel ipfs_submenu_model_;
#endif
  std::vector<std::unique_ptr<ui::SimpleMenuModel>> sub_menus_;
};

#endif  // PRESEARCH_BROWSER_UI_TOOLBAR_PRESEARCH_APP_MENU_MODEL_H_
