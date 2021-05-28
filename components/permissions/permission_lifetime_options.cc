/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/permissions/permission_lifetime_options.h"

namespace permissions {

PermissionLifetimeOption::PermissionLifetimeOption(
    base::string16 label,
    base::Optional<base::TimeDelta> lifetime)
    : label(std::move(label)), lifetime(std::move(lifetime)) {}

PermissionLifetimeOption::PermissionLifetimeOption(
    const PermissionLifetimeOption&) = default;
PermissionLifetimeOption& PermissionLifetimeOption::operator=(
    const PermissionLifetimeOption&) = default;
PermissionLifetimeOption::PermissionLifetimeOption(
    PermissionLifetimeOption&&) noexcept = default;
PermissionLifetimeOption& PermissionLifetimeOption::operator=(
    PermissionLifetimeOption&&) noexcept = default;
PermissionLifetimeOption::~PermissionLifetimeOption() = default;

}  // namespace permissions
