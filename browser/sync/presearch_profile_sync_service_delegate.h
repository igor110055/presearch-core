/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_SYNC_PRESEARCH_PROFILE_SYNC_SERVICE_DELEGATE_H_
#define PRESEARCH_BROWSER_SYNC_PRESEARCH_PROFILE_SYNC_SERVICE_DELEGATE_H_

#include "presearch/components/sync/driver/profile_sync_service_delegate.h"

#include "base/memory/weak_ptr.h"
#include "base/scoped_observer.h"
#include "components/sync/driver/profile_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"

class Profile;

namespace syncer {

class DeviceInfoSyncService;
class DeviceInfoTracker;
class LocalDeviceInfoProvider;

// Helper class to prevent pass of profile pointer into PresearchProfileSyncService
// and to keep DeviceInfoSyncService
class PresearchProfileSyncServiceDelegate
    : public ProfileSyncServiceDelegate,
      public syncer::DeviceInfoTracker::Observer {
 public:
  explicit PresearchProfileSyncServiceDelegate(
      DeviceInfoSyncService* device_info_sync_service);
  ~PresearchProfileSyncServiceDelegate() override;

  void SuspendDeviceObserverForOwnReset() override;
  void ResumeDeviceObserver() override;

 private:
  // syncer::DeviceInfoTracker::Observer:
  void OnDeviceInfoChange() override;

  void OnSelfDeviceInfoDeleted(void);

  syncer::DeviceInfoTracker* device_info_tracker_;
  syncer::LocalDeviceInfoProvider* local_device_info_provider_;
  ScopedObserver<syncer::DeviceInfoTracker, syncer::DeviceInfoTracker::Observer>
      device_info_observer_{this};

  DeviceInfoSyncService* device_info_sync_service_;

  base::WeakPtrFactory<PresearchProfileSyncServiceDelegate> weak_ptr_factory_;

  PresearchProfileSyncServiceDelegate(const PresearchProfileSyncServiceDelegate&) =
      delete;
  PresearchProfileSyncServiceDelegate& operator=(
      const PresearchProfileSyncServiceDelegate&) = delete;
};

}  // namespace syncer

#endif  // PRESEARCH_BROWSER_SYNC_PRESEARCH_PROFILE_SYNC_SERVICE_DELEGATE_H_
