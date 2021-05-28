/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/ios/browser/api/sync/presearch_sync_worker.h"

#include <string>
#include <vector>

#include "base/json/json_writer.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observer.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "presearch/components/presearch_sync/presearch_sync_prefs.h"
#include "presearch/components/presearch_sync/crypto/crypto.h"
#include "presearch/components/presearch_sync/profile_sync_service_helper.h"
#include "presearch/components/sync/driver/presearch_sync_profile_sync_service.h"
#include "presearch/components/sync_device_info/presearch_device_info.h"
#include "components/sync/driver/profile_sync_service.h"
#include "components/sync/driver/sync_service.h"
#include "components/sync/driver/sync_service_observer.h"
#include "components/sync_device_info/device_info.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"
#include "ios/chrome/browser/sync/profile_sync_service_factory.h"
#include "ios/chrome/browser/sync/sync_setup_service.h"
#include "ios/chrome/browser/sync/sync_setup_service_factory.h"
#include "ios/web/public/thread/web_thread.h"

namespace {
static const size_t SEED_BYTES_COUNT = 32u;
}  // namespace

PresearchSyncDeviceTracker::PresearchSyncDeviceTracker(
    syncer::DeviceInfoTracker* device_info_tracker,
    std::function<void()> on_device_info_changed_callback)
    : on_device_info_changed_callback_(on_device_info_changed_callback) {
  DCHECK(device_info_tracker);
  device_info_tracker_observer_.Add(device_info_tracker);
}

PresearchSyncDeviceTracker::~PresearchSyncDeviceTracker() {
  // Observer will be removed by ScopedObserver
}

void PresearchSyncDeviceTracker::OnDeviceInfoChange() {
  if (on_device_info_changed_callback_) {
    on_device_info_changed_callback_();
  }
}

PresearchSyncServiceTracker::PresearchSyncServiceTracker(
    syncer::ProfileSyncService* profile_sync_service,
    std::function<void()> on_state_changed_callback)
    : on_state_changed_callback_(on_state_changed_callback) {
  DCHECK(profile_sync_service);
  sync_service_observer_.Add(profile_sync_service);
}

PresearchSyncServiceTracker::~PresearchSyncServiceTracker() {
  // Observer will be removed by ScopedObserver
}

void PresearchSyncServiceTracker::OnStateChanged(syncer::SyncService* sync) {
  if (on_state_changed_callback_) {
    on_state_changed_callback_();
  }
}

PresearchSyncWorker::PresearchSyncWorker(ChromeBrowserState* browser_state)
    : browser_state_(browser_state) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
}

PresearchSyncWorker::~PresearchSyncWorker() {
  // Observer will be removed by ScopedObserver
}

bool PresearchSyncWorker::SetSyncEnabled(bool enabled) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* setup_service =
      SyncSetupServiceFactory::GetForBrowserState(browser_state_);
  auto* sync_service =
      ProfileSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!setup_service || !sync_service) {
    return false;
  }

  if (!sync_service_observer_.IsObserving(sync_service)) {
    sync_service_observer_.Add(sync_service);
  }

  setup_service->SetSyncEnabled(enabled);

  if (enabled && !sync_service->GetUserSettings()->IsFirstSetupComplete()) {
    // setup_service->PrepareForFirstSyncSetup();
    // setup_service->CommitSyncChanges();
    setup_service->SetFirstSetupComplete(
        syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
  }

  return true;
}

const syncer::DeviceInfo* PresearchSyncWorker::GetLocalDeviceInfo() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!device_info_service) {
    return nullptr;
  }

  return device_info_service->GetLocalDeviceInfoProvider()
      ->GetLocalDeviceInfo();
}

std::vector<std::unique_ptr<syncer::PresearchDeviceInfo>>
PresearchSyncWorker::GetDeviceList() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!device_info_service) {
    return std::vector<std::unique_ptr<syncer::PresearchDeviceInfo>>();
  }

  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  return tracker->GetAllPresearchDeviceInfo();
}

std::string PresearchSyncWorker::GetOrCreateSyncCode() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service) {
    sync_code = sync_service->GetOrCreateSyncCode();
  }
  return sync_code;
}

bool PresearchSyncWorker::IsValidSyncCode(const std::string& sync_code) {
  std::vector<uint8_t> seed;
  if (!presearch_sync::crypto::PassphraseToBytes32(sync_code, &seed)) {
    return false;
  }
  return seed.size() == SEED_BYTES_COUNT;
}

