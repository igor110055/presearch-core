/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_NOTIFICATION_HELPER_MOCK_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_NOTIFICATION_HELPER_MOCK_H_

#include "presearch/components/presearch_ads/browser/notification_helper.h"

#include "testing/gmock/include/gmock/gmock.h"

namespace presearch_ads {

class NotificationHelperMock : public NotificationHelper {
 public:
  NotificationHelperMock();
  ~NotificationHelperMock() override;

  NotificationHelperMock(const NotificationHelperMock&) = delete;
  NotificationHelperMock& operator=(const NotificationHelperMock&) = delete;

  MOCK_METHOD0(ShouldShowNotifications, bool());
  MOCK_METHOD0(ShowMyFirstAdNotification, bool());
  MOCK_CONST_METHOD0(CanShowBackgroundNotifications, bool());
};

}  // namespace presearch_ads

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_ADS_BROWSER_NOTIFICATION_HELPER_MOCK_H_
