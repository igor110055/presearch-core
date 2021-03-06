/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_UI_PRESEARCH_ADS_NOTIFICATION_VIEW_FACTORY_H_
#define PRESEARCH_UI_PRESEARCH_ADS_NOTIFICATION_VIEW_FACTORY_H_

#include <memory>
#include <string>

#include "base/callback_forward.h"

namespace presearch_ads {

class NotificationView;
class Notification;

// Creates appropriate NotificationViews for notifications depending on the
// notification type. A notification is top level if it needs to be rendered
// outside the browser window. No custom shadows are created for top level
// notifications on Linux with Aura.
class NotificationViewFactory {
 public:
  static NotificationView* Create(const Notification& notification);
};

}  // namespace presearch_ads

#endif  // PRESEARCH_UI_PRESEARCH_ADS_NOTIFICATION_VIEW_FACTORY_H_
