/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "presearch/browser/presearch_stats/presearch_stats_updater.h"

#include "base/files/scoped_temp_dir.h"
#include "base/system/sys_info.h"
#include "base/time/time.h"
#include "bat/ads/pref_names.h"
#include "presearch/browser/presearch_stats/presearch_stats_updater_params.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_ads/browser/test_util.h"
#include "presearch/components/presearch_referrals/browser/presearch_referrals_service.h"
#include "presearch/components/presearch_stats/browser/presearch_stats_updater_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- presearch_unit_tests --filter=PresearchStatsUpdaterTest.*

const char kYesterday[] = "2018-06-21";
const char kToday[] = "2018-06-22";
const char kTomorrow[] = "2018-06-23";

const int kLastWeek = 24;
const int kThisWeek = 25;
const int kNextWeek = 26;

const int kLastMonth = 5;
const int kThisMonth = 6;
const int kNextMonth = 7;

class PresearchStatsUpdaterTest : public testing::Test {
 public:
  PresearchStatsUpdaterTest()
      : profile_manager_(TestingBrowserProcess::GetGlobal()) {}
  ~PresearchStatsUpdaterTest() override {}

  void SetUp() override {
    EXPECT_TRUE(temp_dir_.CreateUniqueTempDir());
    EXPECT_TRUE(profile_manager_.SetUp());
    profile_ = presearch_ads::CreatePresearchAdsProfile(temp_dir_.GetPath());
    EXPECT_TRUE(profile_.get() != NULL);
    presearch_stats::RegisterLocalStatePrefs(testing_local_state_.registry());
    presearch::RegisterPrefsForPresearchReferralsService(
        testing_local_state_.registry());
    presearch_stats::PresearchStatsUpdaterParams::SetFirstRunForTest(true);
  }

  void TearDown() override { profile_.reset(); }

  PrefService* GetLocalState() { return &testing_local_state_; }
  PrefService* GetProfilePrefs() { return profile_->GetPrefs(); }
  void SetEnableAds(bool ads_enabled) {
    GetProfilePrefs()->SetBoolean(ads::prefs::kEnabled, ads_enabled);
  }

  void SetCurrentTimeForTest(const base::Time& current_time) {
    presearch_stats::PresearchStatsUpdaterParams::SetCurrentTimeForTest(current_time);
  }

 private:
  TestingPrefServiceSimple testing_local_state_;
  TestingProfileManager profile_manager_;
  std::unique_ptr<Profile> profile_;
  base::ScopedTempDir temp_dir_;
  content::BrowserTaskEnvironment task_environment_;
};

TEST_F(PresearchStatsUpdaterTest, IsDailyUpdateNeededLastCheckedYesterday) {
  GetLocalState()->SetString(kLastCheckYMD, kYesterday);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetDailyParam(), "true");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetString(kLastCheckYMD), kToday);
}

TEST_F(PresearchStatsUpdaterTest, IsDailyUpdateNeededLastCheckedToday) {
  GetLocalState()->SetString(kLastCheckYMD, kToday);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetDailyParam(), "false");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetString(kLastCheckYMD), kToday);
}

TEST_F(PresearchStatsUpdaterTest, IsDailyUpdateNeededLastCheckedTomorrow) {
  GetLocalState()->SetString(kLastCheckYMD, kTomorrow);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetDailyParam(), "false");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetString(kLastCheckYMD), kToday);
}

TEST_F(PresearchStatsUpdaterTest, IsWeeklyUpdateNeededLastCheckedLastWeek) {
  GetLocalState()->SetInteger(kLastCheckWOY, kLastWeek);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetWeeklyParam(), "true");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), kThisWeek);
}

TEST_F(PresearchStatsUpdaterTest, IsWeeklyUpdateNeededLastCheckedThisWeek) {
  GetLocalState()->SetInteger(kLastCheckWOY, kThisWeek);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetWeeklyParam(), "false");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), kThisWeek);
}

