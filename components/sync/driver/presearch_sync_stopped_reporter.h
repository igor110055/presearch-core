/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_SYNC_DRIVER_PRESEARCH_SYNC_STOPPED_REPORTER_H_
#define PRESEARCH_COMPONENTS_SYNC_DRIVER_PRESEARCH_SYNC_STOPPED_REPORTER_H_

#include <string>

#include "components/sync/driver/sync_stopped_reporter.h"

namespace syncer {

class PresearchSyncStoppedReporter : public SyncStoppedReporter {
 public:
  PresearchSyncStoppedReporter(
      const GURL& sync_service_url,
      const std::string& user_agent,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      ResultCallback callback);
  ~PresearchSyncStoppedReporter() override;

  void ReportSyncStopped(const std::string& access_token,
                         const std::string& cache_guid,
                         const std::string& birthday) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchSyncStoppedReporter);
};

}  // namespace syncer

#endif  // PRESEARCH_COMPONENTS_SYNC_DRIVER_PRESEARCH_SYNC_STOPPED_REPORTER_H_
