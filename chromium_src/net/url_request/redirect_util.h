/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_NET_URL_REQUEST_REDIRECT_UTIL_H_
#define PRESEARCH_CHROMIUM_SRC_NET_URL_REQUEST_REDIRECT_UTIL_H_

#define UpdateHttpRequest                                                \
    UpdateHttpRequest_ChromiumImpl(                                      \
        const GURL& original_url,                                        \
        const std::string& original_method,                              \
        const RedirectInfo& redirect_info,                               \
        const base::Optional<std::vector<std::string>>& removed_headers, \
        const base::Optional<net::HttpRequestHeaders>& modified_headers, \
        HttpRequestHeaders* request_headers,                             \
        bool* should_clear_upload);                                      \
    NET_EXPORT static void UpdateHttpRequest

#include "../../../../net/url_request/redirect_util.h"
#undef UpdateHttpRequest

#endif  // PRESEARCH_CHROMIUM_SRC_NET_URL_REQUEST_REDIRECT_UTIL_H_