TEST_F(PresearchStatsUpdaterTest, IsWeeklyUpdateNeededLastCheckedNextWeek) {
  GetLocalState()->SetInteger(kLastCheckWOY, kNextWeek);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetWeeklyParam(), "true");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), kThisWeek);
}

TEST_F(PresearchStatsUpdaterTest, IsMonthlyUpdateNeededLastCheckedLastMonth) {
  GetLocalState()->SetInteger(kLastCheckMonth, kLastMonth);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetMonthlyParam(), "true");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckMonth), kThisMonth);
}

TEST_F(PresearchStatsUpdaterTest, IsMonthlyUpdateNeededLastCheckedThisMonth) {
  GetLocalState()->SetInteger(kLastCheckMonth, kThisMonth);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetMonthlyParam(), "false");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckMonth), kThisMonth);
}

TEST_F(PresearchStatsUpdaterTest, IsMonthlyUpdateNeededLastCheckedNextMonth) {
  GetLocalState()->SetInteger(kLastCheckMonth, kNextMonth);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetMonthlyParam(), "true");
  presearch_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckMonth), kThisMonth);
}

TEST_F(PresearchStatsUpdaterTest, HasAdsDisabled) {
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetEnableAds(false);
  EXPECT_EQ(presearch_stats_updater_params.GetAdsEnabledParam(), "false");
}

TEST_F(PresearchStatsUpdaterTest, HasAdsEnabled) {
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetEnableAds(true);
  EXPECT_EQ(presearch_stats_updater_params.GetAdsEnabledParam(), "true");
}

TEST_F(PresearchStatsUpdaterTest, HasArchSkip) {
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetProcessArchParam(), "");
}

TEST_F(PresearchStatsUpdaterTest, HasArchVirt) {
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchVirt,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetProcessArchParam(), "virt");
}

TEST_F(PresearchStatsUpdaterTest, HasArchMetal) {
  auto arch = base::SysInfo::OperatingSystemArchitecture();
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchMetal,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetProcessArchParam(), arch);
}

TEST_F(PresearchStatsUpdaterTest, HasDateOfInstallationFirstRun) {
  base::Time::Exploded exploded;
  base::Time current_time;

  // Set date to 2018-11-04 (ISO week #44)
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 0;
  exploded.year = 2018;
  exploded.month = 11;
  exploded.day_of_month = 4;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
  SetCurrentTimeForTest(current_time);

  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(presearch_stats_updater_params.GetDateOfInstallationParam(),
            "2018-11-04");
}

TEST_F(PresearchStatsUpdaterTest, HasDailyRetention) {
  base::Time::Exploded exploded;
  base::Time current_time, dtoi_time;

  // Set date to 2018-11-04
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 0;
  exploded.year = 2018;
  exploded.month = 11;
  exploded.day_of_month = 4;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &dtoi_time));
  // Make first run date 6 days earlier (still within 14 day window)
  exploded.day_of_month = 10;
  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

  SetCurrentTimeForTest(dtoi_time);
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetCurrentTimeForTest(current_time);
  EXPECT_EQ(presearch_stats_updater_params.GetDateOfInstallationParam(),
            "2018-11-04");
}

TEST_F(PresearchStatsUpdaterTest, HasDailyRetentionExpiration) {
  base::Time::Exploded exploded;
  base::Time current_time, dtoi_time;

  // Set date to 2018-11-04
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 0;
  exploded.year = 2018;
  exploded.month = 11;
  exploded.day_of_month = 4;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &dtoi_time));
  // Make first run date 14 days earlier (outside 14 day window)
  exploded.day_of_month = 18;
  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

  SetCurrentTimeForTest(dtoi_time);
  presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), presearch_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetCurrentTimeForTest(current_time);
  EXPECT_EQ(presearch_stats_updater_params.GetDateOfInstallationParam(), "null");
}

