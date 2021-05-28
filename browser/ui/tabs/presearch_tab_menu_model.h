/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_TABS_PRESEARCH_TAB_MENU_MODEL_H_
#define PRESEARCH_BROWSER_UI_TABS_PRESEARCH_TAB_MENU_MODEL_H_

#include "chrome/browser/ui/tabs/tab_menu_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"

namespace content {
class WebContents;
}  // namespace content

namespace sessions {
class TabRestoreService;
}  // namespace sessions

class PresearchTabMenuModel : public TabMenuModel {
 public:
  enum PresearchTabContextMenuCommand {
    CommandStart = TabStripModel::CommandLast,
    CommandRestoreTab,
    CommandBookmarkAllTabs,
    CommandLast,
  };

  PresearchTabMenuModel(ui::SimpleMenuModel::Delegate* delegate,
                    TabStripModel* tab_strip_model,
                    int index);
  ~PresearchTabMenuModel() override;

 private:
  void Build();
  int GetRestoreTabCommandStringId() const;

  content::WebContents* web_contents_;
  sessions::TabRestoreService* restore_service_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(PresearchTabMenuModel);
};

#endif  // PRESEARCH_BROWSER_UI_TABS_PRESEARCH_TAB_MENU_MODEL_H_
