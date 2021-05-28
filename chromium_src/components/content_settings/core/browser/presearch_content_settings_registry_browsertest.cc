/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_shields/common/presearch_shield_constants.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "content/public/test/browser_test.h"

const GURL& GetPresearchURL() {
  static const GURL kPresearchURL("https://www.presearch.com");
  return kPresearchURL;
}

class PresearchContentSettingsRegistryBrowserTest : public InProcessBrowserTest {
 public:
  using InProcessBrowserTest::InProcessBrowserTest;

  HostContentSettingsMap* content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }

  HostContentSettingsMap* private_content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(
        browser()->profile()->GetPrimaryOTRProfile());
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchContentSettingsRegistryBrowserTest);
};

IN_PROC_BROWSER_TEST_F(PresearchContentSettingsRegistryBrowserTest,
                       WithoutWildcardContentSetting) {
  ContentSetting presearch_url_shields_setting =
      content_settings()->GetContentSetting(GetPresearchURL(), GetPresearchURL(),
                                            ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, presearch_url_shields_setting);

  ContentSetting presearch_url_shields_setting_private =
      private_content_settings()->GetContentSetting(
          GetPresearchURL(), GetPresearchURL(), ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, presearch_url_shields_setting_private);
}

IN_PROC_BROWSER_TEST_F(PresearchContentSettingsRegistryBrowserTest,
                       WithPresearchShieldsContentSetting) {
  ContentSettingsPattern presearch_url_pattern =
      ContentSettingsPattern::FromURL(GetPresearchURL());

  content_settings()->SetContentSettingCustomScope(
      presearch_url_pattern, presearch_url_pattern, ContentSettingsType::PRESEARCH_SHIELDS,
      CONTENT_SETTING_ALLOW);

  ContentSetting presearch_url_shields_setting =
      content_settings()->GetContentSetting(GetPresearchURL(), GetPresearchURL(),
                                            ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, presearch_url_shields_setting);

  ContentSetting presearch_url_shields_setting_private =
      private_content_settings()->GetContentSetting(
          GetPresearchURL(), GetPresearchURL(), ContentSettingsType::PRESEARCH_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, presearch_url_shields_setting_private);
}
