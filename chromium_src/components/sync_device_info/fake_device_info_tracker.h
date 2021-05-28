// Copyright (c) 2021 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_FAKE_DEVICE_INFO_TRACKER_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_FAKE_DEVICE_INFO_TRACKER_H_

#include "presearch/components/sync_device_info/presearch_device_info.h"
#include "components/sync_device_info/device_info_tracker.h"

#define ForcePulseForTest                                                    \
  DeleteDeviceInfo(const std::string& client_id, base::OnceClosure callback) \
      override;                                                              \
  std::vector<std::unique_ptr<PresearchDeviceInfo>> GetAllPresearchDeviceInfo()      \
      const override;                                                        \
  void ForcePulseForTest

#include "../../../../components/sync_device_info/fake_device_info_tracker.h"

#undef ForcePulseForTest

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_FAKE_DEVICE_INFO_TRACKER_H_
