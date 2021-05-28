/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_component_updater/browser/presearch_on_demand_updater.h"

#include <string>

#include "base/memory/singleton.h"

namespace presearch_component_updater {

PresearchOnDemandUpdater* PresearchOnDemandUpdater::GetInstance() {
  return base::Singleton<PresearchOnDemandUpdater>::get();
}

PresearchOnDemandUpdater::PresearchOnDemandUpdater() {}

PresearchOnDemandUpdater::~PresearchOnDemandUpdater() {}

void PresearchOnDemandUpdater::OnDemandUpdate(const std::string& id) {
  DCHECK(!on_demand_update_callback_.is_null());
  on_demand_update_callback_.Run(id);
}

void PresearchOnDemandUpdater::RegisterOnDemandUpdateCallback(Callback callback) {
  on_demand_update_callback_ = callback;
}


}  // namespace presearch_component_updater
