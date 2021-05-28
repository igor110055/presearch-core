/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/presearch_pages.h"

#include "presearch/browser/webcompat_reporter/webcompat_reporter_dialog.h"
#include "presearch/common/webui_url_constants.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/browser/ui/singleton_tabs.h"
#include "chrome/common/webui_url_constants.h"
#include "url/gurl.h"

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/browser/ipfs/ipfs_tab_helper.h"
#endif

namespace presearch {

void ShowPresearchRewards(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kPresearchUIRewardsURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowPresearchAdblock(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kPresearchUIAdblockURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowSync(Browser* browser) {
  auto url = chrome::GetSettingsUrl(chrome::kSyncSetupSubPage);
  NavigateParams params(GetSingletonTabNavigateParams(browser, url));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowWebcompatReporter(Browser* browser) {
  content::WebContents* web_contents =
      browser->tab_strip_model()->GetActiveWebContents();
  if (!web_contents)
    return;

  OpenWebcompatReporterDialog(web_contents);
}

#if BUILDFLAG(IPFS_ENABLED)
void ShareLocalFileUsingIPFS(Browser* browser) {
  auto* web_contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!web_contents)
    return;
  ipfs::IPFSTabHelper* helper =
      ipfs::IPFSTabHelper::FromWebContents(web_contents);
  helper->ShowImportDialog(ui::SelectFileDialog::SELECT_OPEN_FILE);
}

void ShareLocalFolderUsingIPFS(Browser* browser) {
  auto* web_contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!web_contents)
    return;
  ipfs::IPFSTabHelper* helper =
      ipfs::IPFSTabHelper::FromWebContents(web_contents);
  helper->ShowImportDialog(ui::SelectFileDialog::SELECT_EXISTING_FOLDER);
}
#endif

void ShowPresearchWallet(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kPresearchUIWalletURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowExtensionSettings(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kExtensionSettingsURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowWalletSettings(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kWalletSettingsURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowIPFS(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kIPFSWebUIURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

}  // namespace presearch
