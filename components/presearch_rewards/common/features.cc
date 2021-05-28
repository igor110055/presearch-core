/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_rewards/common/features.h"

#include "base/feature_list.h"

namespace presearch_rewards {
namespace features {

const base::Feature kBitflyerFeature{"PresearchRewardsBitflyer",
                                     base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace features
}  // namespace presearch_rewards
