/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/devtools/presearch_devtools_ui_bindings.h"

#include <string>

#include "base/bind.h"
#include "base/values.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/pref_names.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "components/prefs/pref_service.h"

class PresearchDevToolsUIBindingsBrowserTest : public InProcessBrowserTest {
 public:
  void GetPreferenceCallback(const base::Value* value) {
    ui_theme_ = value->FindKey("uiTheme")->GetString();
  }

  std::string ui_theme_;
};

IN_PROC_BROWSER_TEST_F(PresearchDevToolsUIBindingsBrowserTest, ThemeTest) {
  auto* tab_strip_model = browser()->tab_strip_model();
  content::WebContents* web_contents = tab_strip_model->GetActiveWebContents();
  DCHECK(web_contents);

  dark_mode::SetPresearchDarkModeType(
      dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DARK);
  auto* devtools_ui_bindings = new PresearchDevToolsUIBindings(web_contents);
  DCHECK(devtools_ui_bindings);
  devtools_ui_bindings->GetPreferences(
      base::Bind(&PresearchDevToolsUIBindingsBrowserTest::GetPreferenceCallback,
                 base::Unretained(this)));
  // Check current devtools' theme is same as native theme when user doesn't
  // change devtools' theme explicitely.
  EXPECT_EQ(ui_theme_, "\"dark\"");

  dark_mode::SetPresearchDarkModeType(
    dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_LIGHT);
  devtools_ui_bindings->GetPreferences(
      base::Bind(&PresearchDevToolsUIBindingsBrowserTest::GetPreferenceCallback,
                 base::Unretained(this)));
  // In devtools, default is used as light.
  EXPECT_EQ(ui_theme_, "\"default\"");

  // When user sets devtools' theme explicitely, respect user's setting.
  devtools_ui_bindings->SetPreference("uiTheme", "\"dark\"");
  devtools_ui_bindings->GetPreferences(
      base::Bind(&PresearchDevToolsUIBindingsBrowserTest::GetPreferenceCallback,
                 base::Unretained(this)));
  EXPECT_EQ(ui_theme_, "\"dark\"");
}
