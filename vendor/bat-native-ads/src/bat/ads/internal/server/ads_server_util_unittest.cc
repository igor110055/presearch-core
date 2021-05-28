/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/ads_server_util.h"

#include "bat/ads/internal/unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- presearch_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsServerUtilTest, Production) {
  // Arrange
  SetEnvironment(Environment::PRODUCTION);

  // Act
  const std::string host = server::GetHost();

  // Assert
  const std::string expected_host = "https://ads-static.presearch.org";
  EXPECT_EQ(expected_host, host);
}

TEST(BatAdsServerUtilTest, Staging) {
  // Arrange
  SetEnvironment(Environment::STAGING);

  // Act
  const std::string host = server::GetHost();

  // Assert
  const std::string expected_host = "https://ads-serve.presearchsoftware.com";
  EXPECT_EQ(expected_host, host);
}

TEST(BatAdsServerUtilTest, Development) {
  // Arrange
  SetEnvironment(Environment::DEVELOPMENT);

  // Act
  const std::string host = server::GetHost();

  // Assert
  const std::string expected_host = "https://ads-serve.presearch.software";
  EXPECT_EQ(expected_host, host);
}

}  // namespace ads
