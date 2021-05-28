/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch_static_redirect_network_delegate_helper.h"

#include <memory>
#include <string>

#include "base/strings/string_util.h"
#include "presearch/browser/net/url_context.h"
#include "presearch/browser/translate/buildflags/buildflags.h"
#include "presearch.common/translate_network_constants.h"
#include "components/component_updater/component_updater_url_constants.h"
#include "net/base/net_errors.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"
#include "url/url_constants.h"

using presearch::ResponseCallback;

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, NoModifyTypicalURL) {
  const GURL url("https://bradhatesprimes.presearch.org/composite_numbers_ftw");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyGeoURL) {
  const GURL url(
      "https://www.googleapis.com/geolocation/v1/geolocate?key=2_3_5_7");
  const GURL expected_url(GOOGLEAPIS_ENDPOINT GOOGLEAPIS_API_KEY);

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRLSet1) {
  const GURL url(
      "https://dl.google.com/release2/chrome_component/AJ4r388iQSJq_4819/"
      "4819_all_crl-set-5934829738003798040.data.crx3");
  const GURL expected_url(
      "https://crlsets.presearch.org/release2/chrome_component/"
      "AJ4r388iQSJq_4819/4819_all_crl-set-5934829738003798040.data.crx3");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRLSet2) {
  const GURL url(
      "https://r2---sn-8xgp1vo-qxoe.gvt1.com/edgedl/release2/"
      "chrome_component/AJ4r388iQSJq_4819/4819_all_crl-set-5934829738003798040"
      ".data.crx3");
  const GURL expected_url(
      "https://crlsets.presearch.org/edgedl/release2/chrome_compone"
      "nt/AJ4r388iQSJq_4819/4819_all_crl-set-5934829738003798040.data.crx3");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRLSet3) {
  const GURL url(
      "https://www.google.com/dl/release2/chrome_component/LLjIBPPmveI_4988/"
      "4988_all_crl-set-6296993568184466307.data.crx3");
  const GURL expected_url(
      "https://crlsets.presearch.org/dl/release2/chrome_component/LLjIBPPmveI_4988/"
      "4988_all_crl-set-6296993568184466307.data.crx3");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRXDownload) {
  const GURL url(
      "https://clients2.googleusercontent.com/crx/blobs/QgAAAC6zw0qH2DJtn"
      "Xe8Z7rUJP1RM6lX7kVcwkQ56ujmG3AWYOAkxoNnIdnEBUz_"
      "3z4keVhjzzAF10srsaL7lrntfB"
      "IflcYIrTziwX3SUS9i_P-CAMZSmuV5tdQl-Roo6cnVC_GRzKsnZSKm1Q/"
      "extension_2_0_67"
      "3_0.crx");
  const GURL expected_url(
      "https://crxdownload.presearch.org/crx/blobs/QgAAAC6"
      "zw0qH2DJtnXe8Z7rUJP1RM6lX7kVcwkQ56ujmG3AWYOAkxoNnIdnEBUz_"
      "3z4keVhjzzAF10sr"
      "saL7lrntfBIflcYIrTziwX3SUS9i_P-CAMZSmuV5tdQl-Roo6cnVC_GRzKsnZSKm1Q/"
      "extens"
      "ion_2_0_673_0.crx");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRLSet1_http) {
  const GURL url(
      "http://dl.google.com/release2/chrome_component/AJ4r388iQSJq_4819/"
      "4819_all_crl-set-5934829738003798040.data.crx3");
  const GURL expected_url(
      "https://crlsets.presearch.org/release2/chrome_component/"
      "AJ4r388iQSJq_4819/4819_all_crl-set-5934829738003798040.data.crx3");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRLSet2_http) {
  const GURL url(
      "http://r2---sn-8xgp1vo-qxoe.gvt1.com/edgedl/release2/"
      "chrome_component/AJ4r388iQSJq_4819/4819_all_crl-set-5934829738003798040"
      ".data.crx3");
  const GURL expected_url(
      "https://crlsets.presearch.org/edgedl/release2/chrome_compone"
      "nt/AJ4r388iQSJq_4819/4819_all_crl-set-5934829738003798040.data.crx3");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRLSet3_http) {
  const GURL url(
      "http://www.google.com/dl/release2/chrome_component/LLjIBPPmveI_4988/"
      "4988_all_crl-set-6296993568184466307.data.crx3");
  const GURL expected_url(
      "https://crlsets.presearch.org/dl/release2/chrome_component/LLjIBPPmveI_4988/"
      "4988_all_crl-set-6296993568184466307.data.crx3");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyCRXDownload_http) {
  const GURL url(
      "http://clients2.googleusercontent.com/crx/blobs/QgAAAC6zw0qH2DJtn"
      "Xe8Z7rUJP1RM6lX7kVcwkQ56ujmG3AWYOAkxoNnIdnEBUz_"
      "3z4keVhjzzAF10srsaL7lrntfB"
      "IflcYIrTziwX3SUS9i_P-CAMZSmuV5tdQl-Roo6cnVC_GRzKsnZSKm1Q/"
      "extension_2_0_67"
      "3_0.crx");
  const GURL expected_url(
      "https://crxdownload.presearch.org/crx/blobs/QgAAAC6"
      "zw0qH2DJtnXe8Z7rUJP1RM6lX7kVcwkQ56ujmG3AWYOAkxoNnIdnEBUz_"
      "3z4keVhjzzAF10sr"
      "saL7lrntfBIflcYIrTziwX3SUS9i_P-CAMZSmuV5tdQl-Roo6cnVC_GRzKsnZSKm1Q/"
      "extens"
      "ion_2_0_673_0.crx");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifySafeBrowsingURLV4) {
  presearch::SetSafeBrowsingEndpointForTesting(true);
  const GURL url(
      "https://safebrowsing.googleapis.com/v4/"
      "threatListUpdates:fetch?$req=ChkKCGNocm9taXVtEg02Ni");
  GURL::Replacements replacements;
  replacements.SetHostStr(presearch::kSafeBrowsingTestingEndpoint);
  const GURL expected_url(url.ReplaceComponents(replacements));

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifySafeBrowsingURLV5) {
  presearch::SetSafeBrowsingEndpointForTesting(true);
  const GURL url(
      "https://safebrowsing.googleapis.com/v5/"
      "threatListUpdates:fetch?$req=ChkKCGNocm9taXVtEg02Ni");
  GURL::Replacements replacements;
  replacements.SetHostStr(presearch::kSafeBrowsingTestingEndpoint);
  GURL expected_url(url.ReplaceComponents(replacements));

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyGvt1) {
  const GURL url(
      "http://redirector.gvt1.com/edgedl/release2/"
      "NfaZYtcKdtFc0LUvFkcNFA_0.3/AKveSIjhHAm2K09XAMovFEQ");
  const GURL expected_url(
      "https://redirector.presearch.org/edgedl/release2/"
      "NfaZYtcKdtFc0LUvFkcNFA_0.3/AKveSIjhHAm2K09XAMovFEQ");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, ModifyGoogleDl) {
  const GURL url(
      "http://dl.google.com/release2/"
      "NfaZYtcKdtFc0LUvFkcNFA_0.3/AKveSIjhHAm2K09XAMovFEQ");
  const GURL expected_url(
      "https://redirector.presearch.org/release2/"
      "NfaZYtcKdtFc0LUvFkcNFA_0.3/AKveSIjhHAm2K09XAMovFEQ");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, DontModifyGvt1ForWidevine) {
  const GURL url(
      "http://r2---sn-n4v7sn7y.gvt1.com/edgedl/chromewebstore/"
      "L2Nocm9tZV9leHRlbnNpb24vYmxvYnMvYjYxQUFXaFBmeUtPbVFUYUh"
      "mRGV0MS1Wdw/4.10.1610.0_oimompecagnajdejgnnjijobebaeigek"
      ".crx");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, "");
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest,
     DontModifyGoogleDlForWidevine) {
  const GURL url(
      "http://dl.google.com/edgedl/chromewebstore/"
      "L2Nocm9tZV9leHRlbnNpb24vYmxvYnMvYjYxQUFXaFBmeUtPbVFUYUh"
      "mRGV0MS1Wdw/4.10.1610.0_oimompecagnajdejgnnjijobebaeigek"
      ".crx");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, "");
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest,
     ModifySafeBrowsingFileCheckURL) {
  presearch::SetSafeBrowsingEndpointForTesting(true);
  const GURL url(
      "https://sb-ssl.google.com/safebrowsing/clientreport/download?"
      "key=DUMMY_KEY");
  GURL expected_url(
      "https://sb-ssl.presearch.org/safebrowsing/clientreport/download?"
      "key=DUMMY_KEY");

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

#if BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_GO)
TEST(PresearchStaticRedirectNetworkDelegateHelperTest, RedirectTranslate) {
  const std::string query_string(
      "?cb=cr.googleTranslate.onTranslateElementLoad&aus=true&"
      "clc=cr.googleTranslate.onLoadCSS&"
      "jlc=cr.googleTranslate.onLoadJavascript&hl=en&key=DUMMY_KEY");
  const std::string path_string("/translate_a/element.js");
  const std::string google_host_string("https://translate.googleapis.com");
  const GURL url(google_host_string + path_string + query_string);
  const GURL expected_url(kPresearchTranslateServer + path_string + query_string);

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchStaticRedirectNetworkDelegateHelperTest, RedirectTranslateLanguage) {
  const GURL url(
      "https://translate.googleapis.com/translate_a/l?"
      "client=chrome&hl=en&key=DUMMY_KEY");
  const GURL expected_url(kPresearchTranslateLanguageEndpoint);

  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_StaticRedirectWork(ResponseCallback(), request_info);
  EXPECT_EQ(request_info->new_url_spec, expected_url);
  EXPECT_EQ(rc, net::OK);
}
#endif
