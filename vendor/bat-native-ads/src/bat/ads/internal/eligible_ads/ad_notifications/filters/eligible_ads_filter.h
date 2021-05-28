/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_AD_NOTIFICATIONS_FILTERS_ELIGIBLE_ADS_FILTER_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_AD_NOTIFICATIONS_FILTERS_ELIGIBLE_ADS_FILTER_H_

#include "bat/ads/internal/bundle/creative_ad_notification_info.h"

namespace ads {

class EligibleAdsFilter {
 public:
  enum class Type { kPriority };

  virtual ~EligibleAdsFilter() = default;

  virtual CreativeAdNotificationList Apply(
      const CreativeAdNotificationList& ads) const = 0;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_AD_NOTIFICATIONS_FILTERS_ELIGIBLE_ADS_FILTER_H_
