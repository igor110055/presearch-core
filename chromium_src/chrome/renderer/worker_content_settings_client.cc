/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/renderer/worker_content_settings_client.h"

#include "presearch/components/presearch_shields/common/presearch_shield_utils.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"

PresearchFarblingLevel WorkerContentSettingsClient::GetPresearchFarblingLevel() {
  ContentSetting setting = CONTENT_SETTING_DEFAULT;
  if (content_setting_rules_) {
    const GURL& primary_url = top_frame_origin_.GetURL();
    const GURL& secondary_url = document_origin_.GetURL();
    for (const auto& rule : content_setting_rules_->presearch_shields_rules) {
      if (rule.primary_pattern.Matches(primary_url) &&
          rule.secondary_pattern.Matches(secondary_url)) {
        setting = rule.GetContentSetting();
        break;
      }
    }
    if (setting == CONTENT_SETTING_BLOCK) {
      // Presearch Shields is down
      setting = CONTENT_SETTING_ALLOW;
    } else {
      // Presearch Shields is up, so check fingerprinting rules
      setting = GetPresearchFPContentSettingFromRules(
          content_setting_rules_->fingerprinting_rules, primary_url);
    }
  }
  if (setting == CONTENT_SETTING_BLOCK) {
    return PresearchFarblingLevel::MAXIMUM;
  } else if (setting == CONTENT_SETTING_ALLOW) {
    return PresearchFarblingLevel::OFF;
  } else {
    return PresearchFarblingLevel::BALANCED;
  }
}

bool WorkerContentSettingsClient::AllowFingerprinting(
    bool enabled_per_settings) {
  if (!enabled_per_settings)
    return false;

  return GetPresearchFarblingLevel() != PresearchFarblingLevel::MAXIMUM;
}

#include "../../../../chrome/renderer/worker_content_settings_client.cc"
