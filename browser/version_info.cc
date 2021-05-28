/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/version_info.h"

namespace version_info {

std::string GetPresearchVersionWithoutChromiumMajorVersion() {
  return std::string(PRESEARCH_BROWSER_VERSION);
}

std::string GetPresearchVersionNumberForDisplay() {
  return std::string(PRESEARCH_BROWSER_VERSION) +
      "  Chromium: " + PRESEARCH_CHROMIUM_VERSION;
}

std::string GetPresearchChromiumVersionNumber() {
  return std::string(PRESEARCH_CHROMIUM_VERSION);
}

}  // namespace version_info
