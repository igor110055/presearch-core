/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SYNC_PROFILE_SYNC_SERVICE_HELPER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SYNC_PROFILE_SYNC_SERVICE_HELPER_H_

#include <string>

#include "base/callback.h"

namespace syncer {
class PresearchProfileSyncService;
class DeviceInfoSyncService;
}  // namespace syncer

namespace presearch_sync {

// Helper function to break circular dependency between //components/sync/driver
// and //component/sync_device_info
void ResetSync(syncer::PresearchProfileSyncService* sync_service,
               syncer::DeviceInfoSyncService* device_info_service,
               base::OnceClosure on_reset_done);

void DeleteDevice(syncer::PresearchProfileSyncService* sync_service,
                  syncer::DeviceInfoSyncService* device_info_service,
                  const std::string& device_guid);

}  // namespace presearch_sync

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SYNC_PROFILE_SYNC_SERVICE_HELPER_H_
