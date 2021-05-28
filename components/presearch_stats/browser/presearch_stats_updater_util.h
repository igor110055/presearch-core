/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_STATS_BROWSER_PRESEARCH_STATS_UPDATER_UTIL_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_STATS_BROWSER_PRESEARCH_STATS_UPDATER_UTIL_H_

#include <string>

#include "base/time/time.h"
#include "base/system/sys_info.h"
#include "components/prefs/pref_service.h"

namespace presearch_stats {

enum class ProcessArch {
  kArchSkip,
  kArchMetal,
  kArchVirt,
};

std::string GetDateAsYMD(const base::Time& time);

std::string GetPlatformIdentifier();

int GetIsoWeekNumber(const base::Time& time);

base::Time GetYMDAsDate(const base::StringPiece& ymd);

std::string GetAPIKey();

}  // namespace presearch_stats

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_STATS_BROWSER_PRESEARCH_STATS_UPDATER_UTIL_H_
