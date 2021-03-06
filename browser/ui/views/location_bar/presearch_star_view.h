/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_LOCATION_BAR_PRESEARCH_STAR_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_LOCATION_BAR_PRESEARCH_STAR_VIEW_H_

#include "chrome/browser/ui/views/location_bar/star_view.h"

class PresearchStarView : public StarView {
 public:
  using StarView::StarView;

 protected:
  // views::View:
  void UpdateImpl() override;

  DISALLOW_COPY_AND_ASSIGN(PresearchStarView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_LOCATION_BAR_PRESEARCH_STAR_VIEW_H_
