/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_ENDPOINT_API_API_UTIL_H_
#define PRESEARCHLEDGER_ENDPOINT_API_API_UTIL_H_

#include <string>

namespace ledger {
namespace endpoint {
namespace api {

std::string GetServerUrl(const std::string& path);

}  // namespace api
}  // namespace endpoint
}  // namespace ledger

#endif  // PRESEARCHLEDGER_ENDPOINT_API_API_UTIL_H_
