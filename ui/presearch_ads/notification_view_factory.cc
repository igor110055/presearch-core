/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/ui/presearch_ads/notification_view_factory.h"

#include <vector>

#include "base/lazy_instance.h"
#include "presearch/ui/presearch_ads/ad_notification_view_md.h"

namespace presearch_ads {

// static
NotificationView* NotificationViewFactory::Create(
    const Notification& notification) {
  NotificationView* notification_view = nullptr;
  if (!notification_view)
    notification_view = new AdNotificationViewMD(notification);
  return notification_view;
}

}  // namespace presearch_ads
