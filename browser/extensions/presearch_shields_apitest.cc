/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "presearch/common/presearch_paths.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "content/public/test/browser_test.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {

class PresearchShieldsExtensionApiTest : public ExtensionApiTest {
 public:
  void SetUp() override {
    presearch::RegisterPathProvider();
    base::PathService::Get(presearch::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }
  void TearDown() override {
    ExtensionApiTest::TearDown();
  }
  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(PresearchShieldsExtensionApiTest, PresearchExtensionHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShields"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchShieldsExtensionApiTest,
    NotPresearchExtensionHasNoAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("notPresearchShields"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

}  // namespace
}  // namespace extensions
