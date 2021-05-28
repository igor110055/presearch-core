/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/content_settings/core/browser/presearch_content_settings_default_provider.h"

#include "presearch/components/content_settings/core/browser/presearch_content_settings_utils.h"
#include "build/build_config.h"
#include "components/prefs/pref_service.h"

namespace content_settings {

// static
void PresearchDefaultProvider::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  DefaultProvider::RegisterProfilePrefs(registry);
}

PresearchDefaultProvider::PresearchDefaultProvider(PrefService* prefs,
                                           bool off_the_record)
    : DefaultProvider(prefs, off_the_record) {}

PresearchDefaultProvider::~PresearchDefaultProvider() {}

std::unique_ptr<RuleIterator> PresearchDefaultProvider::GetRuleIterator(
    ContentSettingsType content_type,
    bool off_the_record) const {
  if (content_settings::IsShieldsContentSettingsType(content_type))
    return nullptr;

  return DefaultProvider::GetRuleIterator(content_type, off_the_record);
}

}  // namespace content_settings
