/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_local_state_prefs.h"
#include "presearch/browser/presearch_profile_prefs.h"
#include "presearch/browser/search/ntp_utils.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_sync/presearch_sync_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "presearch/browser/widevine/widevine_utils.h"
#endif

#define MigrateObsoleteProfilePrefs MigrateObsoleteProfilePrefs_ChromiumImpl
#define MigrateObsoleteLocalStatePrefs \
    MigrateObsoleteLocalStatePrefs_ChromiumImpl
#include "../../../../../chrome/browser/prefs/browser_prefs.cc"
#undef MigrateObsoleteProfilePrefs
#undef MigrateObsoleteLocalStatePrefs

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "presearch/browser/gcm_driver/presearch_gcm_utils.h"
#endif

// This method should be periodically pruned of year+ old migrations.
void MigrateObsoleteProfilePrefs(Profile* profile) {
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Added 02/2020.
  // Must be called before ChromiumImpl because it's migrating a Chromium pref
  // to Presearch pref.
  gcm::MigrateGCMPrefs(profile);
#endif

  MigrateObsoleteProfilePrefs_ChromiumImpl(profile);

#if BUILDFLAG(ENABLE_WIDEVINE)
  // Added 11/2019.
  MigrateWidevinePrefs(profile);
#endif
  presearch_sync::MigratePresearchSyncPrefs(profile->GetPrefs());

  // Added 12/2019.
  dark_mode::MigratePresearchDarkModePrefs(profile);

  // Added 9/2020
#if !defined(OS_ANDROID)
  new_tab_page::MigrateNewTabPagePrefs(profile);
#endif

  // Added 04/2021
  profile->GetPrefs()->ClearPref(kAlternativeSearchEngineProviderInTor);
}

// This method should be periodically pruned of year+ old migrations.
void MigrateObsoleteLocalStatePrefs(PrefService* local_state) {
  MigrateObsoleteLocalStatePrefs_ChromiumImpl(local_state);

#if BUILDFLAG(ENABLE_WIDEVINE)
  // Added 11/2020.
  MigrateObsoleteWidevineLocalStatePrefs(local_state);
#endif
}
