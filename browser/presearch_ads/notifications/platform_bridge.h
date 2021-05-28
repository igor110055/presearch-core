/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_ADS_NOTIFICATIONS_PLATFORM_BRIDGE_H_
#define PRESEARCH_BROWSER_PRESEARCH_ADS_NOTIFICATIONS_PLATFORM_BRIDGE_H_

#include <memory>
#include <string>

#include "base/macros.h"
#include "presearch/components/presearch_ads/browser/ads_notification_handler.h"
#include "presearch/ui/presearch_ads/public/cpp/notification.h"

class Profile;

class PlatformBridge {
 public:
  explicit PlatformBridge(Profile* profile);
  ~PlatformBridge();

  void Display(Profile* profile,
               const std::unique_ptr<presearch_ads::Notification>& notification);
  void Close(Profile* profile, const std::string& notification_id);

  void ShowAndroidAdsNotification(
      Profile* profile,
      const std::unique_ptr<presearch_ads::Notification>& notification);

  void CloseAndroidAdsNotification(Profile* profile,
                                   const std::string& notification_id);

 private:
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(PlatformBridge);
};

#endif  // PRESEARCH_BROWSER_PRESEARCH_ADS_NOTIFICATIONS_PLATFORM_BRIDGE_H_
