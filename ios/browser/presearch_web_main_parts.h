/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_BROWSER_PRESEARCH_WEB_MAIN_PARTS_H_
#define PRESEARCH_IOS_BROWSER_PRESEARCH_WEB_MAIN_PARTS_H_

#include <memory>

#include "base/macros.h"
#include "ios/chrome/browser/ios_chrome_field_trials.h"
#include "ios/web/public/init/web_main_parts.h"

class ApplicationContextImpl;
class PrefService;

namespace base {
class FieldTrialList;
}

class PresearchWebMainParts : public web::WebMainParts {
 public:
  PresearchWebMainParts();
  ~PresearchWebMainParts() override;

 private:
  // web::WebMainParts implementation.
  void PreMainMessageLoopStart() override;
  void PreCreateThreads() override;
  void PreMainMessageLoopRun() override;
  void PostMainMessageLoopRun() override;
  void PostDestroyThreads() override;
  void SetupFieldTrials();

  std::unique_ptr<ApplicationContextImpl> application_context_;

  // Statistical testing infrastructure for the entire browser. NULL until
  // SetUpMetricsAndFieldTrials is called.
  std::unique_ptr<base::FieldTrialList> field_trial_list_;

  PrefService* local_state_;

  IOSChromeFieldTrials ios_field_trials_;

  DISALLOW_COPY_AND_ASSIGN(PresearchWebMainParts);
};

#endif  // PRESEARCH_IOS_BROWSER_PRESEARCH_WEB_MAIN_PARTS_H_
