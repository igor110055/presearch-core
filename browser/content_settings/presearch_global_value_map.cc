/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/content_settings/presearch_global_value_map.h"

#include <memory>

#include "presearch/components/content_settings/core/browser/presearch_content_settings_utils.h"
#include "components/content_settings/core/browser/content_settings_rule.h"
#include "components/content_settings/core/common/content_settings.h"

namespace content_settings {

PresearchGlobalValueMap::PresearchGlobalValueMap() {}

PresearchGlobalValueMap::~PresearchGlobalValueMap() {}

std::unique_ptr<RuleIterator> PresearchGlobalValueMap::GetRuleIterator(
    ContentSettingsType content_type) const {
  if (content_settings::IsShieldsContentSettingsType(content_type))
    return nullptr;
  return GlobalValueMap::GetRuleIterator(content_type);
}

void PresearchGlobalValueMap::SetContentSetting(ContentSettingsType content_type,
                                            ContentSetting setting) {
  return GlobalValueMap::SetContentSetting(content_type, setting);
}

ContentSetting PresearchGlobalValueMap::GetContentSetting(
    ContentSettingsType content_type) const {
  return GlobalValueMap::GetContentSetting(content_type);
}

}  // namespace content_settings
