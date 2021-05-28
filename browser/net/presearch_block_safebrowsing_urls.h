/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_PRESEARCH_BLOCK_SAFEBROWSING_URLS_H_
#define PRESEARCH_BROWSER_NET_PRESEARCH_BLOCK_SAFEBROWSING_URLS_H_

class GURL;

namespace presearch {

int OnBeforeURLRequest_BlockSafeBrowsingReportingURLs(const GURL& url,
                                                      GURL* new_url);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_PRESEARCH_BLOCK_SAFEBROWSING_URLS_H_
