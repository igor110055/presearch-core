/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_ACTIONS_PRESEARCH_ACTION_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_ACTIONS_PRESEARCH_ACTION_VIEW_H_

#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"

namespace gfx {
class Rect;
}

// Subclasses ToolbarActionViewc so that the notification badge can be painted
// outside the highlight bubble.
class PresearchActionView : public ToolbarActionView {
 public:
  PresearchActionView(ToolbarActionViewController* view_controller,
                  ToolbarActionView::Delegate* delegate);

  SkPath GetHighlightPath() const;

  DISALLOW_COPY_AND_ASSIGN(PresearchActionView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_ACTIONS_PRESEARCH_ACTION_VIEW_H_
