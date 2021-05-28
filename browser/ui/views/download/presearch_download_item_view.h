/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_DOWNLOAD_PRESEARCH_DOWNLOAD_ITEM_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_DOWNLOAD_PRESEARCH_DOWNLOAD_ITEM_VIEW_H_

#include "presearch/browser/download/presearch_download_item_model.h"
#include "chrome/browser/ui/views/download/download_item_view.h"

// The purpose of this subclass is to add URL origin and lock icon to the
// download item view (shown in the download shelf).
class PresearchDownloadItemView : public DownloadItemView {
 public:
  PresearchDownloadItemView(DownloadUIModel::DownloadUIModelPtr download,
      DownloadShelfView* parent, views::View* accessible_alert);
  ~PresearchDownloadItemView() override;

  // views::View:
  void Layout() override;
  gfx::Size CalculatePreferredSize() const override;

  // download::DownloadItem::Observer:
  void OnDownloadUpdated() override;

 protected:
  // views::View:
  void OnPaint(gfx::Canvas* canvas) override;

 private:
  // Padding between URL lock icon and URL text.
  static constexpr int kOriginURLIconRightPadding = 2;

  // Vertical padding between text lines.
  static constexpr int kPresearchVerticalTextPadding = 3;

  // These functions calculate the vertical coordinates for each text line.
  int GetYForFilenameText() const;
  int GetYForOriginURLText() const;
  int GetYForStatusText() const;

  // Painting of various download item bits.
  void DrawOriginURL(gfx::Canvas* canvas);
  void DrawLockIcon(gfx::Canvas* canvas);

  // Returns a square lock icon of |height|.
  gfx::ImageSkia GetLockIcon(int height);

  // Overrides the accessible name construction to reflect the origin URL.
  void SetMode(Mode mode) override;

  // Presearch download item model.
  PresearchDownloadItemModel presearch_model_;

  // The font list used to draw the origin URL text.
  gfx::FontList origin_url_font_list_;

  // Origin url text.
  base::string16 origin_url_text_;
  bool is_origin_url_secure_;

  DISALLOW_COPY_AND_ASSIGN(PresearchDownloadItemView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_DOWNLOAD_PRESEARCH_DOWNLOAD_ITEM_VIEW_H_
