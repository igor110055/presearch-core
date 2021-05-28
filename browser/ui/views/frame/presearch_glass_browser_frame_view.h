/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_GLASS_BROWSER_FRAME_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_GLASS_BROWSER_FRAME_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/frame/glass_browser_frame_view.h"

class PresearchWindowFrameGraphic;

class PresearchGlassBrowserFrameView : public GlassBrowserFrameView {
 public:
  PresearchGlassBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  ~PresearchGlassBrowserFrameView() override;

  PresearchGlassBrowserFrameView(const PresearchGlassBrowserFrameView&) = delete;
  PresearchGlassBrowserFrameView& operator=(
      const PresearchGlassBrowserFrameView&) = delete;

 private:
  // GlassBrowserFrameView overrides:
  void OnPaint(gfx::Canvas* canvas) override;

  std::unique_ptr<PresearchWindowFrameGraphic> frame_graphic_;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_GLASS_BROWSER_FRAME_VIEW_H_
