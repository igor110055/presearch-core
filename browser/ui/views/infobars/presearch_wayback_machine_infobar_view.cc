/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/infobars/presearch_wayback_machine_infobar_view.h"

#include <string>
#include <utility>

#include "presearch/browser/infobars/presearch_wayback_machine_delegate_impl.h"
#include "presearch/browser/ui/views/infobars/presearch_wayback_machine_infobar_contents_view.h"
#include "presearch/components/presearch_wayback_machine/presearch_wayback_machine_infobar_delegate.h"

// static
std::unique_ptr<infobars::InfoBar>
PresearchWaybackMachineDelegateImpl::CreateInfoBarView(
    std::unique_ptr<PresearchWaybackMachineInfoBarDelegate> delegate,
    content::WebContents* contents) {
  return std::make_unique<PresearchWaybackMachineInfoBarView>(std::move(delegate),
                                                          contents);
}

PresearchWaybackMachineInfoBarView::PresearchWaybackMachineInfoBarView(
      std::unique_ptr<PresearchWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents)
    : InfoBarView(std::move(delegate)) {
  sub_views_ = new PresearchWaybackMachineInfoBarContentsView(contents);
  sub_views_->SizeToPreferredSize();
  AddChildView(sub_views_);
}

PresearchWaybackMachineInfoBarView::~PresearchWaybackMachineInfoBarView() {
}

void PresearchWaybackMachineInfoBarView::Layout() {
  InfoBarView::Layout();
  // |sub_views_| occupies from the beginning.
  // Don't adjust child view's height. Just use their preferred height.
  // It can cause infinite Layout loop because of infobar's height
  // re-calculation during the animation.
  sub_views_->SetBounds(0, OffsetY(sub_views_), GetEndX(),
                        sub_views_->height());
}
