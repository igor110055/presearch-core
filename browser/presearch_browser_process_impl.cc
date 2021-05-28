/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_browser_process_impl.h"

#include <utility>

#include "base/bind.h"
#include "base/path_service.h"
#include "base/task/post_task.h"
#include "presearch/browser/presearch_stats/presearch_stats_updater.h"
#include "presearch/browser/component_updater/presearch_component_updater_configurator.h"
#include "presearch/browser/component_updater/presearch_component_updater_delegate.h"
#include "presearch/browser/net/presearch_system_request_handler.h"
#include "presearch/browser/profiles/presearch_profile_manager.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/browser/ui/presearch_browser_command_controller.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_ads/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_component_updater/browser/presearch_on_demand_updater.h"
#include "presearch/components/presearch_component_updater/browser/local_data_files_service.h"
#include "presearch/components/presearch_referrals/buildflags/buildflags.h"
#include "presearch/components/presearch_shields/browser/ad_block_custom_filters_service.h"
#include "presearch/components/presearch_shields/browser/ad_block_regional_service_manager.h"
#include "presearch/components/presearch_shields/browser/ad_block_service.h"
#include "presearch/components/presearch_shields/browser/https_everywhere_service.h"
#include "presearch/components/presearch_shields/browser/tracking_protection_service.h"
#include "presearch/components/presearch_sync/buildflags/buildflags.h"
#include "presearch/components/presearch_sync/network_time_helper.h"
#include "presearch/components/ntp_background_images/browser/features.h"
#include "presearch/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "presearch/components/p3a/presearch_histogram_rewrite.h"
#include "presearch/components/p3a/presearch_p3a_service.h"
#include "presearch/components/p3a/buildflags.h"
#include "presearch/services/network/public/cpp/system_request_handler.h"
#include "chrome/browser/component_updater/component_updater_utils.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/common/buildflags.h"
#include "chrome/common/chrome_paths.h"
#include "components/component_updater/component_updater_service.h"
#include "components/component_updater/timer_update_scheduler.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/child_process_security_policy.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

#if BUILDFLAG(ENABLE_SYSTEM_NOTIFICATIONS)
#include "chrome/browser/notifications/notification_platform_bridge.h"
#include "presearch/browser/notifications/presearch_notification_platform_bridge.h"
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
#include "presearch/components/presearch_referrals/browser/presearch_referrals_service.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "presearch/common/extensions/whitelist.h"
#include "presearch/components/presearch_component_updater/browser/extension_whitelist_service.h"
#endif

#if BUILDFLAG(ENABLE_GREASELION)
#include "presearch/components/greaselion/browser/greaselion_download_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/components/tor/presearch_tor_client_updater.h"
#include "presearch/components/tor/pref_names.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/presearch_ipfs_client_updater.h"
#include "presearch/components/ipfs/ipfs_constants.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "presearch/components/speedreader/speedreader_rewriter_service.h"
#endif

#if defined(OS_ANDROID)
#include "chrome/browser/flags/android/chrome_feature_list.h"
#else
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#endif

#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
#include "presearch/components/presearch_ads/browser/component_updater/resource_component.h"
#endif

using presearch_component_updater::PresearchComponent;
using ntp_background_images::features::kPresearchNTPBrandedWallpaper;
using ntp_background_images::NTPBackgroundImagesService;

namespace {

// Initializes callback for SystemRequestHandler
void InitSystemRequestHandlerCallback() {
  network::SystemRequestHandler::OnBeforeSystemRequestCallback
      before_system_request_callback = base::Bind(presearch::OnBeforeSystemRequest);
  network::SystemRequestHandler::GetInstance()
      ->RegisterOnBeforeSystemRequestCallback(before_system_request_callback);
}

}  // namespace

PresearchBrowserProcess* g_presearch_browser_process = nullptr;

using content::BrowserThread;

PresearchBrowserProcessImpl::~PresearchBrowserProcessImpl() {}

PresearchBrowserProcessImpl::PresearchBrowserProcessImpl(StartupData* startup_data)
    : BrowserProcessImpl(startup_data) {
  g_browser_process = this;
  g_presearch_browser_process = this;

#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
  // early initialize referrals
  presearch_referrals_service();
#endif
  // early initialize presearch stats
  presearch_stats_updater();

  // Disabled on mobile platforms, see for instance issues/6176
#if BUILDFLAG(PRESEARCH_P3A_ENABLED)
  // Create P3A Service early to catch more histograms. The full initialization
  // should be started once browser process impl is ready.
  presearch_p3a_service();
  presearch::SetupHistogramsPresearchization();
#endif  // BUILDFLAG(PRESEARCH_P3A_ENABLED)
}

