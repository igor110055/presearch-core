/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/infobars/infobar_view.h"

namespace content {
class WebContents;
}

class PresearchWaybackMachineInfoBarDelegate;

class PresearchWaybackMachineInfoBarView : public InfoBarView {
 public:
  PresearchWaybackMachineInfoBarView(
      std::unique_ptr<PresearchWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents);
  ~PresearchWaybackMachineInfoBarView() override;

  PresearchWaybackMachineInfoBarView(
      const PresearchWaybackMachineInfoBarView&) = delete;
  PresearchWaybackMachineInfoBarView& operator=(
      const PresearchWaybackMachineInfoBarView&) = delete;

 private:
  // InfoBarView overrides:
  void Layout() override;

  views::View* sub_views_ = nullptr;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_VIEW_H_
