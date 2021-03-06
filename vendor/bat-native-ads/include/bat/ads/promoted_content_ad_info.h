/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_INCLUDE_BAT_ADS_PROMOTED_CONTENT_AD_INFO_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_INCLUDE_BAT_ADS_PROMOTED_CONTENT_AD_INFO_H_

#include <string>

#include "bat/ads/ad_info.h"
#include "bat/ads/export.h"
#include "bat/ads/result.h"

namespace ads {

struct ADS_EXPORT PromotedContentAdInfo : AdInfo {
  PromotedContentAdInfo();
  PromotedContentAdInfo(const PromotedContentAdInfo& info);
  ~PromotedContentAdInfo();

  bool IsValid() const;

  std::string ToJson() const;
  Result FromJson(const std::string& json);

  std::string title;
  std::string description;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_INCLUDE_BAT_ADS_PROMOTED_CONTENT_AD_INFO_H_
