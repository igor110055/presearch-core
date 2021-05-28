/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_VERSION_INFO_H_
#define PRESEARCH_BROWSER_VERSION_INFO_H_

#include <string>

namespace version_info {
std::string GetPresearchVersionWithoutChromiumMajorVersion();
std::string GetPresearchVersionNumberForDisplay();
std::string GetPresearchChromiumVersionNumber();
}  // namespace version_info

#endif  // PRESEARCH_BROWSER_VERSION_INFO_H_
