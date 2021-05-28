/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/sync/driver/presearch_sync_profile_sync_service.h"

#include <utility>
#include <vector>

#include "base/logging.h"
#include "base/strings/string_util.h"
#include "presearch/components/presearch_sync/crypto/crypto.h"
#include "presearch/components/sync/driver/presearch_sync_auth_manager.h"
#include "presearch/components/sync/driver/profile_sync_service_delegate.h"
#include "components/prefs/pref_service.h"

namespace syncer {

PresearchProfileSyncService::PresearchProfileSyncService(
    InitParams init_params,
    std::unique_ptr<ProfileSyncServiceDelegate> profile_service_delegate)
    : ProfileSyncService(std::move(init_params)),
      presearch_sync_prefs_(sync_client_->GetPrefService()),
      profile_service_delegate_(std::move(profile_service_delegate)),
      weak_ptr_factory_(this) {
  presearch_sync_prefs_change_registrar_.Init(sync_client_->GetPrefService());
  presearch_sync_prefs_change_registrar_.Add(
      presearch_sync::Prefs::GetSeedPath(),
      base::Bind(&PresearchProfileSyncService::OnPresearchSyncPrefsChanged,
                 base::Unretained(this)));
  GetPresearchSyncAuthManager()->DeriveSigningKeys(presearch_sync_prefs_.GetSeed());

  profile_service_delegate_->set_profile_sync_service(this);
}

PresearchProfileSyncService::~PresearchProfileSyncService() {
  presearch_sync_prefs_change_registrar_.RemoveAll();
}

void PresearchProfileSyncService::Initialize() {
  ProfileSyncService::Initialize();
  if (!presearch_sync_prefs_.IsSyncV1Migrated()) {
    StopImpl(CLEAR_DATA);
    presearch_sync_prefs_.SetSyncV1Migrated(true);
  }
}

bool PresearchProfileSyncService::IsSetupInProgress() const {
  return ProfileSyncService::IsSetupInProgress() &&
         !user_settings_->IsFirstSetupComplete();
}

std::string PresearchProfileSyncService::GetOrCreateSyncCode() {
  std::string sync_code = presearch_sync_prefs_.GetSeed();
  if (sync_code.empty()) {
    std::vector<uint8_t> seed = presearch_sync::crypto::GetSeed();
    sync_code = presearch_sync::crypto::PassphraseFromBytes32(seed);
  }
  return sync_code;
}

bool PresearchProfileSyncService::SetSyncCode(const std::string& sync_code) {
  std::vector<uint8_t> seed;
  std::string sync_code_trimmed;
  base::TrimString(sync_code, " \n\t", &sync_code_trimmed);
  if (!presearch_sync::crypto::PassphraseToBytes32(sync_code_trimmed, &seed))
    return false;
  if (!presearch_sync_prefs_.SetSeed(sync_code_trimmed))
    return false;
  return true;
}

void PresearchProfileSyncService::OnSelfDeviceInfoDeleted(base::OnceClosure cb) {
  // This function will follow normal reset process and set SyncRequested to
  // false
  StopAndClear();
  presearch_sync_prefs_.Clear();
  // Sync prefs will be clear in ProfileSyncService::StopImpl
  std::move(cb).Run();
}

PresearchSyncAuthManager* PresearchProfileSyncService::GetPresearchSyncAuthManager() {
  return static_cast<PresearchSyncAuthManager*>(auth_manager_.get());
}

void PresearchProfileSyncService::OnPresearchSyncPrefsChanged(const std::string& path) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (path == presearch_sync::Prefs::GetSeedPath()) {
    const std::string seed = presearch_sync_prefs_.GetSeed();
    if (!seed.empty()) {
      GetPresearchSyncAuthManager()->DeriveSigningKeys(seed);
      // Default enabled types: Bookmarks
      syncer::UserSelectableTypeSet selected_types;
      selected_types.Put(UserSelectableType::kBookmarks);
      GetUserSettings()->SetSelectedTypes(false, selected_types);
    } else {
      VLOG(1) << "Presearch sync seed cleared";
      GetPresearchSyncAuthManager()->ResetKeys();
    }
  }
}

void PresearchProfileSyncService::SuspendDeviceObserverForOwnReset() {
  profile_service_delegate_->SuspendDeviceObserverForOwnReset();
}

void PresearchProfileSyncService::ResumeDeviceObserver() {
  profile_service_delegate_->ResumeDeviceObserver();
}

}  // namespace syncer
