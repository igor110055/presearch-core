/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_BROWSER_PROCESS_IMPL_H_
#define PRESEARCH_BROWSER_PRESEARCH_BROWSER_PROCESS_IMPL_H_

#include <memory>

#include "base/memory/ref_counted.h"
#include "presearch/browser/presearch_browser_process.h"
#include "presearch/components/presearch_ads/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_component_updater/browser/presearch_component.h"
#include "presearch/components/presearch_referrals/buildflags/buildflags.h"
#include "presearch/components/greaselion/browser/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/speedreader/buildflags.h"
#include "presearch/components/tor/buildflags/buildflags.h"
#include "chrome/browser/browser_process_impl.h"
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

class PresearchBrowserProcessImpl : public PresearchBrowserProcess,
                                public BrowserProcessImpl {
 public:
  explicit PresearchBrowserProcessImpl(StartupData* startup_data);
  ~PresearchBrowserProcessImpl() override;

  // BrowserProcess implementation.

  ProfileManager* profile_manager() override;
  NotificationPlatformBridge* notification_platform_bridge() override;

  // PresearchBrowserProcess implementation.

  void StartPresearchServices() override;
  presearch_shields::AdBlockService* ad_block_service() override;
  presearch_shields::AdBlockCustomFiltersService* ad_block_custom_filters_service()
      override;
  presearch_shields::AdBlockRegionalServiceManager*
  ad_block_regional_service_manager() override;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  presearch_component_updater::ExtensionWhitelistService*
  extension_whitelist_service() override;
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionDownloadService* greaselion_download_service() override;
#endif
  presearch_shields::TrackingProtectionService* tracking_protection_service()
      override;
  presearch_shields::HTTPSEverywhereService* https_everywhere_service() override;
  presearch_component_updater::LocalDataFilesService* local_data_files_service()
      override;
#if BUILDFLAG(ENABLE_TOR)
  tor::PresearchTorClientUpdater* tor_client_updater() override;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ipfs::PresearchIpfsClientUpdater* ipfs_client_updater() override;
#endif
  presearch::PresearchP3AService* presearch_p3a_service() override;
  presearch::PresearchReferralsService* presearch_referrals_service() override;
  presearch_stats::PresearchStatsUpdater* presearch_stats_updater() override;
  ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() override;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderRewriterService* speedreader_rewriter_service()
      override;
#endif
#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
  presearch_ads::ResourceComponent* resource_component() override;
#endif

 private:
  // BrowserProcessImpl overrides:
  void Init() override;

  void CreateProfileManager();
  void CreateNotificationPlatformBridge();

#if BUILDFLAG(ENABLE_TOR)
  void OnTorEnabledChanged();
#endif

  void UpdatePresearchDarkMode();
  void OnPresearchDarkModeChanged();

  presearch_component_updater::PresearchComponent::Delegate*
  presearch_component_updater_delegate();

  // local_data_files_service_ should always be first because it needs
  // to be destroyed last
  std::unique_ptr<presearch_component_updater::LocalDataFilesService>
      local_data_files_service_;
  std::unique_ptr<presearch_component_updater::PresearchComponent::Delegate>
      presearch_component_updater_delegate_;
  std::unique_ptr<presearch_shields::AdBlockService> ad_block_service_;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  std::unique_ptr<presearch_component_updater::ExtensionWhitelistService>
      extension_whitelist_service_;
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  std::unique_ptr<greaselion::GreaselionDownloadService>
      greaselion_download_service_;
#endif
  std::unique_ptr<presearch_shields::TrackingProtectionService>
      tracking_protection_service_;
  std::unique_ptr<presearch_shields::HTTPSEverywhereService>
      https_everywhere_service_;
  std::unique_ptr<presearch_stats::PresearchStatsUpdater> presearch_stats_updater_;
#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
  std::unique_ptr<presearch::PresearchReferralsService> presearch_referrals_service_;
#endif
#if BUILDFLAG(ENABLE_TOR)
  std::unique_ptr<tor::PresearchTorClientUpdater> tor_client_updater_;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  std::unique_ptr<ipfs::PresearchIpfsClientUpdater> ipfs_client_updater_;
#endif
  scoped_refptr<presearch::PresearchP3AService> presearch_p3a_service_;
  std::unique_ptr<ntp_background_images::NTPBackgroundImagesService>
      ntp_background_images_service_;

#if BUILDFLAG(ENABLE_SPEEDREADER)
  std::unique_ptr<speedreader::SpeedreaderRewriterService>
      speedreader_rewriter_service_;
#endif

#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
  std::unique_ptr<presearch_ads::ResourceComponent> resource_component_;
#endif

  SEQUENCE_CHECKER(sequence_checker_);

  DISALLOW_COPY_AND_ASSIGN(PresearchBrowserProcessImpl);
};

#endif  // PRESEARCH_BROWSER_PRESEARCH_BROWSER_PROCESS_IMPL_H_
