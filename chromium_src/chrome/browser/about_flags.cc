/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/about_flags.h"

#include "base/strings/string_util.h"
#include "presearch/common/presearch_features.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_component_updater/browser/features.h"
#include "presearch/components/presearch_rewards/common/features.h"
#include "presearch/components/presearch_shields/common/features.h"
#include "presearch/components/presearch_sync/buildflags/buildflags.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/decentralized_dns/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/ntp_background_images/browser/features.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "presearch/components/speedreader/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/permissions/features.h"
#include "components/prefs/pref_service.h"
#include "net/base/features.h"

using presearch_shields::features::kPresearchAdblockCosmeticFiltering;
using presearch_shields::features::kPresearchAdblockCosmeticFilteringNative;
using presearch_shields::features::kPresearchAdblockCspRules;
using presearch_shields::features::kPresearchDomainBlock;
using presearch_shields::features::kPresearchExtensionNetworkBlocking;
using ntp_background_images::features::kPresearchNTPBrandedWallpaper;
using ntp_background_images::features::kPresearchNTPBrandedWallpaperDemo;
using ntp_background_images::features::kPresearchNTPSuperReferralWallpaper;

// clang-format seems to have a lot of issues with the macros in this
// file so we turn it off for the macro sections.
// clang-format off

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "presearch/components/sidebar/features.h"

#define SIDEBAR_FEATURE_ENTRIES \
    {"sidebar",                                                            \
     flag_descriptions::kPresearchSidebarName,                                 \
     flag_descriptions::kPresearchSidebarDescription,                          \
     kOsMac | kOsWin | kOsLinux,                                           \
     FEATURE_VALUE_TYPE(sidebar::kSidebarFeature)},
#else
#define SIDEBAR_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "presearch/components/speedreader/features.h"

#define SPEEDREADER_FEATURE_ENTRIES \
    {"presearch-speedreader",                                                    \
     flag_descriptions::kPresearchSpeedreaderName,                               \
     flag_descriptions::kPresearchSpeedreaderDescription, kOsDesktop,            \
     FEATURE_VALUE_TYPE(speedreader::kSpeedreaderFeature)},                  \
    {"speedreader-readability-backend",                                      \
     flag_descriptions::kPresearchSpeedreaderReadabilityName,                    \
     flag_descriptions::kPresearchSpeedreaderReadabilityDescription, kOsDesktop, \
     FEATURE_VALUE_TYPE(speedreader::kSpeedreaderReadabilityBackend)},
#else
#define SPEEDREADER_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
#include "presearch/components/presearch_sync/features.h"

#define PRESEARCH_SYNC_FEATURE_ENTRIES                                         \
    {"presearch-sync-v2",                                                      \
     flag_descriptions::kPresearchSyncName,                                    \
     flag_descriptions::kPresearchSyncDescription, kOsDesktop,                 \
     FEATURE_VALUE_TYPE(presearch_sync::features::kPresearchSync)},
#else
#define PRESEARCH_SYNC_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/features.h"

#define PRESEARCH_IPFS_FEATURE_ENTRIES                                         \
    {"presearch-ipfs",                                                         \
     flag_descriptions::kPresearchIpfsName,                                    \
     flag_descriptions::kPresearchIpfsDescription,                             \
     kOsDesktop | kOsAndroid,                                              \
     FEATURE_VALUE_TYPE(ipfs::features::kIpfsFeature)},
#else
#define PRESEARCH_IPFS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/common/features.h"

#define PRESEARCH_NATIVE_WALLET_FEATURE_ENTRIES                                  \
    {"native-presearch-wallet",                                                  \
     flag_descriptions::kNativePresearchWalletName,                              \
     flag_descriptions::kNativePresearchWalletDescription,                       \
     kOsDesktop | flags_ui::kOsAndroid,                                      \
     FEATURE_VALUE_TYPE(presearch_wallet::features::kNativePresearchWalletFeature)},
#else
#define PRESEARCH_NATIVE_WALLET_FEATURE_ENTRIES
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "presearch/components/decentralized_dns/features.h"

#define PRESEARCH_DECENTRALIZED_DNS_FEATURE_ENTRIES                             \
    {"presearch-decentralized-dns",                                             \
     flag_descriptions::kPresearchDecentralizedDnsName,                         \
     flag_descriptions::kPresearchDecentralizedDnsDescription,                  \
     kOsDesktop | kOsAndroid,                                               \
     FEATURE_VALUE_TYPE(decentralized_dns::features::kDecentralizedDns)},
#else
#define PRESEARCH_DECENTRALIZED_DNS_FEATURE_ENTRIES
#endif

