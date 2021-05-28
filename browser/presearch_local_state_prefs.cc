/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_local_state_prefs.h"

#include "base/values.h"
#include "presearch/browser/presearch_stats/presearch_stats_updater.h"
#include "presearch/browser/metrics/metrics_reporting_util.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_referrals/buildflags/buildflags.h"
#include "presearch/components/presearch_shields/browser/ad_block_service.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_p3a.h"
#include "presearch/components/decentralized_dns/buildflags/buildflags.h"
#include "presearch/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "presearch/components/ntp_background_images/browser/view_counter_service.h"
#include "presearch/components/p3a/presearch_p3a_service.h"
#include "presearch/components/p3a/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "chrome/common/pref_names.h"
#include "components/metrics/metrics_pref_names.h"
#include "components/prefs/pref_registry_simple.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
#include "presearch/components/presearch_referrals/browser/presearch_referrals_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/components/tor/tor_profile_service.h"
#endif

#include "presearch/browser/ui/webui/new_tab_page/presearch_new_tab_message_handler.h"

#if !defined(OS_ANDROID)
#include "presearch/browser/p3a/p3a_core_metrics.h"
#include "chrome/browser/first_run/first_run.h"
#endif  // !defined(OS_ANDROID)

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "presearch/browser/widevine/widevine_utils.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "presearch/components/decentralized_dns/decentralized_dns_service.h"
#endif

namespace presearch {

void RegisterLocalStatePrefsForMigration(PrefRegistrySimple* registry) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  RegisterWidevineLocalstatePrefsForMigration(registry);
#endif
}

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  presearch_shields::RegisterPrefsForAdBlockService(registry);
  presearch_stats::RegisterLocalStatePrefs(registry);
  ntp_background_images::NTPBackgroundImagesService::RegisterLocalStatePrefs(
      registry);
  ntp_background_images::ViewCounterService::RegisterLocalStatePrefs(registry);
#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
  RegisterPrefsForPresearchReferralsService(registry);
#endif
#if defined(OS_MAC)
  // Turn off super annoying 'Hold to quit'
  registry->SetDefaultPrefValue(prefs::kConfirmToQuitEnabled,
      base::Value(false));
#endif
#if BUILDFLAG(ENABLE_TOR)
  tor::TorProfileService::RegisterLocalStatePrefs(registry);
#endif
  registry->SetDefaultPrefValue(
      metrics::prefs::kMetricsReportingEnabled,
      base::Value(GetDefaultPrefValueForMetricsReporting()));

#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
  presearch::PresearchP3AService::RegisterPrefs(registry,
#if !defined(OS_ANDROID)
                                        first_run::IsChromeFirstRun());
#else
                                        // PresearchP3AService::RegisterPrefs
                                        // doesn't use this arg on Android
                                        false);
#endif  // !defined(OS_ANDROID)

#endif  // BUILDFLAG(PRESEARCH_P3A_ENABLED)

  presearch_shields::RegisterShieldsP3APrefs(registry);
#if !defined(OS_ANDROID)
  PresearchNewTabMessageHandler::RegisterLocalStatePrefs(registry);
  PresearchWindowTracker::RegisterPrefs(registry);
  PresearchUptimeTracker::RegisterPrefs(registry);
  dark_mode::RegisterPresearchDarkModeLocalStatePrefs(registry);

  registry->RegisterBooleanPref(kDefaultBrowserPromptEnabled, true);
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  RegisterWidevineLocalstatePrefs(registry);
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  decentralized_dns::DecentralizedDnsService::RegisterLocalStatePrefs(registry);
#endif

  RegisterLocalStatePrefsForMigration(registry);
}

}  // namespace presearch
