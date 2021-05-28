/* Copyright (c) 2019 The Presearch Authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/presearch_settings_ui.h"

#include <string>

#include "presearch/browser/extensions/presearch_component_loader.h"
#include "presearch/browser/ntp_background_images/view_counter_service_factory.h"
#include "presearch/browser/resources/settings/grit/presearch_settings_resources.h"
#include "presearch/browser/resources/settings/grit/presearch_settings_resources_map.h"
#include "presearch/browser/sparkle_buildflags.h"
#include "presearch/browser/ui/webui/navigation_bar_data_provider.h"
#include "presearch/browser/ui/webui/settings/presearch_appearance_handler.h"
#include "presearch/browser/ui/webui/settings/presearch_default_extensions_handler.h"
#include "presearch/browser/ui/webui/settings/presearch_privacy_handler.h"
#include "presearch/browser/ui/webui/settings/presearch_sync_handler.h"
#include "presearch/browser/ui/webui/settings/default_presearch_shields_handler.h"
#include "presearch/browser/version_info.h"
#include "presearch/components/presearch_sync/buildflags/buildflags.h"
#include "presearch/components/ntp_background_images/browser/view_counter_service.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/settings/metrics_reporting_handler.h"
#include "content/public/browser/web_ui_data_source.h"

#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
#include "components/sync/driver/sync_driver_switches.h"
#endif

#if BUILDFLAG(ENABLE_SPARKLE)
#include "presearch/browser/ui/webui/settings/presearch_relaunch_handler_mac.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "presearch/browser/ui/sidebar/sidebar_utils.h"
#endif

using ntp_background_images::ViewCounterServiceFactory;

PresearchSettingsUI::PresearchSettingsUI(content::WebUI* web_ui,
                                 const std::string& host)
    : SettingsUI(web_ui) {
  web_ui->AddMessageHandler(
    std::make_unique<settings::MetricsReportingHandler>());
  web_ui->AddMessageHandler(std::make_unique<PresearchPrivacyHandler>());
  web_ui->AddMessageHandler(std::make_unique<DefaultPresearchShieldsHandler>());
  web_ui->AddMessageHandler(std::make_unique<PresearchDefaultExtensionsHandler>());
  web_ui->AddMessageHandler(std::make_unique<PresearchAppearanceHandler>());
  web_ui->AddMessageHandler(std::make_unique<PresearchSyncHandler>());
#if BUILDFLAG(ENABLE_SPARKLE)
  // Use sparkle's relaunch api for browser relaunch on update.
  web_ui->AddMessageHandler(std::make_unique<PresearchRelaunchHandler>());
#endif
}

PresearchSettingsUI::~PresearchSettingsUI() {
}

// static
void PresearchSettingsUI::AddResources(content::WebUIDataSource* html_source,
                                   Profile* profile) {
  for (size_t i = 0; i < kPresearchSettingsResourcesSize; ++i) {
    html_source->AddResourcePath(kPresearchSettingsResources[i].path,
                                 kPresearchSettingsResources[i].id);
  }

#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
  html_source->AddBoolean("isSyncDisabled",
                          !switches::IsSyncAllowedByFlag());
#else
  html_source->AddBoolean("isSyncDisabled", true);
#endif
  html_source->AddString("presearchProductVersion",
    version_info::GetPresearchVersionWithoutChromiumMajorVersion());
  NavigationBarDataProvider::Initialize(html_source);
  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile))
    service->InitializeWebUIDataSource(html_source);
#if BUILDFLAG(ENABLE_SIDEBAR)
  // TODO(simonhong): Remove this when sidebar is shipped by default in all
  // channels.
  html_source->AddBoolean("isSidebarFeatureEnabled",
                          sidebar::CanUseSidebar(profile));
#endif
}
