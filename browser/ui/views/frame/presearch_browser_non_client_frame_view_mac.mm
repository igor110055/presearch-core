/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/frame/presearch_browser_non_client_frame_view_mac.h"

#include "presearch/browser/ui/views/frame/presearch_window_frame_graphic.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/scoped_canvas.h"

PresearchBrowserNonClientFrameViewMac::PresearchBrowserNonClientFrameViewMac(
    BrowserFrame* frame, BrowserView* browser_view)
    : BrowserNonClientFrameViewMac(frame, browser_view) {
  frame_graphic_.reset(
      new PresearchWindowFrameGraphic(browser_view->browser()->profile()));
}

PresearchBrowserNonClientFrameViewMac::~PresearchBrowserNonClientFrameViewMac() = default;

void PresearchBrowserNonClientFrameViewMac::OnPaint(gfx::Canvas* canvas) {
  BrowserNonClientFrameViewMac::OnPaint(canvas);

  // Don't draw frame graphic over border outline.
  gfx::ScopedCanvas scoped_canvas(canvas);
  gfx::Rect bounds_to_frame_graphic(bounds());
  if (!IsFrameCondensed()) {
    // Native frame has 1px border outline.
    constexpr int kFrameBorderOutlineThickness = 1;
    bounds_to_frame_graphic.Inset(kFrameBorderOutlineThickness,
                                  kFrameBorderOutlineThickness);
    canvas->ClipRect(bounds_to_frame_graphic);
  }
  frame_graphic_->Paint(canvas, bounds_to_frame_graphic);
}
