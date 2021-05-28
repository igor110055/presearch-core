/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch_system_request_handler.h"

#include "presearch/browser/net/presearch_block_safebrowsing_urls.h"
#include "presearch/browser/net/presearch_common_static_redirect_network_delegate_helper.h"
#include "presearch/browser/net/presearch_static_redirect_network_delegate_helper.h"
#include "presearch/common/network_constants.h"
#include "extensions/common/url_pattern.h"
#include "services/network/public/cpp/resource_request.h"
#include "url/gurl.h"

namespace presearch {

std::string PresearchServicesKeyForTesting() {
  return PRESEARCH_SERVICES_KEY;
}

void AddPresearchServicesKeyHeader(network::ResourceRequest* url_request) {
  static URLPattern presearch_proxy_pattern(URLPattern::SCHEME_HTTPS,
                                        kPresearchProxyPattern);
  static URLPattern presearchsoftware_proxy_pattern(URLPattern::SCHEME_HTTPS,
                                                kPresearchSoftwareProxyPattern);
  if (presearch_proxy_pattern.MatchesURL(url_request->url) ||
      presearchsoftware_proxy_pattern.MatchesURL(url_request->url)) {
    url_request->headers.SetHeaderIfMissing(kPresearchServicesKeyHeader,
                                            PRESEARCH_SERVICES_KEY);
  }
  return;
}

network::ResourceRequest OnBeforeSystemRequest(
    const network::ResourceRequest& url_request) {
  GURL new_url;
  presearch::OnBeforeURLRequest_BlockSafeBrowsingReportingURLs(url_request.url,
                                                           &new_url);
  presearch::OnBeforeURLRequest_StaticRedirectWorkForGURL(url_request.url,
                                                      &new_url);
  presearch::OnBeforeURLRequest_CommonStaticRedirectWorkForGURL(url_request.url,
                                                            &new_url);
  network::ResourceRequest patched_request = url_request;
  if (!new_url.is_empty()) {
    patched_request.url = new_url;
  }
  AddPresearchServicesKeyHeader(&patched_request);
  return patched_request;
}

}  // namespace presearch
