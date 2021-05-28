/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMMON_PRESEARCH_FEATURES_H_
#define PRESEARCH_COMMON_PRESEARCH_FEATURES_H_

#include "base/component_export.h"
#include "base/feature_list.h"
#include "build/build_config.h"

namespace features {

#if defined(OS_ANDROID)
COMPONENT_EXPORT(CHROME_FEATURES)
extern const base::Feature kPresearchRewards;
#endif  // defined(OS_ANDROID)

}  // namespace features

#endif  // PRESEARCH_COMMON_PRESEARCH_FEATURES_H_
