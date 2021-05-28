/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_HISTORY_SORTS_ADS_HISTORY_SORT_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_HISTORY_SORTS_ADS_HISTORY_SORT_H_

#include <deque>

#include "bat/ads/ad_history_info.h"

namespace ads {

class AdsHistorySort {
 public:
  virtual ~AdsHistorySort() = default;

  virtual std::deque<AdHistoryInfo> Apply(
      const std::deque<AdHistoryInfo>& history) const = 0;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_HISTORY_SORTS_ADS_HISTORY_SORT_H_
