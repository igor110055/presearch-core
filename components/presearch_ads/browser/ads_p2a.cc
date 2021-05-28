/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_ads/browser/ads_p2a.h"

#include <cstdint>
#include <map>
#include <string>

#include "base/metrics/histogram_functions.h"
#include "presearch/components/presearch_ads/common/pref_names.h"
#include "presearch/components/weekly_storage/weekly_storage.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace presearch_ads {
namespace {

constexpr const char* kP2AQuestionNameList[] = {
    // Ad Opportunities
    "Presearch.P2A.TotalAdOpportunities",
    "Presearch.P2A.AdOpportunitiesPerSegment.architecture",
    "Presearch.P2A.AdOpportunitiesPerSegment.artsentertainment",
    "Presearch.P2A.AdOpportunitiesPerSegment.automotive",
    "Presearch.P2A.AdOpportunitiesPerSegment.business",
    "Presearch.P2A.AdOpportunitiesPerSegment.careers",
    "Presearch.P2A.AdOpportunitiesPerSegment.cellphones",
    "Presearch.P2A.AdOpportunitiesPerSegment.crypto",
    "Presearch.P2A.AdOpportunitiesPerSegment.education",
    "Presearch.P2A.AdOpportunitiesPerSegment.familyparenting",
    "Presearch.P2A.AdOpportunitiesPerSegment.fashion",
    "Presearch.P2A.AdOpportunitiesPerSegment.folklore",
    "Presearch.P2A.AdOpportunitiesPerSegment.fooddrink",
    "Presearch.P2A.AdOpportunitiesPerSegment.gaming",
    "Presearch.P2A.AdOpportunitiesPerSegment.healthfitness",
    "Presearch.P2A.AdOpportunitiesPerSegment.history",
    "Presearch.P2A.AdOpportunitiesPerSegment.hobbiesinterests",
    "Presearch.P2A.AdOpportunitiesPerSegment.home",
    "Presearch.P2A.AdOpportunitiesPerSegment.law",
    "Presearch.P2A.AdOpportunitiesPerSegment.military",
    "Presearch.P2A.AdOpportunitiesPerSegment.other",
    "Presearch.P2A.AdOpportunitiesPerSegment.personalfinance",
    "Presearch.P2A.AdOpportunitiesPerSegment.pets",
    "Presearch.P2A.AdOpportunitiesPerSegment.realestate",
    "Presearch.P2A.AdOpportunitiesPerSegment.science",
    "Presearch.P2A.AdOpportunitiesPerSegment.sports",
    "Presearch.P2A.AdOpportunitiesPerSegment.technologycomputing",
    "Presearch.P2A.AdOpportunitiesPerSegment.travel",
    "Presearch.P2A.AdOpportunitiesPerSegment.weather",
    "Presearch.P2A.AdOpportunitiesPerSegment.untargeted",
    // Ad Impressions
    "Presearch.P2A.TotalAdImpressions",
    "Presearch.P2A.AdImpressionsPerSegment.architecture",
    "Presearch.P2A.AdImpressionsPerSegment.artsentertainment",
    "Presearch.P2A.AdImpressionsPerSegment.automotive",
    "Presearch.P2A.AdImpressionsPerSegment.business",
    "Presearch.P2A.AdImpressionsPerSegment.careers",
    "Presearch.P2A.AdImpressionsPerSegment.cellphones",
    "Presearch.P2A.AdImpressionsPerSegment.crypto",
    "Presearch.P2A.AdImpressionsPerSegment.education",
    "Presearch.P2A.AdImpressionsPerSegment.familyparenting",
    "Presearch.P2A.AdImpressionsPerSegment.fashion",
    "Presearch.P2A.AdImpressionsPerSegment.folklore",
    "Presearch.P2A.AdImpressionsPerSegment.fooddrink",
    "Presearch.P2A.AdImpressionsPerSegment.gaming",
    "Presearch.P2A.AdImpressionsPerSegment.healthfitness",
    "Presearch.P2A.AdImpressionsPerSegment.history",
    "Presearch.P2A.AdImpressionsPerSegment.hobbiesinterests",
    "Presearch.P2A.AdImpressionsPerSegment.home",
    "Presearch.P2A.AdImpressionsPerSegment.law",
    "Presearch.P2A.AdImpressionsPerSegment.military",
    "Presearch.P2A.AdImpressionsPerSegment.other",
    "Presearch.P2A.AdImpressionsPerSegment.personalfinance",
    "Presearch.P2A.AdImpressionsPerSegment.pets",
    "Presearch.P2A.AdImpressionsPerSegment.realestate",
    "Presearch.P2A.AdImpressionsPerSegment.science",
    "Presearch.P2A.AdImpressionsPerSegment.sports",
    "Presearch.P2A.AdImpressionsPerSegment.technologycomputing",
    "Presearch.P2A.AdImpressionsPerSegment.travel",
    "Presearch.P2A.AdImpressionsPerSegment.weather",
    "Presearch.P2A.AdImpressionsPerSegment.untargeted"};

const uint16_t kIntervalBuckets[] = {0, 5, 10, 20, 50, 100, 250, 500};

}  // namespace

void RegisterP2APrefs(PrefRegistrySimple* registry) {
  for (const char* question_name : kP2AQuestionNameList) {
    std::string pref_path(prefs::kP2AStoragePrefNamePrefix);
    pref_path.append(question_name);
    registry->RegisterListPref(pref_path);
  }
}

void RecordInWeeklyStorageAndEmitP2AHistogramAnswer(PrefService* prefs,
                                                    const std::string& name) {
  std::string pref_path(prefs::kP2AStoragePrefNamePrefix);
  pref_path.append(name);
  if (!prefs->FindPreference(pref_path)) {
    return;
  }
  WeeklyStorage storage(prefs, pref_path.c_str());
  storage.AddDelta(1);
  EmitP2AHistogramAnswer(name, storage.GetWeeklySum());
}

void EmitP2AHistogramAnswer(const std::string& name, uint16_t count_value) {
  const uint16_t* iter = std::lower_bound(
      kIntervalBuckets, std::end(kIntervalBuckets), count_value);
  const uint16_t bucket = iter - kIntervalBuckets;

  for (const char* question_name : kP2AQuestionNameList) {
    if (name != question_name) {
      continue;
    }

    base::UmaHistogramExactLinear(question_name, bucket,
                                  base::size(kIntervalBuckets) + 1);
  }
}

void SuspendP2AHistograms() {
  for (const char* question_name : kP2AQuestionNameList) {
    base::UmaHistogramExactLinear(question_name, INT_MAX,
                                  base::size(kIntervalBuckets) + 1);
  }
}

}  // namespace presearch_ads
