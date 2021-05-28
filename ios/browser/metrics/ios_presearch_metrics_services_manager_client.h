/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_BROWSER_METRICS_IOS_PRESEARCH_METRICS_SERVICES_MANAGER_CLIENT_H_
#define PRESEARCH_IOS_BROWSER_METRICS_IOS_PRESEARCH_METRICS_SERVICES_MANAGER_CLIENT_H_

#include <memory>
#include "base/macros.h"
#include "ios/chrome/browser/metrics/ios_chrome_metrics_services_manager_client.h"

class PrefService;

namespace metrics {
class EnabledStateProvider;
class MetricsStateManager;
}

// Provides an //ios/presearch-specific implementation of
// MetricsServicesManagerClient.
class IOSPresearchMetricsServicesManagerClient
    : public IOSChromeMetricsServicesManagerClient {
 public:
  explicit IOSPresearchMetricsServicesManagerClient(PrefService* local_state);
  ~IOSPresearchMetricsServicesManagerClient() override;

 private:
  // metrics_services_manager::MetricsServicesManagerClient:
  std::unique_ptr<metrics::MetricsServiceClient> CreateMetricsServiceClient()
      override;
  bool IsMetricsReportingEnabled() override;
  bool IsMetricsConsentGiven() override;
  bool IsOffTheRecordSessionActive() override;

  DISALLOW_COPY_AND_ASSIGN(IOSPresearchMetricsServicesManagerClient);
};

#endif  // PRESEARCH_IOS_BROWSER_METRICS_IOS_PRESEARCH_METRICS_SERVICES_MANAGER_CLIENT_H_
