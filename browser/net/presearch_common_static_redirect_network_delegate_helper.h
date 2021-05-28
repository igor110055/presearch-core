/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_PRESEARCH_COMMON_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_
#define PRESEARCH_BROWSER_NET_PRESEARCH_COMMON_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "presearch/browser/net/url_context.h"

class GURL;

namespace presearch {

extern const char kUpdaterTestingEndpoint[];

int OnBeforeURLRequest_CommonStaticRedirectWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<PresearchRequestInfo> ctx);

int OnBeforeURLRequest_CommonStaticRedirectWorkForGURL(
    const GURL& url,
    GURL* new_url);

void SetUpdateURLHostForTesting(bool testing);
}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_PRESEARCH_COMMON_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_
