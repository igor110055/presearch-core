// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_METRICS_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_METRICS_H_

#define LogProfileAvatarSelection \
    LogProfileAvatarSelection_ChromiumImpl(size_t index); \
    static void LogProfileAvatarSelection
#include "../../../../../chrome/browser/profiles/profile_metrics.h"
#undef LogProfileAvatarSelection

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_METRICS_H_
