/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/feature_list.h"
#include "presearch/components/presearch_shields/common/presearch_shield_constants.h"

#define PRESEARCH_GET_RENDER_CONTENT_SETTING_RULES                             \
  map->GetSettingsForOneType(ContentSettingsType::AUTOPLAY,                \
                             &(rules->autoplay_rules));                    \
  map->GetSettingsForOneType(ContentSettingsType::PRESEARCH_FINGERPRINTING_V2, \
                             &(rules->fingerprinting_rules));              \
  map->GetSettingsForOneType(ContentSettingsType::PRESEARCH_SHIELDS,           \
                             &(rules->presearch_shields_rules));

#include "../../../../../../components/content_settings/core/browser/content_settings_utils.cc"

#undef PRESEARCH_PRESEARCH_GET_RENDER_CONTENT_SETTING_RULES
