/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_GCM_DRIVER_PRESEARCH_GCM_CHANNEL_STATUS_H_
#define PRESEARCH_BROWSER_GCM_DRIVER_PRESEARCH_GCM_CHANNEL_STATUS_H_

#include "base/supports_user_data.h"

class Profile;

namespace gcm {

class PresearchGCMChannelStatus : public base::SupportsUserData::Data {
 public:
  explicit PresearchGCMChannelStatus(Profile* profile, bool enabled);
  static PresearchGCMChannelStatus* GetForProfile(Profile *profile);

  bool IsGCMEnabled() const;
  void UpdateGCMDriverStatus();

 private:
  Profile* profile_;
  bool gcm_enabled_;
};

}  // namespace gcm

#endif  // PRESEARCH_BROWSER_GCM_DRIVER_PRESEARCH_GCM_CHANNEL_STATUS_H_
