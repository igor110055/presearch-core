/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_PRESEARCH_HTTPSE_NETWORK_DELEGATE_H_
#define PRESEARCH_BROWSER_NET_PRESEARCH_HTTPSE_NETWORK_DELEGATE_H_

#include "presearch/browser/net/url_context.h"

namespace presearch {

int OnBeforeURLRequest_HttpsePreFileWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<PresearchRequestInfo> ctx);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_PRESEARCH_NETWORK_DELEGATE_H_
