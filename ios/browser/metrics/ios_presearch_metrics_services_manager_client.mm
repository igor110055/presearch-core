/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/ios/browser/metrics/ios_presearch_metrics_services_manager_client.h"

#include "components/metrics/metrics_service_client.h"
#include "components/prefs/pref_service.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

IOSPresearchMetricsServicesManagerClient::IOSPresearchMetricsServicesManagerClient(
    PrefService* local_state)
    : IOSChromeMetricsServicesManagerClient(local_state) {}

IOSPresearchMetricsServicesManagerClient::
    ~IOSPresearchMetricsServicesManagerClient() = default;

std::unique_ptr<metrics::MetricsServiceClient>
IOSPresearchMetricsServicesManagerClient::CreateMetricsServiceClient() {
  return nullptr;
}

bool IOSPresearchMetricsServicesManagerClient::IsMetricsReportingEnabled() {
  return false;
}

bool IOSPresearchMetricsServicesManagerClient::IsMetricsConsentGiven() {
  return false;
}

bool IOSPresearchMetricsServicesManagerClient::IsOffTheRecordSessionActive() {
  return false;
}
