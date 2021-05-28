/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_INFO_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_INFO_H_

#include <stdint.h>

#include <string>

#include "base/files/file_path.h"

namespace presearch_ads {

struct ResourceInfo {
  std::string id;
  uint16_t version;
  base::FilePath path;
};

}  // namespace presearch_ads

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_INFO_H_
