/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_IPFS_SERVICE_SANDBOX_TYPE_H_
#define PRESEARCH_COMPONENTS_IPFS_SERVICE_SANDBOX_TYPE_H_

#include "content/public/browser/service_process_host.h"

// ipfs::mojom::IpfsService
namespace ipfs {
namespace mojom {
class IpfsService;
}  // namespace mojom
}  // namespace ipfs

template <>
inline sandbox::policy::SandboxType
content::GetServiceSandboxType<ipfs::mojom::IpfsService>() {
  return sandbox::policy::SandboxType::kNoSandbox;
}

#endif  // PRESEARCH_COMPONENTS_IPFS_SERVICE_SANDBOX_TYPE_H_
