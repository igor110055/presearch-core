/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_SYNC_ENGINE_PRESEARCH_SYNC_MANAGER_IMPL_H_
#define PRESEARCH_COMPONENTS_SYNC_ENGINE_PRESEARCH_SYNC_MANAGER_IMPL_H_

#include <string>

#include "components/sync/engine/sync_manager_impl.h"

namespace syncer {

class PresearchSyncManagerImpl : public SyncManagerImpl {
 public:
  PresearchSyncManagerImpl(
      const std::string& name,
      network::NetworkConnectionTracker* network_connection_tracker);
  ~PresearchSyncManagerImpl() override;

  void StartSyncingNormally(base::Time last_poll_time) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchSyncManagerImpl);
};

}  // namespace syncer

#endif  // PRESEARCH_COMPONENTS_SYNC_ENGINE_PRESEARCH_SYNC_MANAGER_IMPL_H_
