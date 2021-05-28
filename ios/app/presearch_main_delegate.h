/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_IOS_APP_PRESEARCH_MAIN_DELEGATE_H_
#define PRESEARCH_IOS_APP_PRESEARCH_MAIN_DELEGATE_H_

#include <string>

#include "base/macros.h"
#include "ios/chrome/app/startup/ios_chrome_main_delegate.h"

class PresearchWebClient;

class PresearchMainDelegate : public IOSChromeMainDelegate {
 public:
  PresearchMainDelegate();
  ~PresearchMainDelegate() override;

  void SetSyncServiceURL(const std::string& url);

 protected:
  // web::WebMainDelegate implementation:
  void BasicStartupComplete() override;

 private:
  std::string presearch_sync_service_url_;
  DISALLOW_COPY_AND_ASSIGN(PresearchMainDelegate);
};

#endif  // PRESEARCH_IOS_APP_PRESEARCH_MAIN_DELEGATE_H_
