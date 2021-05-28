/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define HasRegisteredGroupName HasRegisteredGroupName_ChromiumImpl
#define ContentSettingsTypeToGroupName \
  ContentSettingsTypeToGroupName_ChromiumImpl

// clang-format off
#define PRESEARCH_CONTENT_SETTINGS_TYPE_GROUP_NAMES_LIST               \
  {ContentSettingsType::PRESEARCH_ADS, nullptr},                       \
  {ContentSettingsType::PRESEARCH_COSMETIC_FILTERING, nullptr},        \
  {ContentSettingsType::PRESEARCH_TRACKERS, nullptr},                  \
  {ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES, nullptr}, \
  {ContentSettingsType::PRESEARCH_FINGERPRINTING_V2, nullptr},         \
  {ContentSettingsType::PRESEARCH_SHIELDS, nullptr},                   \
  {ContentSettingsType::PRESEARCH_REFERRERS, nullptr},                 \
  {ContentSettingsType::PRESEARCH_COOKIES, nullptr},
// clang-format on

#define PRESEARCH_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_FROM_GROUP_NAME \
  if (name == "autoplay")                                                \
    return ContentSettingsType::AUTOPLAY;

#include "../../../../../../../chrome/browser/ui/webui/settings/site_settings_helper.cc"

#undef PRESEARCH_CONTENT_SETTINGS_TYPE_GROUP_NAMES_LIST
#undef PRESEARCH_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_FROM_GROUP_NAME
#undef ContentSettingsTypeToGroupName
#undef HasRegisteredGroupName

namespace site_settings {

bool HasRegisteredGroupName(ContentSettingsType type) {
  if (type == ContentSettingsType::AUTOPLAY)
    return true;
  return HasRegisteredGroupName_ChromiumImpl(type);
}

base::StringPiece ContentSettingsTypeToGroupName(ContentSettingsType type) {
  if (type == ContentSettingsType::AUTOPLAY)
    return "autoplay";
  return ContentSettingsTypeToGroupName_ChromiumImpl(type);
}

}  // namespace site_settings
