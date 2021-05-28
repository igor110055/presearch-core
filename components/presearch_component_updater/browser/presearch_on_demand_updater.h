/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_COMPONENT_UPDATER_BROWSER_PRESEARCH_ON_DEMAND_UPDATER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_COMPONENT_UPDATER_BROWSER_PRESEARCH_ON_DEMAND_UPDATER_H_

#include <string>

#include "base/callback.h"
#include "base/macros.h"

namespace base {
template <typename T>
struct DefaultSingletonTraits;
}  // namespace base

namespace presearch_component_updater {

class PresearchOnDemandUpdater {
 public:
  using Callback = base::RepeatingCallback<void(const std::string&)>;
  static PresearchOnDemandUpdater* GetInstance();

  ~PresearchOnDemandUpdater();
  void OnDemandUpdate(const std::string& id);

  void RegisterOnDemandUpdateCallback(Callback callback);

 private:
  friend struct base::DefaultSingletonTraits<PresearchOnDemandUpdater>;
  PresearchOnDemandUpdater();

  Callback on_demand_update_callback_;

  DISALLOW_COPY_AND_ASSIGN(PresearchOnDemandUpdater);
};

}  // namespace presearch_component_updater

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_COMPONENT_UPDATER_BROWSER_PRESEARCH_ON_DEMAND_UPDATER_H_
