/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_sync/features.h"

#include "base/feature_list.h"
#include "presearch/components/presearch_sync/buildflags/buildflags.h"

namespace presearch_sync {
namespace features {

#if BUILDFLAG(ENABLE_PRESEARCH_SYNC)
const base::Feature kPresearchSync{"PresearchSync", base::FEATURE_ENABLED_BY_DEFAULT};
#else
const base::Feature kPresearchSync{"PresearchSync", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

}  // namespace features
}  // namespace presearch_sync
