// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/installer/mini_installer/mini_installer.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace mini_installer {

class PresearchMiniInstallerTest: public testing::Test {
 public:
  PresearchMiniInstallerTest() {
  }
  ~PresearchMiniInstallerTest() override {}
};


TEST_F(PresearchMiniInstallerTest, HasNoReferralCode) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup.exe", &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasStandardReferralCode) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"PresearchBrowserSetup-FOO123.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(PresearchMiniInstallerTest, HasStandardReferralCodeWithLowercase) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"PresearchBrowserSetup-foo123.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(PresearchMiniInstallerTest, HasStandardReferralCodeWithPath) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"c:/foo/bar/PresearchBrowserSetup-FOO123.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(PresearchMiniInstallerTest,
                HasStandardReferralCodeWithDeduplicatingSuffix) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"c:/foo/bar/PresearchBrowserSetup-FOO123 (1).exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(PresearchMiniInstallerTest,
                HasStandardReferralCodeWithDeduplicatingSuffixNoSpaces) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"c:/foo/bar/PresearchBrowserSetup-FOO123(1).exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(PresearchMiniInstallerTest,
                HasStandardReferralCodeWithDeduplicatingSuffixExtraSpaces) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(
                          L"c:/foo/bar/PresearchBrowserSetup-FOO123   (1).exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeReversed) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-123FOO.exe",
                          &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeNoDigits) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-FOO.exe", &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeNoLetters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-123.exe", &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeTooManyDigits) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-FOO1234.exe",
                          &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeTooFewDigits) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-FOO12.exe",
                          &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeTooManyLetters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-FOOO123.exe",
                          &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidStandardReferralCodeTooFewLetters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-FO123.exe",
                          &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasExtendedReferralCode) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"PresearchBrowserSetup-extended-code.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"extended-code");
}

TEST_F(PresearchMiniInstallerTest,
                HasInvalidExtendedReferralCodeNonAlphabeticCharacters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(
                          L"PresearchBrowserSetup-invalid-extended-c0de.exe",
                          &referral_code));
}

TEST_F(PresearchMiniInstallerTest, HasInvalidExtendedReferralCodeTooFewWords) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"PresearchBrowserSetup-invalidextendedcode.exe",
                          &referral_code));
}

}  // namespace mini_installer
