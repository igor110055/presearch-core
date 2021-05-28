/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_UTIL_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_UTIL_H_

#include <string>

#include "base/files/file_path.h"
#include "bat/ledger/internal/common/request_util.h"
#include "presearch/components/presearch_rewards/browser/rewards_service_impl.h"
#include "chrome/browser/ui/browser.h"
#include "net/test/embedded_test_server/default_handlers.h"
#include "url/gurl.h"

namespace rewards_browsertest_util {

enum class TipAction { OneTime, SetMonthly, ChangeMonthly, ClearMonthly };

void GetTestDataDir(base::FilePath* test_data_dir);

GURL GetRewardsUrl();

GURL GetRewardsInternalsUrl();

GURL GetNewTabUrl();

void StartProcess(presearch_rewards::RewardsServiceImpl* rewards_service);

GURL GetUrl(
    net::EmbeddedTestServer* https_server,
    const std::string& publisher_key,
    const std::string& path = "");

void ActivateTabAtIndex(Browser* browser, const int index);

std::string BalanceDoubleToString(double amount);

std::string GetUpholdExternalAddress();

void NavigateToPublisherPage(
    Browser* browser,
    net::EmbeddedTestServer* https_server,
    const std::string& publisher_key,
    const std::string& path = "");

void WaitForLedgerStop(presearch_rewards::RewardsServiceImpl* rewards_service);

void CreateWallet(presearch_rewards::RewardsServiceImpl* rewards_service);

void SetOnboardingBypassed(Browser* browser, bool bypassed = true);

}  // namespace rewards_browsertest_util

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_UTIL_H_
