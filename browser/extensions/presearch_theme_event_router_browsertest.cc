/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_theme_event_router.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/browser/themes/presearch_theme_service.h"
#include "presearch/common/pref_names.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "testing/gmock/include/gmock/gmock.h"

using PresearchThemeEventRouterBrowserTest = InProcessBrowserTest;

namespace extensions {

class MockPresearchThemeEventRouter : public PresearchThemeEventRouter {
 public:
  using PresearchThemeEventRouter::PresearchThemeEventRouter;
  ~MockPresearchThemeEventRouter() override {}

  MOCK_METHOD0(Notify, void());
};

}  // namespace extensions

IN_PROC_BROWSER_TEST_F(PresearchThemeEventRouterBrowserTest,
                       ThemeChangeTest) {
  dark_mode::SetPresearchDarkModeType(
      dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DARK);

  extensions::MockPresearchThemeEventRouter* mock_router =
      new extensions::MockPresearchThemeEventRouter(browser()->profile());
  PresearchThemeService* service = static_cast<PresearchThemeService*>(
      ThemeServiceFactory::GetForProfile(browser()->profile()));
  service->SetPresearchThemeEventRouterForTesting(mock_router);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetPresearchDarkModeType(
      dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_LIGHT);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetPresearchDarkModeType(
      dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DARK);

  EXPECT_CALL(*mock_router, Notify()).Times(0);
  dark_mode::SetPresearchDarkModeType(
      dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DARK);
}
