/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_SERVICE_SANDBOX_TYPE_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_SERVICE_SANDBOX_TYPE_H_

#include "../../../../chrome/browser/service_sandbox_type.h"

#include "presearch/components/ipfs/buildflags/buildflags.h"

// presearch::mojom::ProfileImport
namespace presearch {
namespace mojom {
class ProfileImport;
}
}  // namespace presearch

template <>
inline sandbox::policy::SandboxType
content::GetServiceSandboxType<presearch::mojom::ProfileImport>() {
  return sandbox::policy::SandboxType::kNoSandbox;
}

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/service_sandbox_type.h"
#endif

#if !defined(OS_ANDROID)  // Android will use default, which is kUtility.
namespace bat_ledger {
namespace mojom {
class BatLedgerService;
}  // namespace mojom
}  // namespace bat_ledger

template <>
inline sandbox::policy::SandboxType
content::GetServiceSandboxType<bat_ledger::mojom::BatLedgerService>() {
  return sandbox::policy::SandboxType::kNoSandbox;
}
#endif  // !defined(OS_ANDROID)

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_SERVICE_SANDBOX_TYPE_H_
