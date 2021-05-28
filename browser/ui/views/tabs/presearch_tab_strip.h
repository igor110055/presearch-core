/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_TABS_PRESEARCH_TAB_STRIP_H_
#define PRESEARCH_BROWSER_UI_VIEWS_TABS_PRESEARCH_TAB_STRIP_H_

#include "chrome/browser/ui/views/tabs/tab_strip.h"

class PresearchTabStrip : public TabStrip {
 public:
  using TabStrip::TabStrip;
  ~PresearchTabStrip() override;
  PresearchTabStrip(const PresearchTabStrip&) = delete;
  PresearchTabStrip& operator=(const PresearchTabStrip&) = delete;

 private:
  // TabStrip overrides:
  bool ShouldHideCloseButtonForTab(Tab* tab) const override;
  SkColor GetTabSeparatorColor() const override;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_TABS_PRESEARCH_TAB_STRIP_H_
