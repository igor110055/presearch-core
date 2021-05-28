/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/themes/presearch_theme_service.h"

#include "base/no_destructor.h"
#include "presearch/browser/themes/presearch_theme_helper.h"
#include "presearch/browser/extensions/presearch_theme_event_router.h"
#include "chrome/browser/profiles/profile.h"
#include "presearch/browser/profiles/profile_util.h"

#if defined(OS_WIN)
#include "presearch/browser/themes/presearch_theme_helper_win.h"
#endif

namespace {

const ThemeHelper& GetPresearchThemeHelper(Profile* profile) {
#if defined(OS_WIN)
  using PresearchThemeHelper = PresearchThemeHelperWin;
#endif
  // Because the helper is created as a NoDestructor static, we need separate
  // instances for regular, tor and guest profiles.
  if (profile->IsTor()) {
    static base::NoDestructor<std::unique_ptr<ThemeHelper>> dark_theme_helper(
        std::make_unique<PresearchThemeHelper>());
    (static_cast<PresearchThemeHelper*>(dark_theme_helper.get()->get()))
        ->set_is_tor();
    return **dark_theme_helper;
  } else if (presearch::IsGuestProfile(profile)) {
    static base::NoDestructor<std::unique_ptr<ThemeHelper>> dark_theme_helper(
        std::make_unique<PresearchThemeHelper>());
    (static_cast<PresearchThemeHelper*>(dark_theme_helper.get()->get()))
        ->set_is_guest();
    return **dark_theme_helper;
  } else {
    static base::NoDestructor<std::unique_ptr<ThemeHelper>> theme_helper(
        std::make_unique<PresearchThemeHelper>());
    return **theme_helper;
  }
}

}  // namespace

// Replace Chromium's ThemeHelper with PresearchThemeHelper that is appropriate for
// the given profile. There should only be 3 static ThemeHelpers at most: the
// original Chromium one, and 2 Presearch ones.
PresearchThemeService::PresearchThemeService(Profile* profile,
                                     const ThemeHelper& theme_helper)
    : ThemeService(profile, GetPresearchThemeHelper(profile)) {
  presearch_theme_event_router_.reset(
      new extensions::PresearchThemeEventRouter(profile));
}

PresearchThemeService::~PresearchThemeService() = default;

void PresearchThemeService::SetPresearchThemeEventRouterForTesting(
    extensions::PresearchThemeEventRouter* mock_router) {
  presearch_theme_event_router_.reset(mock_router);
}
