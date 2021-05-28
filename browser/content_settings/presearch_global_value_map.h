/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_CONTENT_SETTINGS_PRESEARCH_GLOBAL_VALUE_MAP_H_
#define PRESEARCH_BROWSER_CONTENT_SETTINGS_PRESEARCH_GLOBAL_VALUE_MAP_H_

#include <memory>

#include "components/content_settings/core/browser/content_settings_global_value_map.h"
#include "components/content_settings/core/browser/content_settings_provider.h"
#include "components/content_settings/core/common/content_settings_types.h"

namespace content_settings {

class RuleIterator;

class PresearchGlobalValueMap : public GlobalValueMap {
 public:
  PresearchGlobalValueMap();
  ~PresearchGlobalValueMap();

  // Returns nullptr to indicate the RuleIterator is empty.
  std::unique_ptr<RuleIterator> GetRuleIterator(
      ContentSettingsType content_type) const;
  void SetContentSetting(ContentSettingsType content_type,
                         ContentSetting setting);
  ContentSetting GetContentSetting(ContentSettingsType content_type) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchGlobalValueMap);
};

}  // namespace content_settings

#endif  // PRESEARCH_BROWSER_CONTENT_SETTINGS_PRESEARCH_GLOBAL_VALUE_MAP_H_
