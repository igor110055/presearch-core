/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_COMPONENT_OBSERVER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_COMPONENT_OBSERVER_H_

#include <string>

#include "base/observer_list.h"

namespace presearch_ads {

class Observer : public base::CheckedObserver {
 public:
  virtual void OnResourceComponentUpdated(const std::string& id) = 0;

 protected:
  ~Observer() override = default;
};

}  // namespace presearch_ads

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_COMPONENT_OBSERVER_H_
