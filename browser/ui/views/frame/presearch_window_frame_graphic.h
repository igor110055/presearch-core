/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_WINDOW_FRAME_GRAPHIC_H_
#define PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_WINDOW_FRAME_GRAPHIC_H_

namespace gfx {
class Canvas;
class Rect;
}  // namespace gfx

namespace content {
class BrowserContext;
}  // namespace content

class PresearchWindowFrameGraphic {
 public:
  explicit PresearchWindowFrameGraphic(content::BrowserContext* context);
  virtual ~PresearchWindowFrameGraphic();

  PresearchWindowFrameGraphic(const PresearchWindowFrameGraphic&) = delete;
  PresearchWindowFrameGraphic& operator=(const PresearchWindowFrameGraphic&) = delete;

  void Paint(gfx::Canvas* canvas, const gfx::Rect& frame_bounds);

 private:
  const bool is_tor_window_;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_WINDOW_FRAME_GRAPHIC_H_
