/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "testing/gtest/include/gtest/gtest.h"

#include "presearch/components/l10n/browser/locale.h"

// npm run test -- presearch_unit_tests --filter=PresearchAds*


namespace ads {

class PresearchAdsLocaleTest : public ::testing::Test {
 protected:
  PresearchAdsLocaleTest() {
    // You can do set-up work for each test here
  }

  ~PresearchAdsLocaleTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }
};

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForEnglish) {
  // Arrange
  const std::string locale = "en";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForEnglish) {
  // Arrange
  const std::string locale = "en";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeDashCountryCode) {
  // Arrange
  const std::string locale = "en-US";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeDashCountryCode) {
  // Arrange
  const std::string locale = "en-US";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeDashWorld) {
  // Arrange
  const std::string locale = "en-101";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeDashWorld) {
  // Arrange
  const std::string locale = "en-101";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "101";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeDashCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en-US.UTF-8";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeDashCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en-US.UTF-8";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeDashScriptDashCountryCode) {
  // Arrange
  const std::string locale = "az-Latn-AZ";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "az";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeDashScriptDashCountryCode) {
  // Arrange
  const std::string locale = "az-Latn-AZ";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "AZ";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "en_US";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "en_US";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeUnderscoreCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en_US.UTF-8";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "en";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeUnderscoreCountryCodeDotEncoding) {
  // Arrange
  const std::string locale = "en_US.UTF-8";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "US";
  EXPECT_EQ(expected_country_code, country_code);
}

TEST_F(PresearchAdsLocaleTest,
    LanguageCodeForLanguageCodeUnderscoreScriptUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "az_Latn_AZ";

  // Act
  const std::string language_code =
      presearch_l10n::Locale::GetLanguageCode(locale);

  // Assert
  const std::string expected_language_code = "az";
  EXPECT_EQ(expected_language_code, language_code);
}

TEST_F(PresearchAdsLocaleTest,
    CountryCodeForLanguageCodeUnderscoreScriptUnderscoreCountryCode) {
  // Arrange
  const std::string locale = "az_Latn_AZ";

  // Act
  const std::string country_code =
      presearch_l10n::Locale::GetCountryCode(locale);

  // Assert
  const std::string expected_country_code = "AZ";
  EXPECT_EQ(expected_country_code, country_code);
}

}  // namespace ads
