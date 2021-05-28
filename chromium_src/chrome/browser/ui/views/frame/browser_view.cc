/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/tabs/presearch_browser_tab_strip_controller.h"
#include "presearch/browser/ui/views/tabs/presearch_tab_strip.h"
#include "presearch/browser/ui/views/toolbar/presearch_toolbar_view.h"
#include "chrome/browser/ui/views/frame/tab_strip_region_view.h"

#define ToolbarView PresearchToolbarView
#define BrowserTabStripController PresearchBrowserTabStripController
#define TabStrip PresearchTabStrip
#include "../../../../../../../chrome/browser/ui/views/frame/browser_view.cc"  // NOLINT
#undef ToolbarView
#undef BrowserTabStripController
#undef TabStrip
