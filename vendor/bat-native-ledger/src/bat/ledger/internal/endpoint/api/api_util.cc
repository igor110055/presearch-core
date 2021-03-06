/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "bat/ledger/internal/endpoint/api/api_util.h"

#include "bat/ledger/ledger.h"

namespace ledger {
namespace endpoint {
namespace api {

const char kDevelopment[] = "https://api.rewards.presearch.software";
const char kStaging[] = "https://api.rewards.presearchsoftware.com";
const char kProduction[] = "https://api.rewards.presearch.org";

std::string GetServerUrl(const std::string& path) {
  DCHECK(!path.empty());

  std::string url;
  switch (ledger::_environment) {
    case type::Environment::DEVELOPMENT:
      url = kDevelopment;
      break;
    case type::Environment::STAGING:
      url = kStaging;
      break;
    case type::Environment::PRODUCTION:
      url = kProduction;
      break;
  }

  return url + path;
}

}  // namespace api
}  // namespace endpoint
}  // namespace ledger
