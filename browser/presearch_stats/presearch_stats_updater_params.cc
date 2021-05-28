/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>

#include "presearch/browser/presearch_stats/presearch_stats_updater_params.h"
#include "presearch/components/presearch_referrals/buildflags/buildflags.h"

#include "base/strings/string_util.h"
#include "base/system/sys_info.h"
#include "base/threading/thread_restrictions.h"
#include "base/time/time.h"
#include "bat/ads/pref_names.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_referrals/common/pref_names.h"
#include "chrome/browser/first_run/first_run.h"
#include "components/prefs/pref_service.h"

namespace presearch_stats {

base::Time PresearchStatsUpdaterParams::g_current_time;
bool PresearchStatsUpdaterParams::g_force_first_run = false;
static constexpr base::TimeDelta g_dtoi_delete_delta =
    base::TimeDelta::FromSeconds(14 * 24 * 60 * 60);

PresearchStatsUpdaterParams::PresearchStatsUpdaterParams(
    PrefService* stats_pref_service,
    PrefService* profile_pref_service,
    const ProcessArch arch)
    : PresearchStatsUpdaterParams(stats_pref_service,
                              profile_pref_service,
                              arch,
                              GetCurrentDateAsYMD(),
                              GetCurrentISOWeekNumber(),
                              GetCurrentMonth()) {}

PresearchStatsUpdaterParams::PresearchStatsUpdaterParams(
    PrefService* stats_pref_service,
    PrefService* profile_pref_service,
    const ProcessArch arch,
    const std::string& ymd,
    int woy,
    int month)
    : stats_pref_service_(stats_pref_service),
      profile_pref_service_(profile_pref_service),
      arch_(arch),
      ymd_(ymd),
      woy_(woy),
      month_(month) {
  LoadPrefs();
}

PresearchStatsUpdaterParams::~PresearchStatsUpdaterParams() {}

std::string PresearchStatsUpdaterParams::GetDailyParam() const {
  return BooleanToString(
      base::CompareCaseInsensitiveASCII(ymd_, last_check_ymd_) == 1);
}

std::string PresearchStatsUpdaterParams::GetWeeklyParam() const {
  return BooleanToString(last_check_woy_ == 0 || woy_ != last_check_woy_);
}

std::string PresearchStatsUpdaterParams::GetMonthlyParam() const {
  return BooleanToString(last_check_month_ == 0 || month_ != last_check_month_);
}

std::string PresearchStatsUpdaterParams::GetFirstCheckMadeParam() const {
  return BooleanToString(!first_check_made_);
}

std::string PresearchStatsUpdaterParams::GetWeekOfInstallationParam() const {
  return week_of_installation_;
}

std::string PresearchStatsUpdaterParams::GetDateOfInstallationParam() const {
  return (GetCurrentTimeNow() - date_of_installation_ >= g_dtoi_delete_delta)
             ? "null"
             : presearch_stats::GetDateAsYMD(date_of_installation_);
}

std::string PresearchStatsUpdaterParams::GetReferralCodeParam() const {
  return referral_promo_code_.empty() ? "none" : referral_promo_code_;
}

std::string PresearchStatsUpdaterParams::GetAdsEnabledParam() const {
  return BooleanToString(
      profile_pref_service_->GetBoolean(ads::prefs::kEnabled));
}

std::string PresearchStatsUpdaterParams::GetProcessArchParam() const {
  if (arch_ == ProcessArch::kArchSkip) {
    return "";
  } else if (arch_ == ProcessArch::kArchMetal) {
    return base::SysInfo::OperatingSystemArchitecture();
  } else {
    return "virt";
  }
}

void PresearchStatsUpdaterParams::LoadPrefs() {
  last_check_ymd_ = stats_pref_service_->GetString(kLastCheckYMD);
  last_check_woy_ = stats_pref_service_->GetInteger(kLastCheckWOY);
  last_check_month_ = stats_pref_service_->GetInteger(kLastCheckMonth);
  first_check_made_ = stats_pref_service_->GetBoolean(kFirstCheckMade);
  week_of_installation_ = stats_pref_service_->GetString(kWeekOfInstallation);
  if (week_of_installation_.empty())
    week_of_installation_ = GetLastMondayAsYMD();

  if (ShouldForceFirstRun()) {
    date_of_installation_ = GetCurrentTimeNow();
  } else {
    date_of_installation_ = GetFirstRunTime(stats_pref_service_);
    if (date_of_installation_.is_null()) {
      LOG(WARNING)
          << "Couldn't find the time of first run. This should only happen "
             "when running tests, but never in production code.";
    }
  }

#if BUILDFLAG(ENABLE_PRESEARCH_REFERRALS)
  referral_promo_code_ = stats_pref_service_->GetString(kReferralPromoCode);
#endif
}

void PresearchStatsUpdaterParams::SavePrefs() {
  stats_pref_service_->SetString(kLastCheckYMD, ymd_);
  stats_pref_service_->SetInteger(kLastCheckWOY, woy_);
  stats_pref_service_->SetInteger(kLastCheckMonth, month_);
  stats_pref_service_->SetBoolean(kFirstCheckMade, true);
  stats_pref_service_->SetString(kWeekOfInstallation, week_of_installation_);
}

std::string PresearchStatsUpdaterParams::BooleanToString(bool bool_value) const {
  return bool_value ? "true" : "false";
}

std::string PresearchStatsUpdaterParams::GetCurrentDateAsYMD() const {
  return presearch_stats::GetDateAsYMD(GetCurrentTimeNow());
}

std::string PresearchStatsUpdaterParams::GetLastMondayAsYMD() const {
  base::Time now = GetCurrentTimeNow();
  base::Time::Exploded exploded;
  now.LocalExplode(&exploded);

  int days_adjusted =
      (exploded.day_of_week == 0) ? 6 : exploded.day_of_week - 1;
  base::Time last_monday = base::Time::FromJsTime(
      now.ToJsTime() - (days_adjusted * base::Time::kMillisecondsPerDay));

  return presearch_stats::GetDateAsYMD(last_monday);
}

int PresearchStatsUpdaterParams::GetCurrentMonth() const {
  base::Time now = GetCurrentTimeNow();
  base::Time::Exploded exploded;
  now.LocalExplode(&exploded);
  return exploded.month;
}

int PresearchStatsUpdaterParams::GetCurrentISOWeekNumber() const {
  return GetIsoWeekNumber(GetCurrentTimeNow());
}

base::Time PresearchStatsUpdaterParams::GetCurrentTimeNow() const {
  return g_current_time.is_null() ? base::Time::Now() : g_current_time;
}

// static
bool PresearchStatsUpdaterParams::ShouldForceFirstRun() const {
  return g_force_first_run;
}

// static
void PresearchStatsUpdaterParams::SetCurrentTimeForTest(
    const base::Time& current_time) {
  g_current_time = current_time;
}

// static
void PresearchStatsUpdaterParams::SetFirstRunForTest(bool first_run) {
  g_force_first_run = first_run;
}

// static
base::Time PresearchStatsUpdaterParams::GetFirstRunTime(PrefService* pref_service) {
#if defined(OS_ANDROID)
  // Android doesn't use a sentinel to track first run, so we use a
  // preference instead. kReferralAndroidFirstRunTimestamp is used because
  // previously only referrals needed to know the first run value.
  base::Time first_run_timestamp =
      pref_service->GetTime(kReferralAndroidFirstRunTimestamp);
  if (first_run_timestamp.is_null()) {
    first_run_timestamp = base::Time::Now();
    pref_service->SetTime(kReferralAndroidFirstRunTimestamp,
                          first_run_timestamp);
  }
  return first_run_timestamp;
#else
  (void)pref_service;  // suppress unused warning

  // CreateSentinelIfNeeded() is called in chrome_browser_main.cc, making this a
  // non-blocking read of the cached sentinel value when running from production
  // code. However tests will never create the sentinel file due to being run
  // with the switches:kNoFirstRun flag, so we need to allow blocking for that.
  base::ScopedAllowBlockingForTesting allow_blocking;
  return first_run::GetFirstRunSentinelCreationTime();
#endif  // #defined(OS_ANDROID)
}

}  // namespace presearch_stats
