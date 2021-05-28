// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_H_

#define PRESEARCH_AVATAR_BUTTON_H friend class PresearchAvatarToolbarButton;
#define GetAvatarIcon virtual GetAvatarIcon
#define GetAvatarTooltipText virtual GetAvatarTooltipText
#define AvatarToolbarButtonDelegate PresearchAvatarToolbarButtonDelegate
#include "../../../../../../../chrome/browser/ui/views/profiles/avatar_toolbar_button.h"
#undef PRESEARCH_AVATAR_BUTTON_H
#undef GetAvatarIcon
#undef GetAvatarTooltipText
#undef AvatarToolbarButtonDelegate

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_H_

