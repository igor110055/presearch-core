/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_MANAGER_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_MANAGER_H_

#define GetLastUsedProfileName virtual GetLastUsedProfileName
#define InitProfileUserPrefs virtual InitProfileUserPrefs
#define DoFinalInitForServices virtual DoFinalInitForServices
#define SetNonPersonalProfilePrefs virtual SetNonPersonalProfilePrefs
#define IsAllowedProfilePath virtual IsAllowedProfilePath
#define LoadProfileByPath virtual LoadProfileByPath
#define PRESEARCH_PROFILE_MANAGER_H     \
 private:                           \
  friend class PresearchProfileManager; \
                                    \
 public:
#include "../../../../../chrome/browser/profiles/profile_manager.h"
#undef LoadProfileByPath
#undef IsAllowedProfilePath
#undef SetNonPersonalProfilePrefs
#undef DoFinalInitForServices
#undef InitProfileUserPrefs
#undef GetLastUsedProfileName

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_MANAGER_H_
