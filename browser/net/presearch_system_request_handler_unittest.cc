/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/net/presearch_system_request_handler.h"

#include <string>

#include "services/network/public/cpp/resource_request.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace presearch {

TEST(PresearchSystemRequestHandlerTest, AddPresearchServiceKeyHeaderForPresearch) {
  GURL url("https://demo.presearch.com");
  network::ResourceRequest request;

  request.url = url;
  presearch::AddPresearchServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kPresearchServicesKeyHeader, &key));
  EXPECT_EQ(key, PresearchServicesKeyForTesting());
}

TEST(PresearchSystemRequestHandlerTest, AddPresearchServiceKeyHeaderForPresearchSoftware) {
  GURL url("https://demo.presearchsoftware.com");
  network::ResourceRequest request;

  request.url = url;
  presearch::AddPresearchServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kPresearchServicesKeyHeader, &key));
  EXPECT_EQ(key, PresearchServicesKeyForTesting());
}

TEST(PresearchSystemRequestHandlerTest, DontAddPresearchServiceKeyHeader) {
  GURL url("https://demo.example.com");
  network::ResourceRequest request;

  request.url = url;
  presearch::AddPresearchServicesKeyHeader(&request);
  std::string key;
  EXPECT_FALSE(request.headers.GetHeader(kPresearchServicesKeyHeader, &key));
}

}  // namespace presearch
