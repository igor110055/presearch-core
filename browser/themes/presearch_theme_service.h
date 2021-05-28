/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_THEMES_PRESEARCH_THEME_SERVICE_H_
#define PRESEARCH_BROWSER_THEMES_PRESEARCH_THEME_SERVICE_H_

#include <memory>

#include "chrome/browser/themes/theme_service.h"

namespace extensions {
class PresearchThemeEventRouter;
}  // namespace extensions

class Profile;

class PresearchThemeService : public ThemeService {
 public:
  explicit PresearchThemeService(Profile* profile, const ThemeHelper& theme_helper);
  ~PresearchThemeService() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(PresearchThemeEventRouterBrowserTest, ThemeChangeTest);

  // Own |mock_router|.
  void SetPresearchThemeEventRouterForTesting(
      extensions::PresearchThemeEventRouter* mock_router);

  std::unique_ptr<extensions::PresearchThemeEventRouter> presearch_theme_event_router_;
};

#endif  // PRESEARCH_BROWSER_THEMES_PRESEARCH_THEME_SERVICE_H_
