/*  Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/macros.h"
#include "presearch.components/presearch_shields/browser/presearch_shields_util.h"
#include "presearch.components/presearch_shields/common/presearch_shield_constants.h"
#include "presearch.components/presearch_shields/common/features.h"
#include "build/build_config.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

using presearch_shields::ControlType;
using presearch_shields::ControlTypeFromString;
using presearch_shields::ControlTypeToString;
using presearch_shields::GetPatternFromURL;
using presearch_shields::features::kPresearchDomainBlock;

class PresearchShieldsUtilTest : public testing::Test {
 public:
  PresearchShieldsUtilTest() = default;
  ~PresearchShieldsUtilTest() override = default;

  void SetUp() override { profile_ = std::make_unique<TestingProfile>(); }

  TestingProfile* profile() { return profile_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;

  DISALLOW_COPY_AND_ASSIGN(PresearchShieldsUtilTest);
};

class PresearchShieldsUtilDomainBlockFeatureTest : public PresearchShieldsUtilTest {
 public:
  PresearchShieldsUtilDomainBlockFeatureTest() {
    feature_list_.InitAndDisableFeature(kPresearchDomainBlock);
  }

 private:
  base::test::ScopedFeatureList feature_list_;
};

TEST_F(PresearchShieldsUtilTest, GetPatternFromURL) {
  // wildcard
  auto pattern = GetPatternFromURL(GURL());
  EXPECT_EQ(ContentSettingsPattern::Wildcard(), pattern);

  // scheme is a wildcard, should match any scheme
  pattern = GetPatternFromURL(GURL("http://presearch.org"));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org/path2")));
  EXPECT_TRUE(pattern.Matches(GURL("https://presearch.org")));
  EXPECT_TRUE(pattern.Matches(GURL("ftp://presearch.org")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.presearch.org")));
  EXPECT_FALSE(pattern.Matches(GURL("http://presearch2.com")));

  // path is a wildcard
  pattern = GetPatternFromURL(GURL("http://presearch.org/path1"));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org/path2")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.presearch.org")));
  EXPECT_FALSE(pattern.Matches(GURL("http://presearch2.com")));

  // port is a wildcard
  pattern = GetPatternFromURL(GURL("http://presearch.org:8080"));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org:8080")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org:8080/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org:8080/path2")));
  EXPECT_TRUE(pattern.Matches(GURL("http://presearch.org:5555")));
  EXPECT_TRUE(pattern.Matches(GURL("https://presearch.org")));
  EXPECT_TRUE(pattern.Matches(GURL("https://presearch.org:8080")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.presearch.org")));
  EXPECT_FALSE(pattern.Matches(GURL("http://presearch2.com")));

  // with implied port
  pattern = GetPatternFromURL(GURL("https://brianbondy.com"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
  pattern = GetPatternFromURL(GURL("http://brianbondy.com"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
  // with specified port
  pattern = GetPatternFromURL(GURL("http://brianbondy.com:8080"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
}

TEST_F(PresearchShieldsUtilTest, ControlTypeToString) {
  EXPECT_EQ("block", ControlTypeToString(ControlType::BLOCK));
  EXPECT_EQ("allow", ControlTypeToString(ControlType::ALLOW));
  EXPECT_EQ("block_third_party",
            ControlTypeToString(ControlType::BLOCK_THIRD_PARTY));
}

TEST_F(PresearchShieldsUtilTest, ControlTypeFromString) {
  EXPECT_EQ(ControlType::BLOCK, ControlTypeFromString("block"));
  EXPECT_EQ(ControlType::ALLOW, ControlTypeFromString("allow"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY,
            ControlTypeFromString("block_third_party"));
}

/* PRESEARCH_SHIELDS CONTROL */
TEST_F(PresearchShieldsUtilTest, SetPresearchShieldsEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  presearch_shields::SetPresearchShieldsEnabled(map, true, GURL("http://presearch.org"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                        ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to different scheme
  setting = map->GetContentSetting(GURL("https://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(PresearchShieldsUtilTest, SetPresearchShieldsEnabled_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);
  presearch_shields::SetPresearchShieldsEnabled(map, ControlType::ALLOW,
                                        GURL("chrome://preferences"));
  setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = presearch_shields::GetPresearchShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
  presearch_shields::SetPresearchShieldsEnabled(map, ControlType::ALLOW,
                                        GURL("about:blank"));
  setting = presearch_shields::GetPresearchShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

TEST_F(PresearchShieldsUtilTest, GetPresearchShieldsEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetPresearchShieldsEnabled(map, GURL());
  EXPECT_EQ(true, setting);
  setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("http://presearch.org"));
  EXPECT_EQ(true, setting);
  setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("https://presearch.org"));
  EXPECT_EQ(true, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::PRESEARCH_SHIELDS,
      CONTENT_SETTING_BLOCK);
  setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("http://presearch.org/*"));
  EXPECT_EQ(false, setting);
  // https in unchanged
  setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("https://presearch.org"));
  EXPECT_EQ(true, setting);
  // default is unchanged
  setting = presearch_shields::GetPresearchShieldsEnabled(map, GURL());
  EXPECT_EQ(true, setting);
}

TEST_F(PresearchShieldsUtilTest, GetPresearchShieldsEnabled_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      presearch_shields::GetPresearchShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = presearch_shields::GetPresearchShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

/* AD CONTROL */
TEST_F(PresearchShieldsUtilTest, SetAdControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);

  /* ALLOW */
  presearch_shields::SetAdControlType(map, ControlType::ALLOW, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  presearch_shields::SetAdControlType(map, ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(PresearchShieldsUtilTest, SetAdControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  presearch_shields::SetAdControlType(map, ControlType::ALLOW,
                                  GURL("http://presearch.org"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                        ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::PRESEARCH_ADS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(PresearchShieldsUtilTest, GetAdControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_ADS, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_ADS, CONTENT_SETTING_BLOCK);
  setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
}

TEST_F(PresearchShieldsUtilTest, GetAdControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = presearch_shields::GetAdControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = presearch_shields::GetAdControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::PRESEARCH_ADS,
      CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetAdControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting = presearch_shields::GetAdControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // default is unchanged
  setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* BLOCK */
  // change default to allow
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_ADS, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetAdControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = presearch_shields::GetAdControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::PRESEARCH_ADS,
      CONTENT_SETTING_BLOCK);
  setting = presearch_shields::GetAdControlType(map, GURL("http://presearch.org/*"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // https in unchanged
  setting = presearch_shields::GetAdControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  // default is unchanged
  setting = presearch_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
}

/* COOKIE CONTROL */
TEST_F(PresearchShieldsUtilTest, SetCookieControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // setting should be default to start with
  auto setting = map->GetContentSetting(GURL(), GURL(),
                                        ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  setting = map->GetContentSetting(GURL("http://presearch.org"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);

  /* ALLOW */
  presearch_shields::SetCookieControlType(map, ControlType::ALLOW, GURL());
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("http://presearch.org"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  presearch_shields::SetCookieControlType(map, ControlType::BLOCK, GURL());
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL("http://presearch.org"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* BLOCK_THIRD_PARTY */
  presearch_shields::SetCookieControlType(map, ControlType::BLOCK_THIRD_PARTY,
                                      GURL());
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL("http://presearch.org"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(PresearchShieldsUtilTest, SetCookieControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  presearch_shields::SetCookieControlType(map, ControlType::ALLOW,
                                      GURL("http://presearch.org"));
  // override should apply to origin
  auto setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                        ContentSettingsType::PRESEARCH_COOKIES);
  setting = map->GetContentSetting(GURL("http://presearch.org"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://presearch.org"), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("https://presearch.org"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should not apply to default
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  // override should not apply to default
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::PRESEARCH_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(PresearchShieldsUtilTest, GetCookieControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_ALLOW);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_BLOCK);
  setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* BLOCK_THIRD_PARTY */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
}

TEST_F(PresearchShieldsUtilTest, GetCookieControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting =
      presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::PRESEARCH_COOKIES,
      CONTENT_SETTING_ALLOW);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::PRESEARCH_COOKIES,
      CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_BLOCK);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* BLOCK_THIRD_PARTY */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::PRESEARCH_COOKIES,
      CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::PRESEARCH_COOKIES, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetCookieControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = presearch_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
}

/* FINGERPRINTING CONTROL */
TEST_F(PresearchShieldsUtilTest, SetFingerprintingControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // setting should be default to start with
  auto type = presearch_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* ALLOW */
  presearch_shields::SetFingerprintingControlType(map, ControlType::ALLOW, GURL());
  type = presearch_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, type);

  // setting should apply to all urls
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, type);

  /* BLOCK */
  presearch_shields::SetFingerprintingControlType(map, ControlType::BLOCK, GURL());
  type = presearch_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, type);

  // setting should apply to all urls
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, type);

  /* DEFAULT */
  presearch_shields::SetFingerprintingControlType(map, ControlType::DEFAULT,
                                              GURL());
  type = presearch_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);

  // setting should apply to all urls
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* Global ALLOW and Site explicit DEFAULT */
  presearch_shields::SetFingerprintingControlType(map, ControlType::ALLOW, GURL());
  presearch_shields::SetFingerprintingControlType(map, ControlType::DEFAULT,
                                              GURL("http://presearch.org"));
  // Site should have DEFAULT if it's explicitly set.
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* Global BLOCK and Site explicit DEFAULT */
  presearch_shields::SetFingerprintingControlType(map, ControlType::BLOCK, GURL());
  // Site should have DEFAULT if it's explicitly set.
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::DEFAULT, type);
}

TEST_F(PresearchShieldsUtilTest, SetFingerprintingControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  presearch_shields::SetFingerprintingControlType(map, ControlType::ALLOW,
                                              GURL("http://presearch.org"));
  auto type = presearch_shields::GetFingerprintingControlType(
      map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, type);
  // override should also apply to different scheme
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, type);

  presearch_shields::SetFingerprintingControlType(map, ControlType::BLOCK,
                                              GURL("http://presearch.org"));
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, type);
  // override should also apply to different scheme
  type = presearch_shields::GetFingerprintingControlType(map,
                                                     GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, type);

  // override should not apply to default
  type = presearch_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);
}

/* HTTPSEVERYWHERE CONTROL */
TEST_F(PresearchShieldsUtilTest, SetHTTPSEverywhereEnabled_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(
      GURL("http://presearch.org"), GURL(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);

  /* disabled */
  presearch_shields::SetHTTPSEverywhereEnabled(map, false, GURL());
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(
      GURL("http://presearch.org"), GURL(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* enabled */
  presearch_shields::SetHTTPSEverywhereEnabled(map, true, GURL());
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(
      GURL("http://presearch.org"), GURL(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(PresearchShieldsUtilTest, SetHTTPSEverywhereEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  presearch_shields::SetHTTPSEverywhereEnabled(map, false,
                                           GURL("http://presearch.org"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(
      GURL("http://presearch.org"), GURL(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to different scheme
  setting = map->GetContentSetting(
      GURL("https://presearch.org"), GURL(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(PresearchShieldsUtilTest, GetHTTPSEverywhereEnabled_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(false, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_BLOCK);
  setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);
}

TEST_F(PresearchShieldsUtilTest, GetHTTPSEverywhereEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("http://presearch.org"));
  EXPECT_EQ(true, setting);
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("https://presearch.org"));
  EXPECT_EQ(true, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("https://presearch.org"));
  EXPECT_EQ(true, setting);
  // default is unchanged
  setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);

  /* BLOCK */
  // change default to allow
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(),
      ContentSettingsType::PRESEARCH_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_BLOCK);
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("http://presearch.org/*"));
  EXPECT_EQ(true, setting);
  // https in unchanged
  setting =
      presearch_shields::GetHTTPSEverywhereEnabled(map, GURL("https://presearch.org"));
  EXPECT_EQ(false, setting);
  // default is unchanged
  setting = presearch_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(false, setting);
}

/* NOSCRIPT CONTROL */
TEST_F(PresearchShieldsUtilTest, SetNoScriptControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  presearch_shields::SetNoScriptControlType(map, ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* ALLOW */
  presearch_shields::SetNoScriptControlType(map, ControlType::ALLOW, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(PresearchShieldsUtilTest, SetNoScriptControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  presearch_shields::SetNoScriptControlType(map, ControlType::BLOCK,
                                        GURL("http://presearch.org"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://presearch.org"), GURL(),
                                        ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://presearch.org"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should not apply to default
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(PresearchShieldsUtilTest, GetNoScriptControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_BLOCK);
  setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_ALLOW);
  setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
}

TEST_F(PresearchShieldsUtilTest, GetNoScriptControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::JAVASCRIPT,
      CONTENT_SETTING_BLOCK);
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("http://presearch.org/*"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // https in unchanged
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  // default is unchanged
  setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* ALLOW */
  // change default to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_BLOCK);
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://presearch.org/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::JAVASCRIPT,
      CONTENT_SETTING_ALLOW);
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("http://presearch.org"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting =
      presearch_shields::GetNoScriptControlType(map, GURL("https://presearch.org"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // default is unchanged
  setting = presearch_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
}

// Should not do domain blocking if domain blocking feature is disabled
TEST_F(PresearchShieldsUtilDomainBlockFeatureTest, ShouldDoDomainBlocking) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      presearch_shields::ShouldDoDomainBlocking(map, GURL("https://presearch.org"));
  EXPECT_EQ(false, setting);
}

// Should not do domain blocking if Presearch Shields is down
TEST_F(PresearchShieldsUtilTest, ShouldDoDomainBlocking_ShieldsDown) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  GURL url = GURL("https://presearch.org");
  presearch_shields::SetPresearchShieldsEnabled(map, false, url);
  auto setting = presearch_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(false, setting);
}

// Should not do domain blocking on non-HTTP(S) URLs
TEST_F(PresearchShieldsUtilTest, ShouldDoDomainBlocking_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      presearch_shields::ShouldDoDomainBlocking(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = presearch_shields::ShouldDoDomainBlocking(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

// Should not do domain blocking unless ad blocking is "aggressive"
TEST_F(PresearchShieldsUtilTest, ShouldDoDomainBlocking_ControlTypes) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  GURL url = GURL("https://presearch.org");

  presearch_shields::SetAdControlType(map, ControlType::ALLOW, url);
  auto setting = presearch_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(false, setting);

  presearch_shields::SetAdControlType(map, ControlType::BLOCK, url);
  setting = presearch_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(false, setting);

  presearch_shields::SetCosmeticFilteringControlType(map, ControlType::BLOCK, url);
  setting = presearch_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(true, setting);
}
