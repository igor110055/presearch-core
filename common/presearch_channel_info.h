/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMMON_PRESEARCH_CHANNEL_INFO_H_
#define PRESEARCH_COMMON_PRESEARCH_CHANNEL_INFO_H_

#include <string>

namespace presearch {

std::string GetChannelName();
bool IsNightlyOrDeveloperBuild();

}

#endif  // PRESEARCH_COMMON_PRESEARCH_CHANNEL_INFO_H_

