/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_TARGETING_DATA_TYPES_BEHAVIORAL_PURCHASE_INTENT_PURCHASE_INTENT_SIGNAL_HISTORY_INFO_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_TARGETING_DATA_TYPES_BEHAVIORAL_PURCHASE_INTENT_PURCHASE_INTENT_SIGNAL_HISTORY_INFO_H_

#include <cstdint>
#include <string>

#include "bat/ads/result.h"

namespace ads {

struct PurchaseIntentSignalHistoryInfo {
  PurchaseIntentSignalHistoryInfo();
  PurchaseIntentSignalHistoryInfo(const int64_t timestamp_in_seconds,
                                  const uint16_t weight);
  PurchaseIntentSignalHistoryInfo(const PurchaseIntentSignalHistoryInfo& info);
  ~PurchaseIntentSignalHistoryInfo();

  bool operator==(const PurchaseIntentSignalHistoryInfo& rhs) const;
  bool operator!=(const PurchaseIntentSignalHistoryInfo& rhs) const;

  std::string ToJson() const;
  Result FromJson(const std::string& json);

  int64_t timestamp_in_seconds = 0;
  uint16_t weight = 0;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_TARGETING_DATA_TYPES_BEHAVIORAL_PURCHASE_INTENT_PURCHASE_INTENT_SIGNAL_HISTORY_INFO_H_
