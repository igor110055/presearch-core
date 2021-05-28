/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/frame/presearch_browser_frame.h"

#include "presearch/browser/profiles/profile_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"

PresearchBrowserFrame::PresearchBrowserFrame(BrowserView* browser_view)
    : BrowserFrame(browser_view),
      view_(browser_view) {}

// Tor/Guest profile should use DarkAura. If not, their native ui is affected by
// normal windows theme change.
const ui::NativeTheme* PresearchBrowserFrame::GetNativeTheme() const {
  if ((view_->browser()->profile()->IsIncognitoProfile() ||
       view_->browser()->profile()->IsTor() ||
       presearch::IsGuestProfile(view_->browser()->profile())) &&
      ThemeServiceFactory::GetForProfile(view_->browser()->profile())
          ->UsingDefaultTheme()) {
    return ui::NativeTheme::GetInstanceForDarkUI();
  }
  return views::Widget::GetNativeTheme();
}
