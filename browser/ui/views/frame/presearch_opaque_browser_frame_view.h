/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_OPAQUE_BROWSER_FRAME_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_OPAQUE_BROWSER_FRAME_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"

class PresearchWindowFrameGraphic;

class PresearchOpaqueBrowserFrameView : public OpaqueBrowserFrameView {
 public:
  PresearchOpaqueBrowserFrameView(BrowserFrame* frame,
                              BrowserView* browser_view,
                              OpaqueBrowserFrameViewLayout* layout);
  ~PresearchOpaqueBrowserFrameView() override;

  PresearchOpaqueBrowserFrameView(const PresearchOpaqueBrowserFrameView&) = delete;
  PresearchOpaqueBrowserFrameView& operator=(
      const PresearchOpaqueBrowserFrameView&) = delete;

  // PresearchOpaqueBrowserFrameView overrides:
  void OnPaint(gfx::Canvas* canvas) override;

 private:
  std::unique_ptr<PresearchWindowFrameGraphic> frame_graphic_;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_OPAQUE_BROWSER_FRAME_VIEW_H_
