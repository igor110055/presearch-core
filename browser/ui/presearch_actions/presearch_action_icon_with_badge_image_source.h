/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_PRESEARCH_ACTIONS_PRESEARCH_ACTION_ICON_WITH_BADGE_IMAGE_SOURCE_H_
#define PRESEARCH_BROWSER_UI_PRESEARCH_ACTIONS_PRESEARCH_ACTION_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#include "chrome/browser/ui/extensions/icon_with_badge_image_source.h"

namespace gfx {
  class Canvas;
  class Rect;
}

// The purpose of this subclass is to:
// - Paint the PresearchAction badge in a custom location and with a different size
//   to regular BrowserAction extensions.
class PresearchActionIconWithBadgeImageSource : public IconWithBadgeImageSource {
  public:
    using IconWithBadgeImageSource::IconWithBadgeImageSource;
  private:
    base::Optional<int> GetCustomGraphicSize() override;
    base::Optional<int> GetCustomGraphicXOffset() override;
    base::Optional<int> GetCustomGraphicYOffset() override;
    void PaintBadge(gfx::Canvas* canvas) override;
    gfx::Rect GetIconAreaRect() const override;
    DISALLOW_COPY_AND_ASSIGN(PresearchActionIconWithBadgeImageSource);
};

#endif
