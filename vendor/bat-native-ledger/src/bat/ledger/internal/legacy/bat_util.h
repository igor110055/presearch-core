/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_LEGACY_BAT_UTIL_H_
#define PRESEARCHLEDGER_LEGACY_BAT_UTIL_H_

#include <string>

namespace presearchledger_bat_util {

std::string ConvertToProbi(const std::string& amount);

double ProbiToDouble(const std::string& probi);

}  // namespace presearchledger_bat_util

#endif  // PRESEARCHLEDGER_LEGACY_BAT_UTIL_H_
