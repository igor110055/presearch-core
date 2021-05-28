/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/grit/presearch_theme_resources_map.h"

#define PRESEARCH_RESOURCES_UTIL                              \
  for (size_t i = 0; i < kPresearchThemeResourcesSize; ++i) { \
    storage.emplace_back(kPresearchThemeResources[i].path,    \
                         kPresearchThemeResources[i].id);     \
  }

#include "../../../../chrome/browser/resources_util.cc"
#undef PRESEARCH_RESOURCES_UTIL