void PresearchBrowserProcessImpl::Init() {
  BrowserProcessImpl::Init();
#if BUILDFLAG(IPFS_ENABLED)
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      ipfs::kIPFSScheme);
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      ipfs::kIPNSScheme);
#endif
  presearch_component_updater::PresearchOnDemandUpdater::GetInstance()->
      RegisterOnDemandUpdateCallback(
          base::BindRepeating(&component_updater::PresearchOnDemandUpdate));
  UpdatePresearchDarkMode();
  pref_change_registrar_.Add(
      kPresearchDarkMode,
      base::Bind(&PresearchBrowserProcessImpl::OnPresearchDarkModeChanged,
                 base::Unretained(this)));

#if BUILDFLAG(ENABLE_TOR)
  pref_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::Bind(&PresearchBrowserProcessImpl::OnTorEnabledChanged,
                 base::Unretained(this)));
#endif

  InitSystemRequestHandlerCallback();
}

presearch_component_updater::PresearchComponent::Delegate*
PresearchBrowserProcessImpl::presearch_component_updater_delegate() {
  if (!presearch_component_updater_delegate_)
    presearch_component_updater_delegate_ =
        std::make_unique<presearch::PresearchComponentUpdaterDelegate>();

  return presearch_component_updater_delegate_.get();
}

ProfileManager* PresearchBrowserProcessImpl::profile_manager() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!created_profile_manager_)
    CreateProfileManager();
  return profile_manager_.get();
}

void PresearchBrowserProcessImpl::StartPresearchServices() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  ad_block_service()->Start();
  https_everywhere_service()->Start();

#if BUILDFLAG(ENABLE_EXTENSIONS)
  extension_whitelist_service();
#endif
  tracking_protection_service();
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion_download_service();
#endif
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader_rewriter_service();
#endif
#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
  resource_component();
#endif
  // Now start the local data files service, which calls all observers.
  local_data_files_service()->Start();

#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
  presearch_sync::NetworkTimeHelper::GetInstance()
    ->SetNetworkTimeTracker(g_browser_process->network_time_tracker());
#endif
}

presearch_shields::AdBlockService* PresearchBrowserProcessImpl::ad_block_service() {
  if (ad_block_service_)
    return ad_block_service_.get();

  ad_block_service_ =
      presearch_shields::AdBlockServiceFactory(presearch_component_updater_delegate());
  return ad_block_service_.get();
}

presearch_shields::AdBlockCustomFiltersService*
PresearchBrowserProcessImpl::ad_block_custom_filters_service() {
  return ad_block_service()->custom_filters_service();
}

presearch_shields::AdBlockRegionalServiceManager*
PresearchBrowserProcessImpl::ad_block_regional_service_manager() {
  return ad_block_service()->regional_service_manager();
}

NTPBackgroundImagesService*
PresearchBrowserProcessImpl::ntp_background_images_service() {
  if (!base::FeatureList::IsEnabled(kPresearchNTPBrandedWallpaper))
    return nullptr;

  if (!ntp_background_images_service_) {
    ntp_background_images_service_ =
        std::make_unique<NTPBackgroundImagesService>(component_updater(),
                                                     local_state());
    ntp_background_images_service_->Init();
  }

  return ntp_background_images_service_.get();
}

#if BUILDFLAG(ENABLE_EXTENSIONS)
presearch_component_updater::ExtensionWhitelistService*
PresearchBrowserProcessImpl::extension_whitelist_service() {
  if (!extension_whitelist_service_) {
    extension_whitelist_service_ =
        presearch_component_updater::ExtensionWhitelistServiceFactory(
            local_data_files_service(), kVettedExtensions);
  }
  return extension_whitelist_service_.get();
}
#endif

#if BUILDFLAG(ENABLE_GREASELION)
greaselion::GreaselionDownloadService*
PresearchBrowserProcessImpl::greaselion_download_service() {
  if (!greaselion_download_service_) {
    greaselion_download_service_ = greaselion::GreaselionDownloadServiceFactory(
        local_data_files_service());
  }
  return greaselion_download_service_.get();
}
#endif

presearch_shields::TrackingProtectionService*
PresearchBrowserProcessImpl::tracking_protection_service() {
  if (!tracking_protection_service_) {
    tracking_protection_service_ =
        presearch_shields::TrackingProtectionServiceFactory(
            local_data_files_service());
  }
  return tracking_protection_service_.get();
}

presearch_shields::HTTPSEverywhereService*
PresearchBrowserProcessImpl::https_everywhere_service() {
  if (!https_everywhere_service_)
    https_everywhere_service_ = presearch_shields::HTTPSEverywhereServiceFactory(
        presearch_component_updater_delegate());
  return https_everywhere_service_.get();
}

presearch_component_updater::LocalDataFilesService*
PresearchBrowserProcessImpl::local_data_files_service() {
  if (!local_data_files_service_)
    local_data_files_service_ =
        presearch_component_updater::LocalDataFilesServiceFactory(
            presearch_component_updater_delegate());
  return local_data_files_service_.get();
}

