// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/components/presearch_shields/common/features.h"

#include "base/feature_list.h"

namespace presearch_shields {
namespace features {

const base::Feature kPresearchAdblockCosmeticFiltering{
    "PresearchAdblockCosmeticFiltering",
    base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature kPresearchAdblockCosmeticFilteringNative{
    "PresearchAdblockCosmeticFilteringNative", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kPresearchAdblockCspRules{
    "PresearchAdblockCspRules", base::FEATURE_ENABLED_BY_DEFAULT};
// When enabled, Presearch will block domains listed in the user's selected adblock
// filters and present a security interstitial with choice to proceed and
// optionally whitelist the domain.
// Domain block filters look like this:
// ||ads.example.com^
const base::Feature kPresearchDomainBlock{"PresearchDomainBlock",
                                      base::FEATURE_ENABLED_BY_DEFAULT};
// When enabled, network requests initiated by extensions will be checked and
// potentially blocked by Presearch Shields.
const base::Feature kPresearchExtensionNetworkBlocking{
    "PresearchExtensionNetworkBlocking", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features
}  // namespace presearch_shields
