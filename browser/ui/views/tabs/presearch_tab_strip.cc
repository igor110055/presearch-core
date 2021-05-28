/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/tabs/presearch_tab_strip.h"

#include "presearch/browser/profiles/profile_util.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "chrome/browser/ui/views/tabs/tab_strip_controller.h"
#include "third_party/skia/include/core/SkColor.h"

PresearchTabStrip::~PresearchTabStrip() = default;

bool PresearchTabStrip::ShouldHideCloseButtonForTab(Tab* tab) const {
  bool should_hide = TabStrip::ShouldHideCloseButtonForTab(tab);

  // If upstream logic want to hide, follow it.
  if (should_hide)
    return should_hide;

  if (tab->IsActive())
    return false;

  // Only shows close button on tab when mouse is hovered on tab.
  return !tab->mouse_hovered();
}

SkColor PresearchTabStrip::GetTabSeparatorColor() const {
  Profile* profile = controller()->GetProfile();
  if (!presearch::IsRegularProfile(profile)) {
    if (profile->IsTor())
      return SkColorSetRGB(0x5A, 0x53, 0x66);

    // For incognito/guest window.
    return SkColorSetRGB(0x3F, 0x32, 0x56);
  }

  // If custom theme is used, follow upstream separator color.
  auto* theme_service = ThemeServiceFactory::GetForProfile(profile);
  if (theme_service->GetThemeSupplier())
    return TabStrip::GetTabSeparatorColor();

  bool dark_mode = dark_mode::GetActivePresearchDarkModeType() ==
                   dark_mode::PresearchDarkModeType::PRESEARCH_DARK_MODE_TYPE_DARK;
  return dark_mode ? SkColorSetRGB(0x39, 0x38, 0x38)
                   : SkColorSetRGB(0xBE, 0xBF, 0xBF);
}
