/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_component_loader.h"

#include <string>

#include "base/bind.h"
#include "base/command_line.h"
#include "bat/ads/pref_names.h"
#include "presearch/browser/component_updater/presearch_component_installer.h"
#include "presearch/common/presearch_switches.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_ads/common/pref_names.h"
#include "presearch/components/presearch_component_updater/browser/presearch_on_demand_updater.h"
#include "presearch/components/presearch_extension/grit/presearch_extension.h"
#include "presearch/components/presearch_rewards/browser/buildflags/buildflags.h"
#include "presearch/components/presearch_rewards/common/pref_names.h"
#include "presearch/components/presearch_rewards/resources/extension/grit/presearch_rewards_extension_resources.h"
#include "presearch/components/presearch_webtorrent/grit/presearch_webtorrent_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/grit/presearch_components_resources.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/browser/extensions/presearch_wallet_util.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#include "presearch/components/presearch_wallet/browser/pref_names.h"
#endif

namespace extensions {

PresearchComponentLoader::PresearchComponentLoader(ExtensionSystem* extension_system,
                                           Profile* profile)
    : ComponentLoader(extension_system, profile),
      profile_(profile),
      profile_prefs_(profile->GetPrefs()) {
#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
  pref_change_registrar_.Init(profile_prefs_);
  pref_change_registrar_.Add(
      presearch_rewards::prefs::kAutoContributeEnabled,
      base::Bind(&PresearchComponentLoader::CheckRewardsStatus,
                 base::Unretained(this)));
#endif
}

PresearchComponentLoader::~PresearchComponentLoader() {}

void PresearchComponentLoader::OnComponentRegistered(std::string extension_id) {
  presearch_component_updater::PresearchOnDemandUpdater::GetInstance()->OnDemandUpdate(
      extension_id);
}

void PresearchComponentLoader::OnComponentReady(std::string extension_id,
                                            bool allow_file_access,
                                            const base::FilePath& install_dir,
                                            const std::string& manifest) {
  Add(manifest, install_dir);
  if (allow_file_access) {
    ExtensionPrefs::Get(profile_)->SetAllowFileAccess(extension_id, true);
  }
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  if (extension_id == ethereum_remote_client_extension_id) {
    ReinstallAsNonComponent(ethereum_remote_client_extension_id);
  }
#endif
}

void PresearchComponentLoader::ReinstallAsNonComponent(
    const std::string extension_id) {
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile_);
  const Extension* extension = registry->GetInstalledExtension(extension_id);
  DCHECK(extension);
  if (extension->location() == Manifest::COMPONENT) {
    service->RemoveComponentExtension(extension_id);
    std::string error;
    scoped_refptr<Extension> normal_extension = Extension::Create(
        extension->path(), Manifest::EXTERNAL_PREF,
        *extension->manifest()->value(), extension->creation_flags(), &error);
    service->AddExtension(normal_extension.get());
  }
}

void PresearchComponentLoader::AddExtension(const std::string& extension_id,
                                        const std::string& name,
                                        const std::string& public_key) {
  presearch::RegisterComponent(
      g_browser_process->component_updater(), name, public_key,
      base::Bind(&PresearchComponentLoader::OnComponentRegistered,
                 base::Unretained(this), extension_id),
      base::Bind(&PresearchComponentLoader::OnComponentReady,
                 base::Unretained(this), extension_id, true));
}

void PresearchComponentLoader::AddHangoutServicesExtension() {
  if (!profile_prefs_->FindPreference(kHangoutsEnabled) ||
      profile_prefs_->GetBoolean(kHangoutsEnabled)) {
    ForceAddHangoutServicesExtension();
  }
}

void PresearchComponentLoader::ForceAddHangoutServicesExtension() {
  ComponentLoader::AddHangoutServicesExtension();
}

void PresearchComponentLoader::AddDefaultComponentExtensions(
    bool skip_session_components) {
  ComponentLoader::AddDefaultComponentExtensions(skip_session_components);

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisablePresearchExtension)) {
    base::FilePath presearch_extension_path(FILE_PATH_LITERAL(""));
    presearch_extension_path =
        presearch_extension_path.Append(FILE_PATH_LITERAL("presearch_extension"));
    Add(IDR_PRESEARCH_EXTENSION, presearch_extension_path);
  }

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
  // Enable rewards extension if already opted-in
  CheckRewardsStatus();
#endif
}

#if BUILDFLAG(PRESEARCH_REWARDS_ENABLED)
void PresearchComponentLoader::AddRewardsExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisablePresearchRewardsExtension) &&
      !Exists(presearch_rewards_extension_id)) {
    base::FilePath presearch_rewards_path(FILE_PATH_LITERAL(""));
    presearch_rewards_path =
        presearch_rewards_path.Append(FILE_PATH_LITERAL("presearch_rewards"));
    Add(IDR_PRESEARCH_REWARDS, presearch_rewards_path);
  }
}

void PresearchComponentLoader::CheckRewardsStatus() {
  const bool is_ac_enabled =
      profile_prefs_->GetBoolean(presearch_rewards::prefs::kAutoContributeEnabled);

  if (is_ac_enabled) {
    AddRewardsExtension();
  }
}
#endif

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
void PresearchComponentLoader::AddEthereumRemoteClientExtension() {
  AddExtension(ethereum_remote_client_extension_id,
               ethereum_remote_client_extension_name,
               ethereum_remote_client_extension_public_key);
}

void PresearchComponentLoader::AddEthereumRemoteClientExtensionOnStartup() {
  // Only load if the eagerly load Crypto Wallets setting is on and there is a
  // project id configured in the build.
  if (HasInfuraProjectID() &&
      profile_prefs_->GetBoolean(kLoadCryptoWalletsOnStartup)) {
    AddEthereumRemoteClientExtension();
  }
}
#endif

void PresearchComponentLoader::AddWebTorrentExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableWebTorrentExtension) &&
      (!profile_prefs_->FindPreference(kWebTorrentEnabled) ||
       profile_prefs_->GetBoolean(kWebTorrentEnabled))) {
    base::FilePath presearch_webtorrent_path(FILE_PATH_LITERAL(""));
    presearch_webtorrent_path =
        presearch_webtorrent_path.Append(FILE_PATH_LITERAL("presearch_webtorrent"));
    Add(IDR_PRESEARCH_WEBTORRENT, presearch_webtorrent_path);
  }
}

}  // namespace extensions
