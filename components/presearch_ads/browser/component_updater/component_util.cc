/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_ads/browser/component_updater/component_util.h"

#include "presearch/components/presearch_ads/browser/component_updater/components.h"

namespace presearch_ads {

base::Optional<ComponentInfo> GetComponentInfo(const std::string& id) {
  const auto iter = components.find(id);
  if (iter == components.end()) {
    return base::nullopt;
  }

  return iter->second;
}

}  // namespace presearch_ads