void PresearchBrowserProcessImpl::UpdatePresearchDarkMode() {
  // Update with proper system theme to make presearch theme and base ui components
  // theme use same theme.
  dark_mode::SetSystemDarkMode(dark_mode::GetPresearchDarkModeType());
}

void PresearchBrowserProcessImpl::OnPresearchDarkModeChanged() {
  UpdatePresearchDarkMode();
}

#if BUILDFLAG(ENABLE_TOR)
tor::PresearchTorClientUpdater*
PresearchBrowserProcessImpl::tor_client_updater() {
  if (tor_client_updater_)
    return tor_client_updater_.get();

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

  tor_client_updater_.reset(new tor::PresearchTorClientUpdater(
      presearch_component_updater_delegate(), local_state(), user_data_dir));
  return tor_client_updater_.get();
}

void PresearchBrowserProcessImpl::OnTorEnabledChanged() {
  // Update all browsers' tor command status.
  for (Browser* browser : *BrowserList::GetInstance()) {
    static_cast<chrome::PresearchBrowserCommandController*>(
        browser->command_controller())->UpdateCommandForTor();
  }
}
#endif

presearch::PresearchP3AService* PresearchBrowserProcessImpl::presearch_p3a_service() {
  if (presearch_p3a_service_) {
    return presearch_p3a_service_.get();
  }
  presearch_p3a_service_ = new presearch::PresearchP3AService(local_state());
  presearch_p3a_service()->InitCallbacks();
  return presearch_p3a_service_.get();
}

presearch::PresearchReferralsService*
PresearchBrowserProcessImpl::presearch_referrals_service() {
  if (!presearch_referrals_service_)
    presearch_referrals_service_ = std::make_unique<presearch::PresearchReferralsService>(
        local_state(), presearch_stats::GetAPIKey(),
        presearch_stats::GetPlatformIdentifier());
  return presearch_referrals_service_.get();
}

presearch_stats::PresearchStatsUpdater* PresearchBrowserProcessImpl::presearch_stats_updater() {
  if (!presearch_stats_updater_)
    presearch_stats_updater_ =
        std::make_unique<presearch_stats::PresearchStatsUpdater>(local_state());
  return presearch_stats_updater_.get();
}

void PresearchBrowserProcessImpl::CreateProfileManager() {
  DCHECK(!created_profile_manager_ && !profile_manager_);
  created_profile_manager_ = true;

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  profile_manager_ = std::make_unique<PresearchProfileManager>(user_data_dir);
}

NotificationPlatformBridge*
PresearchBrowserProcessImpl::notification_platform_bridge() {
#if !defined(OS_MAC)
  return BrowserProcessImpl::notification_platform_bridge();
#else
#if BUILDFLAG(ENABLE_SYSTEM_NOTIFICATIONS)
  if (!created_notification_bridge_)
    CreateNotificationPlatformBridge();
  return notification_bridge_.get();
#else
  return nullptr;
#endif
#endif
}

void PresearchBrowserProcessImpl::CreateNotificationPlatformBridge() {
#if defined(OS_MAC)
#if BUILDFLAG(ENABLE_SYSTEM_NOTIFICATIONS)
  DCHECK(!notification_bridge_);
  notification_bridge_ = PresearchNotificationPlatformBridge::Create();
  created_notification_bridge_ = true;
#endif
#endif
}

#if BUILDFLAG(ENABLE_SPEEDREADER)
speedreader::SpeedreaderRewriterService*
PresearchBrowserProcessImpl::speedreader_rewriter_service() {
  if (!speedreader_rewriter_service_) {
    speedreader_rewriter_service_.reset(
        new speedreader::SpeedreaderRewriterService(
            presearch_component_updater_delegate()));
  }
  return speedreader_rewriter_service_.get();
}
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

#if BUILDFLAG(PRESEARCH_ADS_ENABLED)
presearch_ads::ResourceComponent* PresearchBrowserProcessImpl::resource_component() {
  if (!resource_component_) {
    resource_component_.reset(
        new presearch_ads::ResourceComponent(presearch_component_updater_delegate()));
  }
  return resource_component_.get();
}

#endif  // BUILDFLAG(PRESEARCH_ADS_ENABLED)

#if BUILDFLAG(IPFS_ENABLED)
ipfs::PresearchIpfsClientUpdater*
PresearchBrowserProcessImpl::ipfs_client_updater() {
  if (ipfs_client_updater_)
    return ipfs_client_updater_.get();

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

  ipfs_client_updater_ = ipfs::PresearchIpfsClientUpdaterFactory(
      presearch_component_updater_delegate(), user_data_dir);
  return ipfs_client_updater_.get();
}
#endif  // BUILDFLAG(IPFS_ENABLED)
