/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_PRESEARCH_AD_BLOCK_CSP_NETWORK_DELEGATE_HELPER_H_
#define PRESEARCH_BROWSER_NET_PRESEARCH_AD_BLOCK_CSP_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "base/memory/scoped_refptr.h"
#include "presearch/browser/net/url_context.h"

namespace net {
class HttpResponseHeaders;
}  // namespace net

class GURL;

namespace presearch {

int OnHeadersReceived_AdBlockCspWork(
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url,
    const presearch::ResponseCallback& next_callback,
    std::shared_ptr<presearch::PresearchRequestInfo> ctx);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_NET_PRESEARCH_AD_BLOCK_CSP_NETWORK_DELEGATE_HELPER_H_
