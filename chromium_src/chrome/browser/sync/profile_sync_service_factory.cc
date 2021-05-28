/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/signin/presearch_identity_manager_factory.h"
#include "presearch/browser/sync/presearch_profile_sync_service_delegate.h"
#include "presearch/components/signin/public/identity_manager/presearch_identity_manager.h"
#include "presearch/components/sync/driver/presearch_sync_profile_sync_service.h"
#include "chrome/browser/sync/device_info_sync_service_factory.h"

#define IdentityManagerFactory PresearchIdentityManagerFactory

#define PRESEARCH_BUILD_SERVICE_INSTANCE_FOR                         \
  std::make_unique<syncer::PresearchProfileSyncService>(             \
      std::move(init_params),                                    \
      std::make_unique<syncer::PresearchProfileSyncServiceDelegate>( \
          DeviceInfoSyncServiceFactory::GetForProfile(profile)));

#include "../../../../../chrome/browser/sync/profile_sync_service_factory.cc"

#undef PRESEARCH_BUILD_SERVICE_INSTANCE_FOR

#undef IdentityManagerFactory
