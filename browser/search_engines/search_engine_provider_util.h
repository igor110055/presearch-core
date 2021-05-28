/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_SEARCH_ENGINES_SEARCH_ENGINE_PROVIDER_UTIL_H_
#define PRESEARCH_BROWSER_SEARCH_ENGINES_SEARCH_ENGINE_PROVIDER_UTIL_H_

class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace presearch {

bool UseAlternativeSearchEngineProviderEnabled(Profile* profile);

void ToggleUseAlternativeSearchEngineProvider(Profile* profile);

bool IsRegionForQwant(Profile* profile);

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_SEARCH_ENGINES_SEARCH_ENGINE_PROVIDER_UTIL_H_
