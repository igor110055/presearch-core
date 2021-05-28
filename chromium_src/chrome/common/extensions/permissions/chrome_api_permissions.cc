/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define GetPermissionInfos GetPermissionInfos_ChromiumImpl
#include "../../../../../../chrome/common/extensions/permissions/chrome_api_permissions.cc"
#undef GetPermissionInfos

namespace extensions {
namespace chrome_api_permissions {

namespace {

constexpr APIPermissionInfo::InitInfo presearch_permissions_to_register[] = {
    {APIPermission::kIpfs, "ipfs",
     APIPermissionInfo::kFlagImpliesFullURLAccess}};

// Merges Presearch and Chrormium constant arrays to final list of permissions.
template <typename T, size_t N>
class PermissionsContainer {
 public:
  constexpr PermissionsContainer(base::span<const T> chromium,
                                 base::span<const T> presearch) {
    CHECK(N == chromium.size() + presearch.size());
    size_t last_index = 0;
    for (const auto& item : chromium) {
      permissions_[last_index++] = item;
    }
    for (const auto& item : presearch) {
      permissions_[last_index++] = item;
    }
  }
  base::span<const T> GetPermissionInfos() const {
    return base::make_span(permissions_);
  }

 private:
  T permissions_[N];
};

constexpr size_t PermissionsTotal = base::size(permissions_to_register) +
                                    base::size(presearch_permissions_to_register);

const PermissionsContainer<APIPermissionInfo::InitInfo, PermissionsTotal>
    final_permissions(base::make_span(permissions_to_register),
                      base::make_span(presearch_permissions_to_register));

}  // namespace

base::span<const APIPermissionInfo::InitInfo> GetPermissionInfos() {
  return final_permissions.GetPermissionInfos();
}

}  // namespace chrome_api_permissions
}  // namespace extensions
