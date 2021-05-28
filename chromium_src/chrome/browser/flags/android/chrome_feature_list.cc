/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/common/presearch_features.h"
#include "third_party/blink/public/common/features.h"

namespace chrome {
namespace android {

namespace {

const base::Feature* kPresearchFeaturesExposedToJava[] = {
    &features::kPresearchRewards,
    &blink::features::kForceWebContentsDarkMode,
};

const base::Feature* PresearchFindFeatureExposedToJava(
    const std::string& feature_name) {
  for (size_t i = 0; i < base::size(kPresearchFeaturesExposedToJava); ++i) {
    if (kPresearchFeaturesExposedToJava[i]->name == feature_name)
      return kPresearchFeaturesExposedToJava[i];
  }

  return nullptr;
}

}  // namespace

}  // namespace android
}  // namespace chrome

#define PRESEARCH_FIND_FEATURE_EXPOSED_TO_JAVA                                    \
  const base::Feature* feature = PresearchFindFeatureExposedToJava(feature_name); \
  if (feature)                                                                \
    return feature;

#include "../../../../../../chrome/browser/flags/android/chrome_feature_list.cc"
#undef PRESEARCH_FIND_FEATURE_EXPOSED_TO_JAVA
