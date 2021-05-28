/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/presearch_theme_api.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/pref_names.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/browser/extensions/extension_function_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/common/extension_builder.h"

using extensions::api::PresearchThemeGetPresearchThemeTypeFunction;
using extension_function_test_utils::RunFunctionAndReturnSingleResult;

class PresearchThemeAPIBrowserTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    extension_ = extensions::ExtensionBuilder("Test").Build();
  }

  scoped_refptr<const extensions::Extension> extension() {
    return extension_;
  }

 private:
  scoped_refptr<const extensions::Extension> extension_;
};

IN_PROC_BROWSER_TEST_F(PresearchThemeAPIBrowserTest,
                       PresearchThemeGetPresearchThemeTypeTest) {
  // Change to Light type and check it from api.
  dark_mode::SetPresearchDarkModeType(
      dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_LIGHT);
  EXPECT_EQ(dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_LIGHT,
            dark_mode::GetActivePresearchDarkModeType());

  scoped_refptr<PresearchThemeGetPresearchThemeTypeFunction> get_function(
      new PresearchThemeGetPresearchThemeTypeFunction());
  get_function->set_extension(extension().get());
  std::unique_ptr<base::Value> value;
  value.reset(RunFunctionAndReturnSingleResult(get_function.get(),
                                               std::string("[]"),
                                               browser()));
  EXPECT_EQ(value->GetString(), "Light");
}