bool PresearchSyncWorker::SetSyncCode(const std::string& sync_code) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  if (sync_code.empty()) {
    return false;
  }

  auto* sync_service = GetSyncService();
  if (!sync_service || !sync_service->SetSyncCode(sync_code)) {
    const std::string error_msg = sync_service
                                      ? "invalid sync code:" + sync_code
                                      : "sync service is not available";
    LOG(ERROR) << error_msg;
    return false;
  }

  passphrase_ = sync_code;
  return true;
}

std::string PresearchSyncWorker::GetSyncCodeFromHexSeed(
    const std::string& hex_code_seed) {
  DCHECK(!hex_code_seed.empty());

  std::vector<uint8_t> bytes;
  std::string sync_code_words;
  if (base::HexStringToBytes(hex_code_seed, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size(), SEED_BYTES_COUNT) {
      sync_code_words = presearch_sync::crypto::PassphraseFromBytes32(bytes);
      if (sync_code_words.empty()) {
        VLOG(1) << __func__
                << " PassphraseFromBytes32 failed for hex_code_seed";
      }
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }

    DCHECK_NE(sync_code_words, "");
  } else {
    VLOG(1) << __func__ << " HexStringToBytes failed for hex_code_seed";
  }
  return sync_code_words;
}

bool PresearchSyncWorker::IsFirstSetupComplete() {
  syncer::SyncService* sync_service = GetSyncService();
  return sync_service &&
         sync_service->GetUserSettings()->IsFirstSetupComplete();
}

void PresearchSyncWorker::ResetSync() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  DCHECK(device_info_service);

  presearch_sync::ResetSync(sync_service, device_info_service,
                        base::BindOnce(&PresearchSyncWorker::OnResetDone,
                                        weak_ptr_factory_.GetWeakPtr()));
}

void PresearchSyncWorker::DeleteDevice(const std::string& device_guid) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  DCHECK(device_info_service);

  presearch_sync::DeleteDevice(sync_service, device_info_service, device_guid);
}

syncer::PresearchProfileSyncService* PresearchSyncWorker::GetSyncService() const {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  return static_cast<syncer::PresearchProfileSyncService*>(
      ProfileSyncServiceFactory::GetForBrowserState(browser_state_));
}

void PresearchSyncWorker::SetEncryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(!sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->IsEncryptEverythingAllowed() &&
      !sync_user_settings->IsUsingSecondaryPassphrase() &&
      !sync_user_settings->IsTrustedVaultKeyRequired()) {
    sync_user_settings->SetEncryptionPassphrase(this->passphrase_);

    VLOG(3) << "[PresearchSync] " << __func__ << " SYNC_CREATED_NEW_PASSPHRASE";
  }
}

void PresearchSyncWorker::SetDecryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->SetDecryptionPassphrase(this->passphrase_)) {
    VLOG(3) << "[PresearchSync] " << __func__
            << " SYNC_ENTERED_EXISTING_PASSPHRASE";
  }
}

void PresearchSyncWorker::OnStateChanged(syncer::SyncService* service) {
  // If the sync engine has shutdown for some reason, just give up
  if (!service || !service->IsEngineInitialized()) {
    VLOG(3) << "[PresearchSync] " << __func__ << " sync engine is not initialized";
    return;
  }

  if (this->passphrase_.empty()) {
    VLOG(3) << "[PresearchSync] " << __func__ << " empty passphrase";
    return;
  }

  presearch_sync::Prefs presearch_sync_prefs(browser_state_->GetPrefs());
  std::string sync_code = presearch_sync_prefs.GetSeed();
  DCHECK_NE(sync_code.size(), 0u);

  if (service->GetUserSettings()->IsPassphraseRequired()) {
    SetDecryptionPassphrase(service);
  } else {
    SetEncryptionPassphrase(service);
  }
}

void PresearchSyncWorker::OnSyncShutdown(syncer::SyncService* service) {
  if (sync_service_observer_.IsObserving(service)) {
    sync_service_observer_.Remove(service);
  }
}

void PresearchSyncWorker::OnResetDone() {
  syncer::SyncService* sync_service = GetSyncService();
  if (sync_service) {
    if (sync_service_observer_.IsObserving(sync_service)) {
      sync_service_observer_.Remove(sync_service);
    }
  }
}

bool PresearchSyncWorker::IsSyncEnabled() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* setup_service =
      SyncSetupServiceFactory::GetForBrowserState(browser_state_);

  if (!setup_service) {
    return false;
  }

  return setup_service->IsSyncEnabled();
}

bool PresearchSyncWorker::IsSyncFeatureActive() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service =
      ProfileSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!sync_service) {
    return false;
  }

  return sync_service->IsSyncFeatureActive();
}
