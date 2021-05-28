/* Copyright (c) 2019 The Presearch Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/notifications/presearch_notification_platform_bridge.h"

#include <memory>

#include "presearch/browser/notifications/presearch_alert_dispatcher_mac.h"
#include "chrome/browser/notifications/notification_platform_bridge_mac.h"
#include "ui/message_center/public/cpp/notification.h"
#include "ui/message_center/public/cpp/notification_types.h"

@class NSUserNotificationCenter;

// static
std::unique_ptr<NotificationPlatformBridge>
PresearchNotificationPlatformBridge::Create() {
  base::scoped_nsobject<PresearchAlertDispatcherMac> alert_dispatcher(
      [[PresearchAlertDispatcherMac alloc] init]);
  return std::make_unique<NotificationPlatformBridgeMac>(
      [NSUserNotificationCenter defaultUserNotificationCenter],
          alert_dispatcher.get());
}
