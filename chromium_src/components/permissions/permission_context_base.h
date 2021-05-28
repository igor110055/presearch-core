/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_CONTEXT_BASE_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_CONTEXT_BASE_H_

namespace permissions {
class PermissionContextBase;
using PermissionContextBase_PresearchImpl = PermissionContextBase;
}  // namespace permissions

#define PermissionContextBase PermissionContextBase_ChromiumImpl
#define PermissionDecided virtual PermissionDecided
#define PRESEARCH_PERMISSION_CONTEXT_BASE_H_ friend PermissionContextBase_PresearchImpl;

#include "../../../../components/permissions/permission_context_base.h"

#undef PRESEARCH_PERMISSION_CONTEXT_BASE_H_
#undef PermissionDecided
#undef PermissionContextBase

namespace permissions {

class PermissionLifetimeManager;

class PermissionContextBase : public PermissionContextBase_ChromiumImpl {
 public:
  using PermissionContextBase_ChromiumImpl::PermissionContextBase_ChromiumImpl;

  void SetPermissionLifetimeManager(
      PermissionLifetimeManager* lifetime_manager);

 private:
  void PermissionDecided(const PermissionRequestID& id,
                         const GURL& requesting_origin,
                         const GURL& embedding_origin,
                         BrowserPermissionCallback callback,
                         ContentSetting content_setting,
                         bool is_one_time) override;

  PermissionLifetimeManager* permission_lifetime_manager_ = nullptr;
};

}  // namespace permissions

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSION_CONTEXT_BASE_H_
