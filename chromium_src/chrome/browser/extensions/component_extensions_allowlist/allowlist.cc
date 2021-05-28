/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define IsComponentExtensionAllowlisted IsComponentExtensionAllowlisted_ChromiumImpl  // NOLINT
#include "../../../../../../chrome/browser/extensions/component_extensions_allowlist/allowlist.cc"  // NOLINT
#undef IsComponentExtensionAllowlisted

#include "base/stl_util.h"
#include "presearch/components/presearch_extension/grit/presearch_extension.h"
#include "presearch/components/presearch_rewards/resources/extension/grit/presearch_rewards_extension_resources.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/presearch_webtorrent/grit/presearch_webtorrent_resources.h"
#include "components/grit/presearch_components_resources.h"
#include "extensions/common/constants.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#endif

namespace extensions {

  bool IsComponentExtensionAllowlisted(const std::string& extension_id) {
    const char* const kAllowed[] = {
      presearch_extension_id,
      presearch_rewards_extension_id,
#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
      ethereum_remote_client_extension_id,
#endif
      presearch_webtorrent_extension_id
    };

    for (size_t i = 0; i < base::size(kAllowed); ++i) {
      if (extension_id == kAllowed[i])
        return true;
    }

    return IsComponentExtensionAllowlisted_ChromiumImpl(extension_id);
  }

  bool IsComponentExtensionAllowlisted(int manifest_resource_id) {
    switch (manifest_resource_id) {
      // Please keep the list in alphabetical order.
      case IDR_PRESEARCH_EXTENSION:
      case IDR_PRESEARCH_REWARDS:
      case IDR_PRESEARCH_WEBTORRENT:
        return true;
    }

    return IsComponentExtensionAllowlisted_ChromiumImpl(manifest_resource_id);
  }

}  // namespace extensions
