/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_WALLET_UTIL_H_
#define PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_WALLET_UTIL_H_

#include <string>

namespace extensions {

std::string GetInfuraProjectID();

std::string GetPresearchKey();

bool HasInfuraProjectID();

}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_WALLET_UTIL_H_
