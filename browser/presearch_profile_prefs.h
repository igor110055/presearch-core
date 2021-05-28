/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_PRESEARCH_PROFILE_PREFS_H_
#define PRESEARCH_BROWSER_PRESEARCH_PRESEARCH_PROFILE_PREFS_H_

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace presearch {

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_PRESEARCH_PRESEARCH_PROFILE_PREFS_H_
