/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/toolbar/presearch_location_bar_model_delegate.h"

#include "base/strings/utf_string_conversions.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"
#include "testing/gtest/include/gtest/gtest.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#endif

using PresearchLocationBarModelDelegateTest = testing::Test;

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
TEST_F(PresearchLocationBarModelDelegateTest, ResolvesEthereumRemoteClient) {
  GURL url(ethereum_remote_client_base_url);
  base::string16 formatted_url = base::UTF8ToUTF16(url.spec());
  PresearchLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "presearch://wallet");
}
TEST_F(PresearchLocationBarModelDelegateTest,
    ResolvesEthereumRemoteClientPhishingRoute) {
  GURL url(ethereum_remote_client_phishing_url);
  base::string16 formatted_url = base::UTF8ToUTF16(url.spec());
  PresearchLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "presearch://wallet");
}
TEST_F(PresearchLocationBarModelDelegateTest,
    ResolvesEthereumRemoteClientENSRoute) {
  GURL url(ethereum_remote_client_ens_redirect_url);
  base::string16 formatted_url = base::UTF8ToUTF16(url.spec());
  PresearchLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "presearch://wallet");
}
#endif

TEST_F(PresearchLocationBarModelDelegateTest, ResolvesChromeSchemeToPresearch) {
  GURL url("chrome://sync/");
  base::string16 formatted_url = base::UTF8ToUTF16(url.spec());
  PresearchLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "presearch://sync/");
}
