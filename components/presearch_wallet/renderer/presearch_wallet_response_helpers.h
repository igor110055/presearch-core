/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_RENDERER_PRESEARCH_WALLET_RESPONSE_HELPERS_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_RENDERER_PRESEARCH_WALLET_RESPONSE_HELPERS_H_

#include <string>

#include "presearch/components/presearch_wallet/renderer/web3_provider_constants.h"

namespace presearch_wallet {

std::string FormProviderResponse(ProviderErrors code,
                                 const std::string& message);
std::string FormProviderResponse(const std::string& controller_response,
                                 bool* reject);

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_RENDERER_PRESEARCH_WALLET_RESPONSE_HELPERS_H_
