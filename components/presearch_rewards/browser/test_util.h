/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_TEST_UTIL_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_TEST_UTIL_H_

#include <memory>

#include "base/files/file_path.h"

class KeyedService;
class Profile;

namespace presearch_rewards {

std::unique_ptr<Profile> CreatePresearchRewardsProfile(const base::FilePath& path);

}  // namespace presearch_rewards

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_REWARDS_BROWSER_TEST_UTIL_H_
