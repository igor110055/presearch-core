/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_

#include <string>
#include "base/values.h"

namespace presearch_wallet {

// Returns the balance of the account of given address.
bool ParseEthGetBalance(const std::string& json, std::string* hex_balance);
bool ParseEthCall(const std::string& json, std::string* result);

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_
