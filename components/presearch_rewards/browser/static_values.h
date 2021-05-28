/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_STATIC_VALUES_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_STATIC_VALUES_H_

#include <stdint.h>

#include <map>
#include <string>
#include <vector>

#include "base/time/time.h"
#include "bat/ledger/option_keys.h"

namespace presearch_rewards {

const std::vector<std::string> kBitflyerCountries = {
    "JP"  // ID: 19024
};

#if defined(OS_ANDROID)
  const std::map<std::string, bool> kBoolOptions = {
      {ledger::option::kClaimUGP, false}
  };

  const std::map<std::string, int> kIntegerOptions = {};

  const std::map<std::string, double> kDoubleOptions = {};

  const std::map<std::string, std::string> kStringOptions = {};

  const std::map<std::string, int64_t> kInt64Options = {};

  const std::map<std::string, uint64_t> kUInt64Options = {
      {ledger::option::kPublisherListRefreshInterval,
       7* base::Time::kHoursPerDay * base::Time::kSecondsPerHour}};
#else
  const std::map<std::string, bool> kBoolOptions = {
      {ledger::option::kClaimUGP, false}
  };

  const std::map<std::string, int> kIntegerOptions = {};

  const std::map<std::string, double> kDoubleOptions = {};

  const std::map<std::string, std::string> kStringOptions = {};

  const std::map<std::string, int64_t> kInt64Options = {};

  const std::map<std::string, uint64_t> kUInt64Options = {
      {ledger::option::kPublisherListRefreshInterval,
       3 * base::Time::kHoursPerDay * base::Time::kSecondsPerHour}};
#endif

}  // namespace presearch_rewards

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_STATIC_VALUES_H_
