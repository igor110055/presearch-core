/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_

class PresearchBrowsingDataRemoverDelegate;

#define PRESEARCH_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H \
  friend class PresearchBrowsingDataRemoverDelegate;

#include "../../../../../chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"
#undef PRESEARCH_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_