#define PRESEARCH_FEATURE_ENTRIES                                               \
    {"use-dev-updater-url",                                                 \
     flag_descriptions::kUseDevUpdaterUrlName,                              \
     flag_descriptions::kUseDevUpdaterUrlDescription, kOsAll,               \
     FEATURE_VALUE_TYPE(presearch_component_updater::kUseDevUpdaterUrl)},       \
    {"presearch-ntp-branded-wallpaper",                                         \
     flag_descriptions::kPresearchNTPBrandedWallpaperName,                      \
     flag_descriptions::kPresearchNTPBrandedWallpaperDescription, kOsAll,       \
     FEATURE_VALUE_TYPE(kPresearchNTPBrandedWallpaper)},                        \
    {"presearch-ntp-branded-wallpaper-demo",                                    \
     flag_descriptions::kPresearchNTPBrandedWallpaperDemoName,                  \
     flag_descriptions::kPresearchNTPBrandedWallpaperDemoDescription, kOsAll,   \
     FEATURE_VALUE_TYPE(kPresearchNTPBrandedWallpaperDemo)},                    \
    {"presearch-adblock-cosmetic-filtering",                                    \
     flag_descriptions::kPresearchAdblockCosmeticFilteringName,                 \
     flag_descriptions::kPresearchAdblockCosmeticFilteringDescription, kOsAll,  \
     FEATURE_VALUE_TYPE(kPresearchAdblockCosmeticFiltering)},                   \
    {"presearch-adblock-cosmetic-filtering-native",                             \
     flag_descriptions::kPresearchAdblockCosmeticFilteringNativeName,           \
     flag_descriptions::kPresearchAdblockCosmeticFilteringNativeDescription,    \
     kOsMac | kOsWin | kOsLinux,                                            \
     FEATURE_VALUE_TYPE(kPresearchAdblockCosmeticFilteringNative)},             \
    {"presearch-adblock-csp-rules",                                             \
     flag_descriptions::kPresearchAdblockCspRulesName,                          \
     flag_descriptions::kPresearchAdblockCspRulesDescription, kOsAll,           \
     FEATURE_VALUE_TYPE(kPresearchAdblockCspRules)},                            \
    {"presearch-domain-block",                                                  \
     flag_descriptions::kPresearchDomainBlockName,                              \
     flag_descriptions::kPresearchDomainBlockDescription, kOsAll,               \
     FEATURE_VALUE_TYPE(kPresearchDomainBlock)},                                \
    {"presearch-extension-network-blocking",                                    \
     flag_descriptions::kPresearchExtensionNetworkBlockingName,                 \
     flag_descriptions::kPresearchExtensionNetworkBlockingDescription, kOsAll,  \
     FEATURE_VALUE_TYPE(kPresearchExtensionNetworkBlocking)},                   \
    SPEEDREADER_FEATURE_ENTRIES                                             \
    PRESEARCH_SYNC_FEATURE_ENTRIES                                              \
    PRESEARCH_IPFS_FEATURE_ENTRIES                                              \
    PRESEARCH_NATIVE_WALLET_FEATURE_ENTRIES                                     \
    SIDEBAR_FEATURE_ENTRIES                                                 \
    PRESEARCH_DECENTRALIZED_DNS_FEATURE_ENTRIES                                 \
    {"presearch-super-referral",                                                \
     flag_descriptions::kPresearchSuperReferralName,                            \
     flag_descriptions::kPresearchSuperReferralDescription,                     \
     flags_ui::kOsMac | flags_ui::kOsWin | flags_ui::kOsAndroid,            \
     FEATURE_VALUE_TYPE(kPresearchNTPSuperReferralWallpaper)},                  \
    {"presearch-ephemeral-storage",                                             \
     flag_descriptions::kPresearchEphemeralStorageName,                         \
     flag_descriptions::kPresearchEphemeralStorageDescription, kOsAll,          \
     FEATURE_VALUE_TYPE(net::features::kPresearchEphemeralStorage)},            \
    {"presearch-ephemeral-storage-keep-alive",                                  \
     flag_descriptions::kPresearchEphemeralStorageKeepAliveName,                \
     flag_descriptions::kPresearchEphemeralStorageKeepAliveDescription, kOsAll, \
     FEATURE_VALUE_TYPE(net::features::kPresearchEphemeralStorage)},            \
    {"presearch-permission-lifetime",                                           \
     flag_descriptions::kPresearchPermissionLifetimeName,                       \
     flag_descriptions::kPresearchPermissionLifetimeDescription, kOsAll,        \
     FEATURE_VALUE_TYPE(permissions::features::kPermissionLifetime)},       \
    {"presearch-rewards-bitflyer",                                              \
     flag_descriptions::kPresearchRewardsBitflyerName,                          \
     flag_descriptions::kPresearchRewardsBitflyerDescription, kOsDesktop,       \
     FEATURE_VALUE_TYPE(presearch_rewards::features::kBitflyerFeature)},


#define SetFeatureEntryEnabled SetFeatureEntryEnabled_ChromiumImpl
#include "../../../../chrome/browser/about_flags.cc"  // NOLINT
#undef SetFeatureEntryEnabled
#undef PRESEARCH_FEATURE_ENTRIES

// clang-format on

namespace about_flags {

void UpdatePresearchMediaRouterPref(const std::string& internal_name,
                                Profile* profile) {
  bool enable = false;
  if (base::EndsWith(internal_name, "@1", base::CompareCase::SENSITIVE)) {
    enable = true;
  }
  if (base::StartsWith(internal_name, "load-media-router-component-extension",
                       base::CompareCase::SENSITIVE)) {
    profile->GetPrefs()->SetBoolean(kPresearchEnabledMediaRouter, enable);
  }
}

void SetFeatureEntryEnabled(flags_ui::FlagsStorage* flags_storage,
                            const std::string& internal_name,
                            bool enable) {
  UpdatePresearchMediaRouterPref(internal_name,
                             ProfileManager::GetActiveUserProfile());
  SetFeatureEntryEnabled_ChromiumImpl(flags_storage, internal_name, enable);
}

}  // namespace about_flags
