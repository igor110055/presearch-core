/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_COMMON_PRESEARCH_SHIELD_UTILS_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_COMMON_PRESEARCH_SHIELD_UTILS_H_

#include "components/content_settings/core/common/content_settings.h"

class GURL;

ContentSetting GetPresearchFPContentSettingFromRules(
    const ContentSettingsForOneType& fp_rules,
    const GURL& primary_url);

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_COMMON_PRESEARCH_SHIELD_UTILS_H_
