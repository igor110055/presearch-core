// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_COMMON_FEATURES_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_COMMON_FEATURES_H_

namespace base {
struct Feature;
}  // namespace base

namespace presearch_shields {
namespace features {
extern const base::Feature kPresearchAdblockCosmeticFiltering;
extern const base::Feature kPresearchAdblockCosmeticFilteringNative;
extern const base::Feature kPresearchAdblockCspRules;
extern const base::Feature kPresearchDomainBlock;
extern const base::Feature kPresearchExtensionNetworkBlocking;
}  // namespace features
}  // namespace presearch_shields

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_COMMON_FEATURES_H_
