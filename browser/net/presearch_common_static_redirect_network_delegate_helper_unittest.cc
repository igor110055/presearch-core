/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch.common_static_redirect_network_delegate_helper.h"

#include <memory>
#include <string>

#include "base/command_line.h"
#include "presearch/browser/net/url_context.h"
#include "presearch.common/network_constants.h"
#include "presearch.components/presearch.component_updater/browser/switches.h"
#include "components/component_updater/component_updater_url_constants.h"
#include "net/base/net_errors.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"
#include "url/url_constants.h"

using presearch::ResponseCallback;

namespace {
const char kComponentUpdaterProxy[] = "https://componentupdater.presearch.org";
}

TEST(PresearchCommonStaticRedirectNetworkDelegateHelperTest,
     ModifyComponentUpdaterURL) {
  presearch::SetUpdateURLHostForTesting(true);
  const std::string query_string("?foo=bar");
  const GURL url(component_updater::kUpdaterJSONDefaultUrl + query_string);
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  const GURL expected_url(
      std::string(presearch::kUpdaterTestingEndpoint + query_string));

  int rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  EXPECT_EQ(GURL(request_info->new_url_spec), expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchCommonStaticRedirectNetworkDelegateHelperTest,
     ModifyComponentUpdaterURLDev) {
  presearch::SetUpdateURLHostForTesting(true);
  base::CommandLine::ForCurrentProcess()->AppendSwitch(
      presearch.component_updater::kUseGoUpdateDev);
  const std::string query_string("?foo=bar");
  const GURL url(component_updater::kUpdaterJSONDefaultUrl + query_string);
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  const GURL expected_url(
      std::string(presearch::kUpdaterTestingEndpoint + query_string));

  int rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  EXPECT_EQ(GURL(request_info->new_url_spec), expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchCommonStaticRedirectNetworkDelegateHelperTest,
     NoModifyComponentUpdaterURL) {
  const GURL url(kComponentUpdaterProxy);
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);

  int rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  EXPECT_EQ(request_info->new_url_spec, GURL());
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchCommonStaticRedirectNetworkDelegateHelperTest,
     RedirectChromecastDownload) {
  const GURL url(
      "http://redirector.gvt1.com/edgedl/chromewebstore/"
      "random_hash/random_version_pkedcjkdefgpdelpbcmbmeomcjbeemfm.crx");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);

  int rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  const GURL redirect = GURL(request_info->new_url_spec);
  EXPECT_EQ(redirect.host(), kPresearchRedirectorProxy);
  EXPECT_TRUE(redirect.SchemeIs(url::kHttpsScheme));
  EXPECT_EQ(redirect.path(), url.path());
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchCommonStaticRedirectNetworkDelegateHelperTest,
     RedirectGoogleClients4) {
  const GURL url("https://clients4.google.com/chrome-sync/dev");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);

  int rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  const GURL redirect = GURL(request_info->new_url_spec);
  EXPECT_EQ(redirect.host(), kPresearchClients4Proxy);
  EXPECT_TRUE(redirect.SchemeIs(url::kHttpsScheme));
  EXPECT_EQ(redirect.path(), url.path());
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchCommonStaticRedirectNetworkDelegateHelperTest,
     RedirectBugsChromium) {
  // Check when we will redirect.
  const GURL url(
      "https://bugs.chromium.org/p/chromium/issues/"
      "entry?template=Crash%20Report&comment=IMPORTANT%20Chrome&labels="
      "Restrict-View-"
      "EditIssue%2CStability-Crash%2CUser-Submitted");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);

  int rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  const GURL redirect = GURL(request_info->new_url_spec);
  EXPECT_EQ(redirect.host(), "github.com");
  EXPECT_TRUE(redirect.SchemeIs(url::kHttpsScheme));
  EXPECT_EQ(redirect.path(), "/presearch/presearch-browser/issues/new");
  EXPECT_EQ(redirect.query(),
            "title=Crash%20Report&labels=crash&body=IMPORTANT%20Presearch");
  EXPECT_EQ(rc, net::OK);

  // Check when we should not redirect: wrong query keys count
  request_info.reset();
  const GURL url_fewer_keys(
      "https://bugs.chromium.org/p/chromium/issues/entry?template=A");
  request_info = std::make_shared<presearch::PresearchRequestInfo>(url_fewer_keys);
  rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                       request_info);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(rc, net::OK);

  // Check when we should not redirect: wrong query keys
  request_info.reset();
  const GURL url_wrong_keys(
      "https://bugs.chromium.org/p/chromium/issues/entry?t=A&l=B&c=C");
  request_info = std::make_shared<presearch::PresearchRequestInfo>(url_wrong_keys);
  rc = OnBeforeURLRequest_CommonStaticRedirectWork(ResponseCallback(),
                                                   request_info);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(rc, net::OK);
}
