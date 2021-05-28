/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/themes/presearch_dark_mode_utils.h"

#include "testing/gtest/include/gtest/gtest.h"

TEST(PresearchThemeServiceTest, GetPresearchThemeListTest) {
  dark_mode::SetUseSystemDarkModeEnabledForTest(true);
  base::Value list = dark_mode::GetPresearchDarkModeTypeList();
  EXPECT_EQ(3UL, list.GetList().size());

  dark_mode::SetUseSystemDarkModeEnabledForTest(false);
  list = dark_mode::GetPresearchDarkModeTypeList();
  EXPECT_EQ(2UL, list.GetList().size());
}
