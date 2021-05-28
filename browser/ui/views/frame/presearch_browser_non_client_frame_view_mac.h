/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
#define PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_

#include <memory>

#include "chrome/browser/ui/views/frame/browser_non_client_frame_view_mac.h"

class PresearchWindowFrameGraphic;

class PresearchBrowserNonClientFrameViewMac : public BrowserNonClientFrameViewMac {
 public:
  PresearchBrowserNonClientFrameViewMac(BrowserFrame* frame,
                                    BrowserView* browser_view);
  ~PresearchBrowserNonClientFrameViewMac() override;

  PresearchBrowserNonClientFrameViewMac(
      const PresearchBrowserNonClientFrameViewMac&) = delete;
  PresearchBrowserNonClientFrameViewMac& operator=(
      const PresearchBrowserNonClientFrameViewMac&) = delete;

 private:
  // BrowserNonClientFrameViewMac overrides:
  void OnPaint(gfx::Canvas* canvas) override;

  std::unique_ptr<PresearchWindowFrameGraphic> frame_graphic_;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
