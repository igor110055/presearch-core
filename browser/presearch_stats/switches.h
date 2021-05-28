// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_BROWSER_PRESEARCH_STATS_SWITCHES_H_
#define PRESEARCH_BROWSER_PRESEARCH_STATS_SWITCHES_H_

namespace presearch_stats {

namespace switches {

// Allows setting the usage server to a custom host. Useful both for manual
// testing against staging and for browser tests.
constexpr char kPresearchStatsUpdaterServer[] =
    "presearch-stats-updater-server";

}  // namespace switches
}  // namespace presearch_stats

#endif  // PRESEARCH_BROWSER_PRESEARCH_STATS_SWITCHES_H_
