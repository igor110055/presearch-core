/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_APP_PRESEARCH_MAIN_DELEGATE_H_
#define PRESEARCH_APP_PRESEARCH_MAIN_DELEGATE_H_

#include <string>

#include "chrome/app/chrome_main_delegate.h"

// Chrome implementation of ContentMainDelegate.
class PresearchMainDelegate : public ChromeMainDelegate {
 public:
  PresearchMainDelegate();

  // |exe_entry_point_ticks| is the time at which the main function of the
  // executable was entered, or null if not available.
  explicit PresearchMainDelegate(base::TimeTicks exe_entry_point_ticks);
  ~PresearchMainDelegate() override;

 protected:
  // content::ContentMainDelegate implementation:
  bool BasicStartupComplete(int* exit_code) override;

  content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;
  content::ContentUtilityClient* CreateContentUtilityClient() override;
  void PreSandboxStartup() override;

 private:
#if defined(OS_ANDROID)
  void AdjustSyncServiceUrlForAndroid(std::string* presearch_sync_service_url);
#endif  // defined(OS_ANDROID)

  DISALLOW_COPY_AND_ASSIGN(PresearchMainDelegate);
};

#endif  // PRESEARCH_APP_PRESEARCH_MAIN_DELEGATE_H_
