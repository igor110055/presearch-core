/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_RLP_ENCODE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_RLP_ENCODE_H_

#include <string>

#include "base/values.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_types.h"

namespace presearch_wallet {

// Converts a uint256_t value into a blob value type
base::Value RLPUint256ToBlobValue(uint256_t input);

// Recursive Length Prefix (RLP) encoding of base::Values consisting of string,
// blob, or int data
std::string RLPEncode(base::Value val);

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_RLP_ENCODE_H_
