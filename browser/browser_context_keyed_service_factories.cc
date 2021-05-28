/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/browser_context_keyed_service_factories.h"

#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/browser/presearch_shields/ad_block_pref_service_factory.h"
#include "presearch/browser/presearch_shields/cookie_pref_service_factory.h"
#include "presearch/browser/ntp_background_images/view_counter_service_factory.h"
#include "presearch/browser/permissions/permission_lifetime_manager_factory.h"
#include "presearch/browser/search_engines/search_engine_provider_service_factory.h"
#include "presearch/browser/search_engines/search_engine_tracker.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/greaselion/browser/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_GREASELION)
#include "presearch/browser/greaselion/greaselion_service_factory.h"
#endif

#if !defined(OS_ANDROID)
#include "presearch/browser/ui/bookmark/bookmark_prefs_service_factory.h"
#else
#include "presearch/browser/ntp_background_images/android/ntp_background_images_bridge.h"
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#include "presearch/browser/presearch_rewards/rewards_service_factory.h"
#endif

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/browser/presearch_wallet/presearch_wallet_service_factory.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/browser/ipfs/ipfs_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/browser/tor/tor_profile_service_factory.h"
#endif

namespace presearch {

void EnsureBrowserContextKeyedServiceFactoriesBuilt() {
  presearch_ads::AdsServiceFactory::GetInstance();
#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
  presearch_rewards::RewardsServiceFactory::GetInstance();
#endif
  presearch_shields::AdBlockPrefServiceFactory::GetInstance();
  presearch_shields::CookiePrefServiceFactory::GetInstance();
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionServiceFactory::GetInstance();
#endif
#if BUILDFLAG(ENABLE_TOR)
  TorProfileServiceFactory::GetInstance();
#endif
  SearchEngineProviderServiceFactory::GetInstance();
  SearchEngineTrackerFactory::GetInstance();
  ntp_background_images::ViewCounterServiceFactory::GetInstance();

#if !defined(OS_ANDROID)
  BookmarkPrefsServiceFactory::GetInstance();
#else
  ntp_background_images::NTPBackgroundImagesBridgeFactory::GetInstance();
#endif

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  PresearchWalletServiceFactory::GetInstance();
#endif

#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IpfsServiceFactory::GetInstance();
#endif
  PermissionLifetimeManagerFactory::GetInstance();
}

}  // namespace presearch
