/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_AD_REWARDS_PAYMENTS_PAYMENT_INFO_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_AD_REWARDS_PAYMENTS_PAYMENT_INFO_H_

#include <cstdint>
#include <string>
#include <vector>

namespace ads {

struct PaymentInfo {
  double balance = 0.0;
  std::string month;
  uint64_t transaction_count = 0;
};

using PaymentList = std::vector<PaymentInfo>;

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_AD_REWARDS_PAYMENTS_PAYMENT_INFO_H_
