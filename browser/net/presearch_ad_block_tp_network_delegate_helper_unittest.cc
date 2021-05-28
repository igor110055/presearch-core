/* Copyright (c) 2019 The Presearch Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch_ad_block_tp_network_delegate_helper.h"

#include <memory>
#include <string>
#include <vector>

#include "presearch/browser/net/url_context.h"
#include "presearch/common/network_constants.h"
#include "net/base/net_errors.h"
#include "testing/gtest/include/gtest/gtest.h"

using presearch::ResponseCallback;

TEST(PresearchAdBlockTPNetworkDelegateHelperTest, NoChangeURL) {
  const GURL url("https://bradhatesprimes.presearch.com/composite_numbers_ftw");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  int rc =
      OnBeforeURLRequest_AdBlockTPPreWork(ResponseCallback(), request_info);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchAdBlockTPNetworkDelegateHelperTest, EmptyRequestURL) {
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(GURL());
  int rc =
      OnBeforeURLRequest_AdBlockTPPreWork(ResponseCallback(), request_info);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(rc, net::OK);
}

TEST(PresearchAdBlockTPNetworkDelegateHelperTest, DevToolURL) {
  const GURL url("devtools://devtools/");
  auto request_info = std::make_shared<presearch::PresearchRequestInfo>(url);
  request_info->initiator_url =
      GURL("devtools://devtools/bundled/root/root.js");
  int rc =
      OnBeforeURLRequest_AdBlockTPPreWork(ResponseCallback(), request_info);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(rc, net::OK);
}
