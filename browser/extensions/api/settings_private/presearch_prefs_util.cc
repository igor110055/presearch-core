/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/settings_private/presearch_prefs_util.h"

#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_rewards/common/pref_names.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/presearch_wayback_machine/buildflags.h"
#include "presearch/components/crypto_dot_com/browser/buildflags/buildflags.h"
#include "presearch/components/decentralized_dns/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/ntp_background_images/common/pref_names.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "chrome/browser/extensions/api/settings_private/prefs_util.h"
#include "chrome/common/extensions/api/settings_private.h"
#include "chrome/common/pref_names.h"
#include "components/browsing_data/core/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/omnibox/browser/omnibox_prefs.h"

#if BUILDFLAG(ENABLE_PRESEARCH_WAYBACK_MACHINE)
#include "presearch/components/presearch_wayback_machine/pref_names.h"
#endif

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/browser/pref_names.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
#include "presearch/components/crypto_dot_com/common/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "presearch/components/sidebar/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/components/tor/pref_names.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "presearch/components/decentralized_dns/pref_names.h"
#endif

namespace extensions {

using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::
    kNewTabPageShowSponsoredImagesBackgroundImage;
using ntp_background_images::prefs::kNewTabPageSuperReferralThemesOption;

namespace settings_api = api::settings_private;

const PrefsUtil::TypedPrefMap& PresearchPrefsUtil::GetAllowlistedKeys() {
  // Static cache, similar to parent class
  static PrefsUtil::TypedPrefMap* s_presearch_allowlist = nullptr;
  if (s_presearch_allowlist)
    return *s_presearch_allowlist;
  s_presearch_allowlist = new PrefsUtil::TypedPrefMap();
  // Start with parent class allowlist
  const auto chromium_prefs = PrefsUtil::GetAllowlistedKeys();
  s_presearch_allowlist->insert(chromium_prefs.begin(), chromium_prefs.end());
  // Add Presearch values to the allowlist
  // import data
  (*s_presearch_allowlist)[kImportDialogExtensions] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kImportDialogPayments] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // Default Presearch shields
  (*s_presearch_allowlist)[kShieldsAdvancedViewEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kShieldsStatsBadgeVisible] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kAdControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kHTTPSEVerywhereControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNoScriptControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kGoogleLoginControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kFBEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kTwitterEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kLinkedInEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // appearance prefs
  (*s_presearch_allowlist)[kLocationBarIsWide] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kAutocompleteEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kTopSiteSuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kPresearchSuggestedSiteSuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[presearch_rewards::prefs::kHideButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kAskWidevineInstall] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageSuperReferralThemesOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#if BUILDFLAG(ENABLE_SIDEBAR)
  (*s_presearch_allowlist)[sidebar::kSidebarShowOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif
  // new tab prefs
  (*s_presearch_allowlist)[kNewTabPageShowSponsoredImagesBackgroundImage] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowBackgroundImage] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowClock] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowStats] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowRewards] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowToday] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowBinance] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowTogether] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowGemini] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kNewTabPageShowsOptions] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
  (*s_presearch_allowlist)[kCryptoDotComNewTabPageShowCryptoDotCom] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Presearch today prefs
  (*s_presearch_allowlist)[kPresearchTodaySources] =
      settings_api::PrefType::PREF_TYPE_DICTIONARY;
  // Clear browsing data on exit prefs.
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteBrowsingHistoryOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteDownloadHistoryOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteCacheOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteCookiesOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeletePasswordsOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteFormDataOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteSiteSettingsOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[browsing_data::prefs::kDeleteHostedAppsDataOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kAlwaysShowBookmarkBarOnNTP] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kMRUCyclingEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // WebTorrent pref
  (*s_presearch_allowlist)[kWebTorrentEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_PRESEARCH_WAYBACK_MACHINE)
  (*s_presearch_allowlist)[kPresearchWaybackMachineEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Hangouts pref
  (*s_presearch_allowlist)[kHangoutsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // IPFS Companion pref
  (*s_presearch_allowlist)[kIPFSCompanionEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // Presearch Wallet pref
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  (*s_presearch_allowlist)[kPresearchWalletWeb3Provider] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_presearch_allowlist)[kLoadCryptoWalletsOnStartup] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // IPFS pref
#if BUILDFLAG(IPFS_ENABLED)
  (*s_presearch_allowlist)[kIPFSResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_presearch_allowlist)[kIPFSAutoFallbackToGateway] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kIPFSPublicGatewayAddress] =
      settings_api::PrefType::PREF_TYPE_STRING;
  (*s_presearch_allowlist)[kIPFSAutoRedirectGateway] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kIPFSAutoRedirectDNSLink] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_presearch_allowlist)[kIpfsStorageMax] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif
  // Media Router Pref
  (*s_presearch_allowlist)[kPresearchEnabledMediaRouter] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Push Messaging Pref
  (*s_presearch_allowlist)[kPresearchGCMChannelStatus] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Omnibox pref
  (*s_presearch_allowlist)[omnibox::kPreventUrlElisionsInOmnibox] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_TOR)
  (*s_presearch_allowlist)[tor::prefs::kAutoOnionRedirect] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  (*s_presearch_allowlist)[prefs::kWebRTCIPHandlingPolicy] =
      settings_api::PrefType::PREF_TYPE_STRING;

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  (*s_presearch_allowlist)[decentralized_dns::kUnstoppableDomainsResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_presearch_allowlist)[decentralized_dns::kENSResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif

  return *s_presearch_allowlist;
}

}  // namespace extensions
