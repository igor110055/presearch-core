/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_BROWSER_API_SYNC_PRESEARCH_SYNC_WORKER_H_
#define PRESEARCH_IOS_BROWSER_API_SYNC_PRESEARCH_SYNC_WORKER_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "base/scoped_observer.h"
#include "components/sync/driver/sync_service.h"
#include "components/sync/driver/sync_service_observer.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"

class ChromeBrowserState;

namespace syncer {
class PresearchProfileSyncService;
class DeviceInfo;
class PresearchDeviceInfo;
class ProfileSyncService;
}  // namespace syncer

class PresearchSyncDeviceTracker : public syncer::DeviceInfoTracker::Observer {
 public:
  PresearchSyncDeviceTracker(syncer::DeviceInfoTracker* device_info_tracker,
                         std::function<void()> on_device_info_changed_callback);
  virtual ~PresearchSyncDeviceTracker();

 private:
  void OnDeviceInfoChange() override;

  std::function<void()> on_device_info_changed_callback_;

  ScopedObserver<syncer::DeviceInfoTracker, syncer::DeviceInfoTracker::Observer>
      device_info_tracker_observer_{this};
};

class PresearchSyncServiceTracker : public syncer::SyncServiceObserver {
 public:
  PresearchSyncServiceTracker(syncer::ProfileSyncService* profile_sync_service,
                          std::function<void()> on_state_changed_callback);
  ~PresearchSyncServiceTracker() override;

 private:
  void OnStateChanged(syncer::SyncService* sync) override;

  std::function<void()> on_state_changed_callback_;

  ScopedObserver<syncer::SyncService, syncer::SyncServiceObserver>
      sync_service_observer_{this};
};

class PresearchSyncWorker : public syncer::SyncServiceObserver {
 public:
  explicit PresearchSyncWorker(ChromeBrowserState* browser_state_);
  ~PresearchSyncWorker() override;

  bool SetSyncEnabled(bool enabled);
  std::string GetOrCreateSyncCode();
  bool IsValidSyncCode(const std::string& sync_code);
  bool SetSyncCode(const std::string& sync_code);
  std::string GetSyncCodeFromHexSeed(const std::string& hex_seed);
  const syncer::DeviceInfo* GetLocalDeviceInfo();
  std::vector<std::unique_ptr<syncer::PresearchDeviceInfo>> GetDeviceList();
  bool IsSyncEnabled();
  bool IsSyncFeatureActive();
  bool IsFirstSetupComplete();
  void ResetSync();
  void DeleteDevice(const std::string& device_guid);

 private:
  // syncer::SyncServiceObserver implementation.

  syncer::PresearchProfileSyncService* GetSyncService() const;
  void OnStateChanged(syncer::SyncService* service) override;
  void OnSyncShutdown(syncer::SyncService* service) override;

  void OnResetDone();

  void SetEncryptionPassphrase(syncer::SyncService* service);
  void SetDecryptionPassphrase(syncer::SyncService* service);

  std::string passphrase_;

  ChromeBrowserState* browser_state_;  // NOT OWNED
  ScopedObserver<syncer::SyncService, syncer::SyncServiceObserver>
      sync_service_observer_{this};
  base::WeakPtrFactory<PresearchSyncWorker> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(PresearchSyncWorker);
};

#endif  // PRESEARCH_IOS_BROWSER_API_SYNC_PRESEARCH_SYNC_WORKER_H_
