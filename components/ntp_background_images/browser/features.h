/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_FEATURES_H_
#define PRESEARCH_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_FEATURES_H_

namespace base {
struct Feature;
}  // namespace base

namespace ntp_background_images {
namespace features {
extern const base::Feature kPresearchNTPBrandedWallpaper;
extern const base::Feature kPresearchNTPBrandedWallpaperDemo;
extern const base::Feature kPresearchNTPSuperReferralWallpaper;
}  // namespace features
}  // namespace ntp_background_images

#endif  // PRESEARCH_COMPONENTS_NTP_BACKGROUND_IMAGES_BROWSER_FEATURES_H_
