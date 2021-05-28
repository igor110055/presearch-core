/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_tab_helpers.h"

#include "base/command_line.h"
#include "base/feature_list.h"
#include "presearch/browser/presearch_ads/ads_tab_helper.h"
#include "presearch/browser/presearch_stats/presearch_stats_tab_helper.h"
#include "presearch/browser/ephemeral_storage/ephemeral_storage_tab_helper.h"
#include "presearch/browser/ui/bookmark/presearch_bookmark_tab_helper.h"
#include "presearch/components/presearch_perf_predictor/browser/buildflags.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_web_contents_observer.h"
#include "presearch/components/presearch_shields/browser/buildflags/buildflags.h"  // For STP
#include "presearch/components/presearch_wayback_machine/buildflags.h"
#include "presearch/components/greaselion/browser/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/speedreader/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "net/base/features.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_GREASELION)
#include "presearch/browser/greaselion/greaselion_tab_helper.h"
#endif

#if defined(OS_ANDROID)
#include "presearch/browser/android/preferences/background_video_playback_tab_helper.h"
#include "presearch/browser/android/preferences/website/desktop_mode_tab_helper.h"
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
#include "presearch/browser/presearch_rewards/rewards_tab_helper.h"
#endif

#if BUILDFLAG(PRESEARCH_STP_ENABLED)
#include "presearch/components/presearch_shields/browser/tracking_protection_helper.h"
#include "presearch/components/presearch_shields/browser/tracking_protection_service.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "presearch/browser/presearch_drm_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_WAYBACK_MACHINE)
#include "presearch/browser/infobars/presearch_wayback_machine_delegate_impl.h"
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_PERF_PREDICTOR)
#include "presearch/components/presearch_perf_predictor/browser/perf_predictor_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "presearch/browser/speedreader/speedreader_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/components/tor/onion_location_tab_helper.h"
#include "presearch/components/tor/tor_tab_helper.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/browser/ipfs/ipfs_service_factory.h"
#include "presearch/browser/ipfs/ipfs_tab_helper.h"
#endif

namespace presearch {

void AttachTabHelpers(content::WebContents* web_contents) {
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionTabHelper::CreateForWebContents(web_contents);
#endif
  presearch_shields::PresearchShieldsWebContentsObserver::CreateForWebContents(
      web_contents);

#if defined(OS_ANDROID)
  DesktopModeTabHelper::CreateForWebContents(web_contents);
  BackgroundVideoPlaybackTabHelper::CreateForWebContents(web_contents);
#else
  // Add tab helpers here unless they are intended for android too
  PresearchBookmarkTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
  presearch_rewards::RewardsTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(PRESEARCH_STP_ENABLED)
  if (presearch_shields::TrackingProtectionService::
          IsSmartTrackingProtectionEnabled()) {
    presearch_shields::TrackingProtectionHelper::CreateForWebContents(web_contents);
  }
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  PresearchDrmTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_WAYBACK_MACHINE)
  PresearchWaybackMachineDelegateImpl::AttachTabHelperIfNeeded(web_contents);
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_PERF_PREDICTOR)
  presearch_perf_predictor::PerfPredictorTabHelper::CreateForWebContents(
      web_contents);
#endif

  presearch_ads::AdsTabHelper::CreateForWebContents(web_contents);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_TOR)
  tor::TorTabHelper::MaybeCreateForWebContents(
      web_contents, web_contents->GetBrowserContext()->IsTor());
  tor::OnionLocationTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IPFSTabHelper::MaybeCreateForWebContents(web_contents);
#endif

  presearch_stats::PresearchStatsTabHelper::CreateForWebContents(web_contents);

  if (base::FeatureList::IsEnabled(net::features::kPresearchEphemeralStorage)) {
    ephemeral_storage::EphemeralStorageTabHelper::CreateForWebContents(
        web_contents);
  }
}

}  // namespace presearch
