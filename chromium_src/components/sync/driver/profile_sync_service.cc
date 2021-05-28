/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/sync/driver/presearch_sync_auth_manager.h"
#include "presearch/components/sync/driver/presearch_sync_stopped_reporter.h"

#define SyncAuthManager PresearchSyncAuthManager
#define SyncStoppedReporter PresearchSyncStoppedReporter

#include "../../../../../components/sync/driver/profile_sync_service.cc"

#undef SyncAuthManager
#undef SyncStoppedReporter
