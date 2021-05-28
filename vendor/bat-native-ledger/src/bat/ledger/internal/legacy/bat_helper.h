/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_BAT_HELPER_H_
#define PRESEARCHLEDGER_BAT_HELPER_H_

#include <string>
#include <vector>
#include <functional>

#include "base/containers/flat_map.h"
#include "bat/ledger/internal/legacy/wallet_info_properties.h"
#include "bat/ledger/internal/constants.h"
#include "bat/ledger/ledger.h"

namespace presearchledger_bat_helper {

bool getJSONValue(const std::string& fieldName,
                  const std::string& json,
                  std::string* value);

bool getJSONTwitchProperties(
    const std::string& json,
    std::vector<base::flat_map<std::string, std::string>>* parts);

std::string getBase64(const std::vector<uint8_t>& in);

// Sign using ed25519 algorithm
std::string sign(
    const std::vector<std::string>& keys,
    const std::vector<std::string>& values,
    const std::string& key_id,
    const std::vector<uint8_t>& secretKey);

bool HasSameDomainAndPath(
    const std::string& url,
    const std::string& to_match,
    const std::string& path);

}  // namespace presearchledger_bat_helper

#endif  // PRESEARCHLEDGER_BAT_HELPER_H_
