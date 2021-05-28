/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_sync/presearch_sync_prefs.h"

void PresearchRegisterBrowserStatePrefs(PrefRegistrySimple* registry) {
  presearch_sync::Prefs::RegisterProfilePrefs(registry);
}

#define PRESEARCH_REGISTER_BROWSER_STATE_PREFS PresearchRegisterBrowserStatePrefs(registry);
#include "../../../../../../ios/chrome/browser/prefs/browser_prefs.mm"
