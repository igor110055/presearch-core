/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_ADS_P2A_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_ADS_P2A_H_

#include <cstdint>

#include <string>
#include <vector>

class PrefService;
class PrefRegistrySimple;

namespace presearch_ads {

void RegisterP2APrefs(PrefRegistrySimple* prefs);

void RecordInWeeklyStorageAndEmitP2AHistogramAnswer(PrefService* prefs,
                                                    const std::string& name);

void EmitP2AHistogramAnswer(const std::string& name, uint16_t count_value);

void SuspendP2AHistograms();

}  // namespace presearch_ads

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_ADS_P2A_H_
