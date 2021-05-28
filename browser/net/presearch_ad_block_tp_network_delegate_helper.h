/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_PRESEARCH_AD_BLOCK_TP_NETWORK_DELEGATE_HELPER_H_
#define PRESEARCH_BROWSER_NET_PRESEARCH_AD_BLOCK_TP_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "presearch/browser/net/url_context.h"

namespace presearch {

int OnBeforeURLRequest_AdBlockTPPreWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<PresearchRequestInfo> ctx);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_PRESEARCH_AD_BLOCK_TP_NETWORK_DELEGATE_HELPER_H_
