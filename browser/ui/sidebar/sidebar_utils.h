/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_SIDEBAR_SIDEBAR_UTILS_H_
#define PRESEARCH_BROWSER_UI_SIDEBAR_SIDEBAR_UTILS_H_

class Browser;
class Profile;

namespace sidebar {

bool CanUseSidebar(Profile* profile);
bool CanAddCurrentActiveTabToSidebar(Browser* browser);

}  // namespace sidebar

#endif  // PRESEARCH_BROWSER_UI_SIDEBAR_SIDEBAR_UTILS_H_
