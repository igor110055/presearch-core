/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_

#include "presearch/browser/ui/presearch_browser_window.h"

#define PRESEARCH_BROWSER_VIEW_H friend class PresearchBrowserView;
#define BrowserWindow PresearchBrowserWindow
#define GetContentsLayoutManager     \
  GetContentsLayoutManager_Unused(); \
  virtual ContentsLayoutManager* GetContentsLayoutManager

#include "../../../../../../../chrome/browser/ui/views/frame/browser_view.h"
#undef PRESEARCH_BROWSER_VIEW_H
#undef BrowserWindow
#undef GetContentsLayoutManager

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
