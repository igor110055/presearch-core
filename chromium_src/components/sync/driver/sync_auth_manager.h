/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_DRIVER_SYNC_AUTH_MANAGER_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_DRIVER_SYNC_AUTH_MANAGER_H_

namespace syncer {
class PresearchSyncAuthManager;
}  // namespace syncer

// Header guard to prevent DetermineAccountToUse from getting overriden in it
#include "components/sync/driver/sync_auth_util.h"

#define RequestAccessToken virtual RequestAccessToken
#define DetermineAccountToUse                                  \
  DetermineAccountToUse_Unused() { return SyncAccountInfo(); } \
  friend PresearchSyncAuthManager;                                 \
  virtual SyncAccountInfo DetermineAccountToUse

#include "../../../../../components/sync/driver/sync_auth_manager.h"

#undef RequestAccessToken
#undef DetermineAccountToUse
#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_DRIVER_SYNC_AUTH_MANAGER_H_
