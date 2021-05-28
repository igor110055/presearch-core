/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/pref_names.h"

namespace ads {

namespace prefs {

// Stores whether Presearch ads is enabled or disabled
const char kEnabled[] = "presearch.presearch_ads.enabled";

// Stores whether Presearch ads should allow conversion tracking
const char kShouldAllowConversionTracking[] =
    "presearch.presearch_ads.should_allow_ad_conversion_tracking";

// Stores the maximum amount of ads per hour
const char kAdsPerHour[] = "presearch.presearch_ads.ads_per_hour";

// Stores the idle time threshold before checking if an ad can be served
const char kIdleTimeThreshold[] = "presearch.presearch_ads.idle_threshold";

// Stores whether Presearch ads should allow subdivision ad targeting
const char kShouldAllowAdsSubdivisionTargeting[] =
    "presearch.presearch_ads.should_allow_ads_subdivision_targeting";

// Stores the selected ads subdivision targeting code
const char kAdsSubdivisionTargetingCode[] =
    "presearch.presearch_ads.ads_subdivision_targeting_code";

// Stores the automatically detected ads subdivision targeting code
const char kAutoDetectedAdsSubdivisionTargetingCode[] =
    "presearch.presearch_ads.automatically_detected_ads_subdivision_targeting_code";

// Stores catalog id
const char kCatalogId[] = "presearch.presearch_ads.catalog_id";

// Stores catalog version
const char kCatalogVersion[] = "presearch.presearch_ads.catalog_version";

// Stores catalog ping
const char kCatalogPing[] = "presearch.presearch_ads.catalog_ping";

// Stores catalog last updated
const char kCatalogLastUpdated[] = "presearch.presearch_ads.catalog_last_updated";

// Stores epsilon greedy bandit arms
const char kEpsilonGreedyBanditArms[] =
    "presearch.presearch_ads.epsilon_greedy_bandit_arms";

// Stores epsilon greedy bandit eligible segments
const char kEpsilonGreedyBanditEligibleSegments[] =
    "presearch.presearch_ads.epsilon_greedy_bandit_eligible_segments";

// Stores migration status
const char kHasMigratedConversionState[] =
    "presearch.presearch_ads.migrated.conversion_state";

}  // namespace prefs

}  // namespace ads
