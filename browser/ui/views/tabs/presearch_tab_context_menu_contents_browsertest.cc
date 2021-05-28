/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/tabs/presearch_tab_menu_model.h"
#include "presearch/browser/ui/views/tabs/presearch_tab_context_menu_contents.h"
#include "presearch/browser/ui/views/tabs/presearch_browser_tab_strip_controller.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"
#include "chrome/browser/ui/views/tabs/tab_strip.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "url/gurl.h"

using PresearchTabContextMenuContentsTest = InProcessBrowserTest;

IN_PROC_BROWSER_TEST_F(PresearchTabContextMenuContentsTest, Basics) {
  TabStrip* tabstrip =
      BrowserView::GetBrowserViewForBrowser(browser())->tabstrip();
  PresearchTabContextMenuContents menu(
      tabstrip->tab_at(0),
      static_cast<PresearchBrowserTabStripController*>(
          BrowserView::GetBrowserViewForBrowser(
              browser())->tabstrip()->controller()),
      0);

  // All items are disable state when there is only one tab.
  EXPECT_FALSE(menu.IsCommandIdEnabled(
      PresearchTabMenuModel::CommandRestoreTab));
  EXPECT_FALSE(menu.IsCommandIdEnabled(
      PresearchTabMenuModel::CommandBookmarkAllTabs));

  chrome::NewTab(browser());
  // Still restore tab menu is disabled because there is no closed tab.
  EXPECT_FALSE(menu.IsCommandIdEnabled(
      PresearchTabMenuModel::CommandRestoreTab));
  // Bookmark all tabs item is enabled if the number of tabs are 2 or more.
  EXPECT_TRUE(menu.IsCommandIdEnabled(
      PresearchTabMenuModel::CommandBookmarkAllTabs));

  // When a tab is closed, restore tab menu item is enabled.
  ui_test_utils::NavigateToURL(browser(), GURL("presearch://version/"));
  chrome::CloseTab(browser());
  EXPECT_TRUE(menu.IsCommandIdEnabled(
      PresearchTabMenuModel::CommandRestoreTab));
  EXPECT_FALSE(menu.IsCommandIdEnabled(
      PresearchTabMenuModel::CommandBookmarkAllTabs));
}
