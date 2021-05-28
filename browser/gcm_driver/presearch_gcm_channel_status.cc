/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/gcm_driver/presearch_gcm_channel_status.h"

#include <memory>

#include "presearch/common/pref_names.h"
#include "chrome/browser/gcm/gcm_profile_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_driver_desktop.h"
#include "components/gcm_driver/gcm_profile_service.h"
#include "components/prefs/pref_service.h"

namespace gcm {

const char kPresearchGCMStatusKey[] = "presearch_gcm_channel_status";

PresearchGCMChannelStatus::PresearchGCMChannelStatus(Profile* profile, bool enabled)
    : profile_(profile), gcm_enabled_(enabled) {}

// static
PresearchGCMChannelStatus* PresearchGCMChannelStatus::GetForProfile(
    Profile* profile) {
  PresearchGCMChannelStatus* status = static_cast<PresearchGCMChannelStatus*>(
      profile->GetUserData(kPresearchGCMStatusKey));

  if (!status) {
    bool enabled = profile->GetPrefs()->GetBoolean(kPresearchGCMChannelStatus);
    // Object cleanup is handled by SupportsUserData
    profile->SetUserData(
        kPresearchGCMStatusKey,
        std::make_unique<PresearchGCMChannelStatus>(profile, enabled));
    status = static_cast<PresearchGCMChannelStatus*>(
        profile->GetUserData(kPresearchGCMStatusKey));
  }
  return status;
}

bool PresearchGCMChannelStatus::IsGCMEnabled() const {
  return gcm_enabled_;
}

void PresearchGCMChannelStatus::UpdateGCMDriverStatus() {
  if (!profile_)
    return;
  gcm::GCMProfileService* gcm_profile_service =
      gcm::GCMProfileServiceFactory::GetForProfile(profile_);
  if (!gcm_profile_service)
    return;
  gcm::GCMDriver* gcm_driver = gcm_profile_service->driver();
  if (!gcm_driver)
    return;
  (static_cast<gcm::PresearchGCMDriverDesktop*>(gcm_driver))
      ->SetEnabled(IsGCMEnabled());
}

}  // namespace gcm
