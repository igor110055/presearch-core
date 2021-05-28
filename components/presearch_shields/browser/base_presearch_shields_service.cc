/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_shields/browser/base_presearch_shields_service.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/ptr_util.h"

using presearch_component_updater::PresearchComponent;

namespace presearch_shields {

BasePresearchShieldsService::BasePresearchShieldsService(
    PresearchComponent::Delegate* delegate)
    : PresearchComponent(delegate),
      initialized_(false) {
}

BasePresearchShieldsService::~BasePresearchShieldsService() {
}

bool BasePresearchShieldsService::IsInitialized() const {
  return initialized_;
}

void BasePresearchShieldsService::InitShields() {
  if (Init()) {
    std::lock_guard<std::mutex> guard(initialized_mutex_);
    initialized_ = true;
  }
}

bool BasePresearchShieldsService::Start() {
  if (initialized_) {
    return true;
  }

  InitShields();
  return false;
}

void BasePresearchShieldsService::ShouldStartRequest(
    const GURL& url,
    blink::mojom::ResourceType resource_type,
    const std::string& tab_host,
    bool* did_match_rule,
    bool* did_match_exception,
    bool* did_match_important,
    std::string* mock_data_url) {
}

}  // namespace presearch_shields
