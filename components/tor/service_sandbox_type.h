/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_TOR_SERVICE_SANDBOX_TYPE_H_
#define PRESEARCH_COMPONENTS_TOR_SERVICE_SANDBOX_TYPE_H_

#include "content/public/browser/service_process_host.h"

namespace tor {
namespace mojom {
class TorLauncher;
}  // namespace mojom
}  // namespace tor

template <>
inline sandbox::policy::SandboxType
content::GetServiceSandboxType<tor::mojom::TorLauncher>() {
  return sandbox::policy::SandboxType::kNoSandbox;
}

#endif  // PRESEARCH_COMPONENTS_TOR_SERVICE_SANDBOX_TYPE_H_
