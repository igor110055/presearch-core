/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_REWARDS_P3A_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_REWARDS_P3A_H_

#include <stddef.h>
#include <ctype.h>

#include <string>

#include "base/containers/flat_map.h"
#include "base/strings/string_piece.h"

class PrefService;

namespace base {
class DictionaryValue;
}

namespace presearch_rewards {

void RecordWalletBalanceP3A(bool wallet_created, bool rewards_enabled,
                            size_t balance);

enum class AutoContributionsP3AState {
  kNoWallet,
  kRewardsDisabled,
  kWalletCreatedAutoContributeOff,
  kAutoContributeOn,
};

void RecordAutoContributionsState(AutoContributionsP3AState state, int count);

void RecordTipsState(bool wallet_created,
                     bool rewards_enabled,
                     int one_time_count,
                     int recurring_count);

enum class AdsP3AState {
  kNoWallet,
  kRewardsDisabled,
  kAdsDisabled,
  kAdsEnabled,
  kAdsEnabledThenDisabledRewardsOn,
  kAdsEnabledThenDisabledRewardsOff,
  kMaxValue = kAdsEnabledThenDisabledRewardsOff,
};

void RecordAdsState(AdsP3AState state);

void UpdateAdsP3AOnPreferenceChange(PrefService* prefs,
                                    const std::string& pref);

// Records an initial metric state ("disabled" or "enabled") if it was not done
// before. Intended to be called if the user has already created a wallet.
void MaybeRecordInitialAdsP3AState(PrefService* local_state);

void RecordNoWalletCreatedForAllMetrics();

void RecordRewardsDisabledForSomeMetrics();

double CalcWalletBalanceForP3A(base::flat_map<std::string, double> wallets,
                               double user_funds);

uint64_t RoundProbiToUint64(base::StringPiece probi);

void ExtractAndLogP3AStats(const base::DictionaryValue& dict);

}  // namespace presearch_rewards

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_REWARDS_P3A_H_
