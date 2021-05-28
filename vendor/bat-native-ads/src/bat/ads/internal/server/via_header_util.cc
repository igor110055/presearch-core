/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/via_header_util.h"

#include "base/strings/stringprintf.h"
#include "bat/ads/ads.h"

namespace ads {
namespace server {

namespace {
const int kVersion = 1;
}  // namespace

std::string BuildViaHeader() {
  return base::StringPrintf(
      "Via: 1.%d presearch, 1.1 ads-serve.presearch.org (Apache/1.%d)",
      g_sys_info.is_uncertain_future ? 1 : 0, kVersion);
}

}  // namespace server
}  // namespace ads
