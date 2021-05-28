/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_CONTENT_SETTINGS_MOJOM_TRAITS_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_CONTENT_SETTINGS_MOJOM_TRAITS_H_

#define PRESEARCH_CONTENT_SETTINGS_MOJOM_TRAITS_H                                  \
  static const std::vector<ContentSettingPatternSource>& autoplay_rules(       \
      const RendererContentSettingRules& r) {                                  \
    return r.autoplay_rules;                                                   \
  }                                                                            \
  static const std::vector<ContentSettingPatternSource>& fingerprinting_rules( \
      const RendererContentSettingRules& r) {                                  \
    return r.fingerprinting_rules;                                             \
  }                                                                            \
  static const std::vector<ContentSettingPatternSource>& presearch_shields_rules(  \
      const RendererContentSettingRules& r) {                                  \
    return r.presearch_shields_rules;                                              \
  }

#include "../../../../../../components/content_settings/core/common/content_settings_mojom_traits.h"

#undef PRESEARCH_CONTENT_SETTINGS_MOJOM_TRAITS_H

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_CONTENT_SETTINGS_MOJOM_TRAITS_H_
