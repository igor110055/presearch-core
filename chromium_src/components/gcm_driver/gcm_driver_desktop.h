/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_GCM_DRIVER_GCM_DRIVER_DESKTOP_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_GCM_DRIVER_GCM_DRIVER_DESKTOP_H_

#include "../../../../components/gcm_driver/gcm_driver_desktop.h"

namespace gcm {

class PresearchGCMDriverDesktop : public GCMDriverDesktop {
 public:
  using GCMDriverDesktop::GCMDriverDesktop;
  ~PresearchGCMDriverDesktop() override;

  void SetEnabled(bool enabled);

 protected:
  // GCMDriver implementation:
  GCMClient::Result EnsureStarted(GCMClient::StartMode start_mode) override;

 private:
  bool enabled_;
};

}  // namespace gcm

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_GCM_DRIVER_GCM_DRIVER_DESKTOP_H_
