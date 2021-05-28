/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_

#define PRESEARCH_PROFILE_MENU_VIEW_H    \
  friend class PresearchProfileMenuView; \

#define OnExitProfileButtonClicked virtual OnExitProfileButtonClicked
#define BuildAutofillButtons virtual BuildAutofillButtons
#define BuildIdentity virtual BuildIdentity
#define BuildSyncInfo virtual BuildSyncInfo
#define BuildFeatureButtons virtual BuildFeatureButtons

#include "../../../../../../../chrome/browser/ui/views/profiles/profile_menu_view.h"
#undef BuildFeatureButtons
#undef BuildSyncInfo
#undef BuildIdentity
#undef BuildAutofillButtons
#undef OnExitProfileButtonClicked
#undef PRESEARCH_PROFILE_MENU_VIEW_H

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_
