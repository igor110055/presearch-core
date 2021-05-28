/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_

class PresearchActionIconWithBadgeImageSource;

#define PRESEARCH_ICON_WITH_BADGE_IMAGE_SOURCE_H_            \
 private:                                                \
  friend class PresearchActionIconWithBadgeImageSource;      \
  virtual base::Optional<int> GetCustomGraphicSize();    \
  virtual base::Optional<int> GetCustomGraphicXOffset(); \
  virtual base::Optional<int> GetCustomGraphicYOffset(); \
                                                         \
 public:                                                 \
// #define PRESEARCH_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#define PaintBadge virtual PaintBadge
#define GetIconAreaRect virtual GetIconAreaRect
#include "../../../../../../chrome/browser/ui/extensions/icon_with_badge_image_source.h"
#undef GetIconAreaRect
#undef PaintBadge
#undef PRESEARCH_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
