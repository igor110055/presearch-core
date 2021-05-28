/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_ETH_CALL_DATA_BUILDER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_ETH_CALL_DATA_BUILDER_H_

#include <string>
#include <vector>
#include "base/values.h"

namespace presearch_wallet {

namespace erc20 {

// Returns the balance of an  address
bool BalanceOf(const std::string& address, std::string* data);

}  // namespace erc20

namespace unstoppable_domains {

// Get mutiple record values mapped with keys of the target domain.
bool GetMany(const std::vector<std::string>& keys,
             const std::string& domain,
             std::string* data);

}  // namespace unstoppable_domains

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_ETH_CALL_DATA_BUILDER_H_
