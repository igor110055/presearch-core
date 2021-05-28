/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_NET_BASE_FEATURES_H_
#define PRESEARCH_CHROMIUM_SRC_NET_BASE_FEATURES_H_

#include "base/feature_list.h"
#include "net/base/net_export.h"

namespace net {
namespace features {

NET_EXPORT extern const base::Feature kPresearchEphemeralStorage;
NET_EXPORT extern const base::Feature kPresearchEphemeralStorageKeepAlive;
NET_EXPORT extern const base::FeatureParam<int>
    kPresearchEphemeralStorageKeepAliveTimeInSeconds;

}  // namespace features
}  // namespace net

#include "../../../../net/base/features.h"

#endif  // PRESEARCH_CHROMIUM_SRC_NET_BASE_FEATURES_H_