// This test ensures that our weekly stats cut over on Monday
TEST_F(PresearchStatsUpdaterTest, IsWeeklyUpdateNeededOnMondayLastCheckedOnSunday) {
  base::Time::Exploded exploded;
  base::Time current_time;

  {
    // Set our local state to indicate that the last weekly check was
    // performed during ISO week #43
    GetLocalState()->SetInteger(kLastCheckWOY, 43);

    // Set date to 2018-11-04 (ISO week #44)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2018;
    exploded.month = 11;
    exploded.day_of_month = 4;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

    SetCurrentTimeForTest(current_time);
    presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        presearch_stats::ProcessArch::kArchSkip);

    // Make sure that the weekly param was set to true, since this is
    // a new ISO week (#44)
    EXPECT_EQ(presearch_stats_updater_params.GetWeeklyParam(), "true");
    presearch_stats_updater_params.SavePrefs();

    // Make sure that local state was updated to reflect this as well
    EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), 44);
  }

  {
    // Now it's the next day (Monday)
    exploded.day_of_week = 1;
    exploded.day_of_month = 5;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

    SetCurrentTimeForTest(current_time);
    presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        presearch_stats::ProcessArch::kArchSkip);

    // Make sure that the weekly param was set to true, since this is
    // a new ISO week (#45)
    EXPECT_EQ(presearch_stats_updater_params.GetWeeklyParam(), "true");
    presearch_stats_updater_params.SavePrefs();

    // Make sure that local state was updated to reflect this as well
    EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), 45);
  }

  {
    // Now it's the next day (Tuesday)
    exploded.day_of_week = 2;
    exploded.day_of_month = 6;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

    SetCurrentTimeForTest(current_time);
    presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        presearch_stats::ProcessArch::kArchSkip);

    // Make sure that the weekly param was set to false, since this is
    // still the same ISO week (#45)
    EXPECT_EQ(presearch_stats_updater_params.GetWeeklyParam(), "false");
    presearch_stats_updater_params.SavePrefs();

    // Make sure that local state also didn't change
    EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), 45);
  }
}

TEST_F(PresearchStatsUpdaterTest, HasCorrectWeekOfInstallation) {
  base::Time::Exploded exploded;
  base::Time current_time;

  {
    // Set date to 2019-03-24 (Sunday)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2019;
    exploded.month = 3;
    exploded.day_of_month = 24;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
    SetCurrentTimeForTest(current_time);

    // Make sure that week of installation is previous Monday
    presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        presearch_stats::ProcessArch::kArchSkip);
    EXPECT_EQ(presearch_stats_updater_params.GetWeekOfInstallationParam(),
              "2019-03-18");
  }

  {
    // Set date to 2019-03-25 (Monday)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2019;
    exploded.month = 3;
    exploded.day_of_month = 25;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
    SetCurrentTimeForTest(current_time);

    // Make sure that week of installation is today, since today is a
    // Monday
    presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        presearch_stats::ProcessArch::kArchSkip);
    EXPECT_EQ(presearch_stats_updater_params.GetWeekOfInstallationParam(),
              "2019-03-25");
  }

  {
    // Set date to 2019-03-30 (Saturday)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2019;
    exploded.month = 3;
    exploded.day_of_month = 30;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
    SetCurrentTimeForTest(current_time);

    // Make sure that week of installation is previous Monday
    presearch_stats::PresearchStatsUpdaterParams presearch_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        presearch_stats::ProcessArch::kArchSkip);
    EXPECT_EQ(presearch_stats_updater_params.GetWeekOfInstallationParam(),
              "2019-03-25");
  }
}

TEST_F(PresearchStatsUpdaterTest, GetIsoWeekNumber) {
  base::Time::Exploded exploded;
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 1;
  exploded.day_of_month = 29;
  exploded.month = 7;
  exploded.year = 2019;

  base::Time time;
  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &time));
  EXPECT_EQ(presearch_stats::GetIsoWeekNumber(time), 31);

  exploded.day_of_month = 30;
  exploded.month = 9;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &time));
  EXPECT_EQ(presearch_stats::GetIsoWeekNumber(time), 40);

  exploded.day_of_month = 1;
  exploded.month = 9;
  exploded.day_of_week = 0;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &time));
  EXPECT_EQ(presearch_stats::GetIsoWeekNumber(time), 35);
}
