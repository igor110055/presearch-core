/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "presearch/components/presearch_ads/browser/test_util.h"

#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"
#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/components/presearch_ads/browser/ads_service.h"
#include "presearch/components/presearch_rewards/browser/rewards_service.h"
#include "chrome/browser/bitmap_fetcher/bitmap_fetcher_service_factory.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/test/base/testing_profile.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/sync_preferences/pref_service_mock_factory.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"

namespace presearch_ads {

std::unique_ptr<Profile> CreatePresearchAdsProfile(const base::FilePath& path) {
  BitmapFetcherServiceFactory::GetInstance();
  AdsServiceFactory::GetInstance();
  sync_preferences::PrefServiceMockFactory factory;
  auto registry = base::MakeRefCounted<user_prefs::PrefRegistrySyncable>();
  std::unique_ptr<sync_preferences::PrefServiceSyncable> prefs(
      factory.CreateSyncable(registry.get()));
  presearch_rewards::RewardsService::RegisterProfilePrefs(registry.get());
  AdsService::RegisterProfilePrefs(registry.get());
  RegisterUserProfilePrefs(registry.get());
  TestingProfile::Builder profile_builder;
  profile_builder.SetPrefService(std::move(prefs));
  profile_builder.SetPath(path);
  return profile_builder.Build();
}

}  // namespace presearch_ads
