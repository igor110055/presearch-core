/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_PROFILES_PRESEARCH_AVATAR_TOOLBAR_BUTTON_H_
#define PRESEARCH_BROWSER_UI_VIEWS_PROFILES_PRESEARCH_AVATAR_TOOLBAR_BUTTON_H_

#include "chrome/browser/ui/views/profiles/avatar_toolbar_button.h"

class PresearchAvatarToolbarButton : public AvatarToolbarButton {
 public:
  explicit PresearchAvatarToolbarButton(Browser* browser);

  // ToolbarButton:
  void SetHighlight(const base::string16& highlight_text,
                    base::Optional<SkColor> highlight_color) override;
  void UpdateColorsAndInsets() override;

 private:
  // AvatarToolbarButton:
  ui::ImageModel GetAvatarIcon(
      ButtonState state,
      const gfx::Image& profile_identity_image) const override;
  base::string16 GetAvatarTooltipText() const override;

  DISALLOW_COPY_AND_ASSIGN(PresearchAvatarToolbarButton);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_PROFILES_PRESEARCH_AVATAR_TOOLBAR_BUTTON_H_
