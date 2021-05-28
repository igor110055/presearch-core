/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/ads_serve_server_util.h"

#include "bat/ads/ads.h"

namespace ads {
namespace serve {
namespace server {

namespace {

const char kProductionHost[] = "https://ads-serve.presearch.com";
const char kStagingHost[] = "https://ads-serve.presearchsoftware.com";
const char kDevelopmentHost[] = "https://ads-serve.presearch.software";

}  // namespace

std::string GetHost() {
  switch (g_environment) {
    case Environment::PRODUCTION: {
      return kProductionHost;
    }

    case Environment::STAGING: {
      return kStagingHost;
    }

    case Environment::DEVELOPMENT: {
      return kDevelopmentHost;
    }
  }
}

}  // namespace server
}  // namespace serve
}  // namespace ads
