/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_GLOBAL_PRIVACY_CONTROL_NETWORK_DELEGATE_HELPER_H_
#define PRESEARCH_BROWSER_NET_GLOBAL_PRIVACY_CONTROL_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "presearch/browser/net/url_context.h"

namespace presearch {

int OnBeforeStartTransaction_GlobalPrivacyControlWork(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<PresearchRequestInfo> ctx);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_GLOBAL_PRIVACY_CONTROL_NETWORK_DELEGATE_HELPER_H_
