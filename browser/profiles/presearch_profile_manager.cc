// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/browser/profiles/presearch_profile_manager.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/metrics/histogram_macros.h"
#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/browser/presearch_rewards/rewards_service_factory.h"
#include "presearch/browser/profiles/profile_util.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/content_settings/core/browser/presearch_content_settings_pref_provider.h"
#include "presearch/components/decentralized_dns/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/tor/tor_constants.h"
#include "presearch/content/browser/webui/presearch_shared_resources_data_source.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/profiles/profile_attributes_entry.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profiles_state.h"
#include "chrome/common/chrome_constants.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/generated_resources.h"
#include "components/bookmarks/common/bookmark_pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"
#include "components/signin/public/base/signin_pref_names.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/url_data_source.h"
#include "ui/base/l10n/l10n_util.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/browser/presearch_wallet/presearch_wallet_service_factory.h"
#endif

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "presearch/browser/gcm_driver/presearch_gcm_channel_status.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/browser/ipfs/ipfs_service_factory.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "presearch/browser/decentralized_dns/decentralized_dns_service_factory.h"
#endif

using content::BrowserThread;

namespace {

void AddPresearchSharedResourcesDataSourceToProfile(Profile* profile) {
  content::URLDataSource::Add(
      profile,
      std::make_unique<presearch_content::PresearchSharedResourcesDataSource>());
}

}  // namespace

PresearchProfileManager::PresearchProfileManager(const base::FilePath& user_data_dir)
    : ProfileManager(user_data_dir) {
  MigrateProfileNames();

  AddObserver(this);
}

PresearchProfileManager::~PresearchProfileManager() {
  std::vector<Profile*> profiles = GetLoadedProfiles();
  for (Profile* profile : profiles) {
    if (presearch::IsSessionProfile(profile)) {
      // passing false for `success` removes the profile from the info cache
      OnProfileCreated(profile, false, false);
    }
  }
  RemoveObserver(this);
}

void PresearchProfileManager::InitProfileUserPrefs(Profile* profile) {
  // migrate obsolete plugin prefs to temporary migration pref because otherwise
  // they get deleteed by PrefProvider before we can migrate them in
  // PresearchPrefProvider
  content_settings::PresearchPrefProvider::CopyPluginSettingsForMigration(
      profile->GetPrefs());

  ProfileManager::InitProfileUserPrefs(profile);
  presearch::RecordInitialP3AValues(profile);
  presearch::SetDefaultSearchVersion(profile, profile->IsNewProfile());
}

std::string PresearchProfileManager::GetLastUsedProfileName() {
  PrefService* local_state = g_browser_process->local_state();
  DCHECK(local_state);
  const std::string last_used_profile_name =
      local_state->GetString(prefs::kProfileLastUsed);
  // Keep this for legacy tor profile migration because tor profile might be
  // last active profile before upgrading
  if (last_used_profile_name ==
      base::FilePath(tor::kTorProfileDir).AsUTF8Unsafe())
    return chrome::kInitialProfile;
  return ProfileManager::GetLastUsedProfileName();
}

void PresearchProfileManager::DoFinalInitForServices(Profile* profile,
                                                 bool go_off_the_record) {
  ProfileManager::DoFinalInitForServices(profile, go_off_the_record);
  if (!do_final_services_init_)
    return;
  presearch_ads::AdsServiceFactory::GetForProfile(profile);
  presearch_rewards::RewardsServiceFactory::GetForProfile(profile);
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  PresearchWalletServiceFactory::GetForContext(profile);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IpfsServiceFactory::GetForContext(profile);
#endif
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  decentralized_dns::DecentralizedDnsServiceFactory::GetForContext(profile);
#endif
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  gcm::PresearchGCMChannelStatus* status =
      gcm::PresearchGCMChannelStatus::GetForProfile(profile);
  DCHECK(status);
  status->UpdateGCMDriverStatus();
#endif
}

bool PresearchProfileManager::IsAllowedProfilePath(
    const base::FilePath& path) const {
  // Chromium only allows profiles to be created in the user_data_dir, but we
  // want to also be able to create profile in subfolders of user_data_dir.
  return ProfileManager::IsAllowedProfilePath(path) ||
         user_data_dir().IsParent(path.DirName());
}

bool PresearchProfileManager::LoadProfileByPath(const base::FilePath& profile_path,
                         bool incognito,
                         ProfileLoadedCallback callback) {
  // Prevent legacy tor session profile to be loaded so we won't hit
  // DCHECK(!GetProfileAttributesWithPath(...)). Workaround for legacy tor guest
  // profile won't work because when AddProfile to storage we will hit
  // DCHECK(user_data_dir_ == profile_path.DirName()), legacy tor session
  // profile was not under user_data_dir like legacy tor guest profile did.
  if (profile_path.BaseName().value() == tor::kTorProfileDir) {
    return false;
  }

  return ProfileManager::LoadProfileByPath(profile_path, incognito,
                                           std::move(callback));
}

// This overridden method doesn't clear |kDefaultSearchProviderDataPrefName|.
// W/o this, prefs set by TorWindowSearchEngineProviderService is cleared
// during the initialization.
void PresearchProfileManager::SetNonPersonalProfilePrefs(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  prefs->SetBoolean(prefs::kSigninAllowed, false);
  prefs->SetBoolean(bookmarks::prefs::kEditBookmarksEnabled, false);
  prefs->SetBoolean(bookmarks::prefs::kShowBookmarkBar, false);
}

void PresearchProfileManager::MigrateProfileNames() {
#if !defined(OS_ANDROID) && !defined(OS_CHROMEOS)
  // If any profiles have a default name using an
  // older version of the default name string format,
  // then name it with the new default name string format.
  // e.g. 'Person X' --> 'Profile X'.
  ProfileAttributesStorage& storage = GetProfileAttributesStorage();
  std::vector<ProfileAttributesEntry*> entries =
      storage.GetAllProfilesAttributesSortedByName();
  // Make sure we keep the numbering the same.
  for (auto* entry : entries) {
    // Rename the necessary profiles. Don't check for legacy names as profile
    // info cache should have migrated them by now.
    if (entry->IsUsingDefaultName() &&
        !storage.IsDefaultProfileName(
            entry->GetName(),
            /*include_check_for_legacy_profile_name=*/false)) {
      auto icon_index = entry->GetAvatarIconIndex();
      entry->SetLocalProfileName(storage.ChooseNameForNewProfile(icon_index),
                                 /*is_default_name=*/true);
    }
  }
#endif
}

void PresearchProfileManager::OnProfileAdded(Profile* profile) {
  // Observe new profiles for creation of OTR profiles so that we can add our
  // shared resources to them.
  observed_profiles_.Add(profile);
  AddPresearchSharedResourcesDataSourceToProfile(profile);
}

void PresearchProfileManager::OnOffTheRecordProfileCreated(
    Profile* off_the_record) {
  AddPresearchSharedResourcesDataSourceToProfile(off_the_record);
}

void PresearchProfileManager::OnProfileWillBeDestroyed(Profile* profile) {
  if (!profile->IsOffTheRecord()) {
    observed_profiles_.Remove(profile);
  }
}

PresearchProfileManagerWithoutInit::PresearchProfileManagerWithoutInit(
    const base::FilePath& user_data_dir)
    : PresearchProfileManager(user_data_dir) {
  set_do_final_services_init(false);
}
