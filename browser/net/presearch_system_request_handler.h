/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_PRESEARCH_SYSTEM_REQUEST_HANDLER_H_
#define PRESEARCH_BROWSER_NET_PRESEARCH_SYSTEM_REQUEST_HANDLER_H_

#include <string>

namespace network {
struct ResourceRequest;
}

extern const char kPresearchServicesKeyHeader[];

namespace presearch {

std::string PresearchServicesKeyForTesting();

void AddPresearchServicesKeyHeader(network::ResourceRequest* url_request);

network::ResourceRequest OnBeforeSystemRequest(
    const network::ResourceRequest& url_request);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_PRESEARCH_SYSTEM_REQUEST_HANDLER_H_
