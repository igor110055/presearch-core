/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_GCM_DRIVER_PRESEARCH_GCM_UTILS_H_
#define PRESEARCH_BROWSER_GCM_DRIVER_PRESEARCH_GCM_UTILS_H_

class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace gcm {

void RegisterGCMProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
void MigrateGCMPrefs(Profile* profile);

}  // namespace gcm

#endif  // PRESEARCH_BROWSER_GCM_DRIVER_PRESEARCH_GCM_UTILS_H_
