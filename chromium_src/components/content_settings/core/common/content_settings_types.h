/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_CONTENT_SETTINGS_TYPES_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_CONTENT_SETTINGS_TYPES_H_

// clang-format off
#define PRESEARCH_CONTENT_SETTINGS_TYPES_LIST                                      \
  PRESEARCH_ADS,                                                                   \
  PRESEARCH_COSMETIC_FILTERING,                                                    \
  PRESEARCH_TRACKERS,                                                              \
  PRESEARCH_HTTP_UPGRADABLE_RESOURCES,                                             \
  PRESEARCH_FINGERPRINTING_V2,                                                     \
  PRESEARCH_SHIELDS,                                                               \
  PRESEARCH_REFERRERS,                                                             \
  PRESEARCH_COOKIES
// clang-format on

#include "../../../../../../components/content_settings/core/common/content_settings_types.h"

#undef PRESEARCH_CONTENT_SETTINGS_TYPES_LIST

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_CONTENT_SETTINGS_TYPES_H_
