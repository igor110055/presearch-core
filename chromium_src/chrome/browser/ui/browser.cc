/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/presearch_browser.h"
#include "presearch/browser/ui/presearch_browser_command_controller.h"
#include "presearch/browser/ui/presearch_browser_content_setting_bubble_model_delegate.h"
#include "presearch/browser/ui/tabs/presearch_tab_strip_model.h"
#include "presearch/browser/ui/toolbar/presearch_location_bar_model_delegate.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

#if !defined(OS_ANDROID)
#include "presearch/browser/ui/bookmark/presearch_bookmark_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "base/feature_list.h"
#include "presearch/components/sidebar/features.h"
#endif

#define PRESEARCH_BROWSER_CREATE return new PresearchBrowser(params);
#define BrowserContentSettingBubbleModelDelegate \
  PresearchBrowserContentSettingBubbleModelDelegate
#define BrowserCommandController PresearchBrowserCommandController
#define BrowserLocationBarModelDelegate PresearchLocationBarModelDelegate
#if !defined(OS_ANDROID)
#define BookmarkTabHelper PresearchBookmarkTabHelper
#endif

#include "../../../../../chrome/browser/ui/browser.cc"  // NOLINT
#undef BrowserLocationBarModelDelegate
#undef BrowserContentSettingBubbleModelDelegate
#undef BrowserCommandController
#undef PRESEARCH_BROWSER_CREATE

#if !defined(OS_ANDROID)
#undef BookmarkTabHelper
#endif
