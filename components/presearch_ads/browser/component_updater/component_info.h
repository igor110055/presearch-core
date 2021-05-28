/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_INFO_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_INFO_H_

#include <string>

namespace presearch_ads {

struct ComponentInfo {
  ComponentInfo();
  ComponentInfo(const std::string& id, const std::string& public_key);
  ~ComponentInfo();

  std::string id;
  std::string public_key;
};

}  // namespace presearch_ads

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_INFO_H_
