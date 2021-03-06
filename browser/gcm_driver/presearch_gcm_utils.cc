/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/gcm_driver/presearch_gcm_utils.h"

#include "base/values.h"
#include "presearch/common/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "components/pref_registry/pref_registry_syncable.h"

namespace gcm {

// Chromium pref deprecated as of 01/2020.
const char kGCMChannelStatus[] = "gcm.channel_status";

void RegisterGCMProfilePrefs(user_prefs::PrefRegistrySyncable* registry) {
  // Deprecated Chromium pref.
  registry->SetDefaultPrefValue(kGCMChannelStatus, base::Value(false));
  // Current Presearch eqiuvalent of the deprecated pref.
  registry->RegisterBooleanPref(kPresearchGCMChannelStatus, false);
}

void MigrateGCMPrefs(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  // The default was false (see above).
  auto* pref = prefs->FindPreference(kGCMChannelStatus);
  if (pref && !pref->IsDefaultValue())
    prefs->SetBoolean(kPresearchGCMChannelStatus, pref->GetValue()->GetBool());
}

}  // namespace gcm
