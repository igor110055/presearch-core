/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TAB_MANAGER_TAB_INFO_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TAB_MANAGER_TAB_INFO_H_

#include <cstdint>
#include <string>

namespace ads {

struct TabInfo {
  TabInfo();
  TabInfo(const TabInfo& info);
  ~TabInfo();

  bool operator==(const TabInfo& rhs) const;
  bool operator!=(const TabInfo& rhs) const;

  int32_t id = 0;
  std::string url;
  bool is_playing_media = false;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TAB_MANAGER_TAB_INFO_H_
