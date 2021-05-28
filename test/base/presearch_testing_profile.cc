/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/test/base/presearch_testing_profile.h"

#include "presearch/browser/profiles/profile_util.h"
#include "presearch/common/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"

PresearchTestingProfile::PresearchTestingProfile(const base::FilePath& path,
                                         Delegate* delegate)
    : TestingProfile(path, delegate) {
  if (presearch::IsSessionProfilePath(path)) {
    presearch::CreateParentProfileData(this);
  }
}

PresearchTestingProfile::PresearchTestingProfile()
    : TestingProfile() {
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  GetPrefs()->SetBoolean(kPresearchGCMChannelStatus, true);
#endif
}
