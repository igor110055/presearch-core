/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_CLIENT_HELPER_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_CLIENT_HELPER_H_

#include "bat/ads/ads_client.h"

namespace ads {

class AdsClientHelper {
 public:
  explicit AdsClientHelper(AdsClient* ads_client);

  ~AdsClientHelper();

  static AdsClient* Get();

  static bool HasInstance();
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_CLIENT_HELPER_H_
