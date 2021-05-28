/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define PRESEARCH_INIT PresearchInit();
#include "../../../../../../components/content_settings/core/browser/content_settings_registry.cc"
#undef PRESEARCH_INIT

#include "presearch/components/presearch_shields/common/presearch_shield_constants.h"
namespace content_settings {

namespace {

const struct {
  const char* name;
  ContentSettingsType type;
} kPresearchContentSettingstypes[] = {
    {presearch_shields::kAds, ContentSettingsType::PRESEARCH_ADS},
    {presearch_shields::kCosmeticFiltering,
     ContentSettingsType::PRESEARCH_COSMETIC_FILTERING},
    {presearch_shields::kTrackers, ContentSettingsType::PRESEARCH_TRACKERS},
    {presearch_shields::kHTTPUpgradableResources,
     ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES},
    {presearch_shields::kFingerprintingV2,
     ContentSettingsType::PRESEARCH_FINGERPRINTING_V2},
    {presearch_shields::kPresearchShields, ContentSettingsType::PRESEARCH_SHIELDS},
    {presearch_shields::kReferrers, ContentSettingsType::PRESEARCH_REFERRERS},
    {presearch_shields::kCookies, ContentSettingsType::PRESEARCH_COOKIES},
};

}  // namespace

void ContentSettingsRegistry::RegisterPresearchContentSettingsTypes(
    const ContentSettingsType& type,
    const std::string& name) {
  Register(type, name, CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::SYNCABLE,
           AllowlistedSchemes(kChromeUIScheme, kChromeDevToolsScheme),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK,
                         CONTENT_SETTING_ASK,
                         CONTENT_SETTING_DETECT_IMPORTANT_CONTENT),
           WebsiteSettingsInfo::SINGLE_ORIGIN_WITH_EMBEDDED_EXCEPTIONS_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);
}

void ContentSettingsRegistry::PresearchInit() {
  // Add CONTENT_SETTING_ASK for autoplay
  // Note ASK has been deprecated, only keeping it for
  // DiscardObsoleteAutoplayAsk test case
  content_settings_info_.erase(ContentSettingsType::AUTOPLAY);
  website_settings_registry_->UnRegister(ContentSettingsType::AUTOPLAY);
  Register(ContentSettingsType::AUTOPLAY, "autoplay", CONTENT_SETTING_ALLOW,
           WebsiteSettingsInfo::UNSYNCABLE, AllowlistedSchemes(),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK,
                         CONTENT_SETTING_ASK),
           WebsiteSettingsInfo::SINGLE_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Register Presearch-specific types, defaulting them to CONTENT_SETTING_BLOCK.
  for (auto presearch_type : kPresearchContentSettingstypes)
    RegisterPresearchContentSettingsTypes(presearch_type.type, presearch_type.name);

  // Disable background sync by default (presearch/presearch-browser#4709)
  content_settings_info_.erase(ContentSettingsType::BACKGROUND_SYNC);
  website_settings_registry_->UnRegister(ContentSettingsType::BACKGROUND_SYNC);
  Register(ContentSettingsType::BACKGROUND_SYNC, "background-sync",
           CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::UNSYNCABLE,
           AllowlistedSchemes(),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK),
           WebsiteSettingsInfo::SINGLE_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_ORIGINS_ONLY);

  // Disable motion sensors by default (presearch/presearch-browser#4789)
  content_settings_info_.erase(ContentSettingsType::SENSORS);
  website_settings_registry_->UnRegister(ContentSettingsType::SENSORS);
  Register(ContentSettingsType::SENSORS, "sensors", CONTENT_SETTING_BLOCK,
           WebsiteSettingsInfo::UNSYNCABLE, AllowlistedSchemes(),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK),
           WebsiteSettingsInfo::SINGLE_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);
}

}  // namespace content_settings
