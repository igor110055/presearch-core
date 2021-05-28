/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_

#include <vector>

#include "presearch/components/presearch_wayback_machine/wayback_machine_url_fetcher.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}  // namespace content

namespace views {
class ImageView;
class Label;
}  // namespace views

class PresearchWaybackMachineInfoBarButtonContainer;
class GURL;

// Includes all view controls except close button that managed by InfoBarView.
class PresearchWaybackMachineInfoBarContentsView
    : public views::View,
      public WaybackMachineURLFetcher::Client {
 public:
  explicit PresearchWaybackMachineInfoBarContentsView(
      content::WebContents* contents);
  ~PresearchWaybackMachineInfoBarContentsView() override;

  PresearchWaybackMachineInfoBarContentsView(
      const PresearchWaybackMachineInfoBarContentsView&) = delete;
  PresearchWaybackMachineInfoBarContentsView& operator=(
      const PresearchWaybackMachineInfoBarContentsView&) = delete;

 private:
  using Labels = std::vector<views::Label*>;
  using Views = std::vector<views::View*>;

  // views::View overrides:
  void OnThemeChanged() override;

  // WaybackMachineURLFetcher::Client overrides:
  void OnWaybackURLFetched(const GURL& latest_wayback_url) override;

  void InitializeChildren();
  views::Label* CreateLabel(const base::string16& text);
  SkColor GetColor(int id) const;
  void UpdateChildrenVisibility(bool show_before_checking_views);
  void FetchWaybackURL();
  void LoadURL(const GURL& url);
  void HideInfobar();

  void ButtonPressed();

  // Used for labels theme changing all together.
  Labels labels_;
  Views views_visible_before_checking_;
  Views views_visible_after_checking_;
  content::WebContents* contents_;
  WaybackMachineURLFetcher wayback_machine_url_fetcher_;

  PresearchWaybackMachineInfoBarButtonContainer* button_ = nullptr;
  views::ImageView* wayback_spot_graphic_ = nullptr;
  bool wayback_url_fetch_requested_ = false;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_
