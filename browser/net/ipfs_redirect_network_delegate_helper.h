/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_NET_IPFS_REDIRECT_NETWORK_DELEGATE_HELPER_H_
#define PRESEARCH_BROWSER_NET_IPFS_REDIRECT_NETWORK_DELEGATE_HELPER_H_

#include <memory>
#include "presearch/browser/net/url_context.h"
#include "net/base/completion_once_callback.h"
#include "net/http/http_response_headers.h"
#include "url/gurl.h"

namespace ipfs {

int OnBeforeURLRequest_IPFSRedirectWork(
    const presearch::ResponseCallback& next_callback,
    std::shared_ptr<presearch::PresearchRequestInfo> ctx);

int OnHeadersReceived_IPFSRedirectWork(
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url,
    const presearch::ResponseCallback& next_callback,
    std::shared_ptr<presearch::PresearchRequestInfo> ctx);

}  // namespace ipfs

#endif  // PRESEARCH_BROWSER_NET_IPFS_REDIRECT_NETWORK_DELEGATE_HELPER_H_
