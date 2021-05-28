// Copyright 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/components/presearch_ads/browser/ads_notification_handler.h"

#define PRESEARCH_ADD_PRESEARCH_ADS_NOTIFICATION_HANDLER \
  AddNotificationHandler(                        \
      NotificationHandler::Type::PRESEARCH_ADS,      \
      std::make_unique<presearch_ads::AdsNotificationHandler>(profile));
#include "../../../../../chrome/browser/notifications/notification_display_service_impl.cc"
#undef PRESEARCH_ADD_PRESEARCH_ADS_NOTIFICATION_HANDLER
