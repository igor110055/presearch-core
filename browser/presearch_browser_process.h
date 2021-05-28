/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This interface is for managing the global services of the application. Each
// service is lazily created when requested the first time. The service getters
// will return NULL if the service is not available, so callers must check for
// this condition.

#ifndef PRESEARCH_BROWSER_PRESEARCH_BROWSER_PROCESS_H_
#define PRESEARCH_BROWSER_PRESEARCH_BROWSER_PROCESS_H_

#include "presearch/components/presearch_ads/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_component_updater/browser/presearch_component.h"
#include "presearch/components/presearch_referrals/buildflags/buildflags.h"
#include "presearch/components/greaselion/browser/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/speedreader/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"

namespace presearch {
class PresearchReferralsService;
class PresearchP3AService;
}  // namespace presearch

namespace presearch_component_updater {
#if BUILDFLAG(ENABLE_EXTENSIONS)
class ExtensionWhitelistService;
#endif
class LocalDataFilesService;
}  // namespace presearch_component_updater

namespace presearch_shields {
class AdBlockService;
class AdBlockCustomFiltersService;
class AdBlockRegionalServiceManager;
class HTTPSEverywhereService;
class TrackingProtectionService;
}  // namespace presearch_shields

namespace presearch_stats {
class PresearchStatsUpdater;
}  // namespace presearch_stats

namespace greaselion {
#if BUILDFLAG(ENABLE_GREASELION)
class GreaselionDownloadService;
#endif
}  // namespace greaselion

namespace ntp_background_images {
class NTPBackgroundImagesService;
}  // namespace ntp_background_images

namespace tor {
class PresearchTorClientUpdater;
}

namespace ipfs {
class PresearchIpfsClientUpdater;
}

namespace speedreader {
class SpeedreaderRewriterService;
}

namespace presearch_ads {
class ResourceComponent;
}

class PresearchBrowserProcess {
 public:
  virtual void StartPresearchServices() = 0;
  virtual presearch_shields::AdBlockService* ad_block_service() = 0;
  virtual presearch_shields::AdBlockCustomFiltersService*
  ad_block_custom_filters_service() = 0;
  virtual presearch_shields::AdBlockRegionalServiceManager*
  ad_block_regional_service_manager() = 0;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  virtual presearch_component_updater::ExtensionWhitelistService*
  extension_whitelist_service() = 0;
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  virtual greaselion::GreaselionDownloadService*
  greaselion_download_service() = 0;
#endif
  virtual presearch_shields::TrackingProtectionService*
  tracking_protection_service() = 0;
  virtual presearch_shields::HTTPSEverywhereService* https_everywhere_service() = 0;
  virtual presearch_component_updater::LocalDataFilesService*
  local_data_files_service() = 0;
#if BUILDFLAG(ENABLE_TOR)
  virtual tor::PresearchTorClientUpdater* tor_client_updater() = 0;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  virtual ipfs::PresearchIpfsClientUpdater* ipfs_client_updater() = 0;
#endif
  virtual presearch::PresearchP3AService* presearch_p3a_service() = 0;
  virtual presearch::PresearchReferralsService* presearch_referrals_service() = 0;
  virtual presearch_stats::PresearchStatsUpdater* presearch_stats_updater() = 0;
  virtual ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() = 0;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  virtual speedreader::SpeedreaderRewriterService*
  speedreader_rewriter_service() = 0;
#endif
#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
  virtual presearch_ads::ResourceComponent* resource_component() = 0;
#endif
};

extern PresearchBrowserProcess* g_presearch_browser_process;

#endif  // PRESEARCH_BROWSER_PRESEARCH_BROWSER_PROCESS_H_
