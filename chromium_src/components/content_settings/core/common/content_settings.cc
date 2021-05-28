/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Leave a gap between Chromium values and our values in the kHistogramValue
// array so that we don't have to renumber when new content settings types are
// added upstream.
namespace {

// Do not change the value arbitrarily. This variable is only used for the
// DCHECK in ContentSettingTypeToHistogramValue function below.
constexpr int kPresearchValuesStart = 1000;

constexpr int presearch_value(int incr) {
  return kPresearchValuesStart + incr;
}

}  // namespace

// clang-format off
#define PRESEARCH_HISTOGRAM_VALUE_LIST                                        \
  {ContentSettingsType::PRESEARCH_ADS, presearch_value(0)},                       \
  {ContentSettingsType::PRESEARCH_COSMETIC_FILTERING, presearch_value(1)},        \
  {ContentSettingsType::PRESEARCH_TRACKERS, presearch_value(2)},                  \
  {ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES, presearch_value(3)}, \
  {ContentSettingsType::PRESEARCH_FINGERPRINTING_V2, presearch_value(4)},         \
  {ContentSettingsType::PRESEARCH_SHIELDS, presearch_value(5)},                   \
  {ContentSettingsType::PRESEARCH_REFERRERS, presearch_value(6)},                 \
  {ContentSettingsType::PRESEARCH_COOKIES, presearch_value(7)},
// clang-format on

#define PRESEARCH_IS_RENDERER_CONTENT_SETTING \
  content_type == ContentSettingsType::AUTOPLAY ||

#define ContentSettingTypeToHistogramValue \
  ContentSettingTypeToHistogramValue_ChromiumImpl

#include "../../../../../../components/content_settings/core/common/content_settings.cc"

#undef ContentSettingTypeToHistogramValue
#undef PRESEARCH_HISTOGRAM_VALUE_LIST
#undef PRESEARCH_IS_RENDERER_CONTENT_SETTING

int ContentSettingTypeToHistogramValue(ContentSettingsType content_setting,
                                       size_t* num_values) {
  DCHECK(static_cast<int>(ContentSettingsType::NUM_TYPES) < kPresearchValuesStart);
  return ContentSettingTypeToHistogramValue_ChromiumImpl(content_setting,
                                                         num_values);
}
