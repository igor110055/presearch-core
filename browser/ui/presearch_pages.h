/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_PRESEARCH_PAGES_H_
#define PRESEARCH_BROWSER_UI_PRESEARCH_PAGES_H_

#include "presearch/components/ipfs/buildflags/buildflags.h"

class Browser;

namespace presearch {

void ShowPresearchAdblock(Browser* browser);
void ShowWebcompatReporter(Browser* browser);
void ShowPresearchRewards(Browser* browser);
void ShowPresearchWallet(Browser* browser);
void ShowExtensionSettings(Browser* browser);
void ShowWalletSettings(Browser* browser);
void ShowSync(Browser* browser);

#if BUILDFLAG(IPFS_ENABLED)
void ShareLocalFileUsingIPFS(Browser* browser);
void ShareLocalFolderUsingIPFS(Browser* browser);
#endif

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_UI_PRESEARCH_PAGES_H_
