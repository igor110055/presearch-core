/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_P3A_P3A_CORE_METRICS_H_
#define PRESEARCH_BROWSER_P3A_P3A_CORE_METRICS_H_

// The classes below can be used on desktop only
// because BrowserListObserver is available on desktop only
// Presearch.Uptime.BrowserOpenMinutes, Presearch.Core.LastTimeIncognitoUsed and
// Presearch.Core.TorEverUsed don't work on Android

#include "build/build_config.h"

#if defined(OS_ANDROID)
#error This file should only be included on desktop.
#endif

#include <list>

#include "base/timer/timer.h"
#include "presearch/components/weekly_storage/weekly_storage.h"
#include "chrome/browser/resource_coordinator/usage_clock.h"
#include "chrome/browser/ui/browser_list_observer.h"

class PrefService;
class PrefRegistrySimple;

namespace presearch {

class PresearchUptimeTracker {
 public:
  explicit PresearchUptimeTracker(PrefService* local_state);
  ~PresearchUptimeTracker();

  static void CreateInstance(PrefService* local_state);

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  void RecordUsage();
  void RecordP3A();

  resource_coordinator::UsageClock usage_clock_;
  base::RepeatingTimer timer_;
  base::TimeDelta current_total_usage_;
  WeeklyStorage state_;

  DISALLOW_COPY_AND_ASSIGN(PresearchUptimeTracker);
};

// PresearchWindowTracker is under !OS_ANDROID guard because
// BrowserListObserver should only be only on desktop
// Presearch.Uptime.BrowserOpenMinutes and Presearch.Core.LastTimeIncognitoUsed
// don't work on Android
#if !defined(OS_ANDROID)
// Periodically records P3A stats (extracted from Local State) regarding the
// time when incognito windows were used.
// Used as a leaking singletone.
class PresearchWindowTracker : public BrowserListObserver {
 public:
  explicit PresearchWindowTracker(PrefService* local_state);
  ~PresearchWindowTracker() override;

  static void CreateInstance(PrefService* local_state);

  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  // BrowserListObserver:
  void OnBrowserAdded(Browser* browser) override;
  void OnBrowserSetLastActive(Browser* browser) override;

  void UpdateP3AValues() const;

  base::RepeatingTimer timer_;
  PrefService* local_state_;
  DISALLOW_COPY_AND_ASSIGN(PresearchWindowTracker);
};
#endif  // !defined(OS_ANDROID)

}  // namespace presearch

#endif  // PRESEARCH_BROWSER_P3A_P3A_CORE_METRICS_H_
