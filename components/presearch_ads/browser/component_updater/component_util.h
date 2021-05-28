/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_UTIL_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_UTIL_H_

#include <string>

#include "base/optional.h"
#include "presearch/components/presearch_ads/browser/component_updater/component_info.h"

namespace presearch_ads {

base::Optional<ComponentInfo> GetComponentInfo(const std::string& id);

}  // namespace presearch_ads

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_COMPONENT_UTIL_H_
