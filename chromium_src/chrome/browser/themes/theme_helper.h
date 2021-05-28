/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_H_

#define PRESEARCH_THEME_HELPER_H_    \
 private:                        \
  friend class PresearchThemeHelper; \
                                 \
 public:
#define GetOmniboxColor virtual GetOmniboxColor
#include "../../../../../chrome/browser/themes/theme_helper.h"
#undef GetOmniboxColor
#undef PRESEARCH_THEME_HELPER_H_

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_H_
