/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/download/presearch_download_item_view.h"

#include <algorithm>
#include <utility>

#include "base/auto_reset.h"
#include "base/strings/string16.h"
#include "presearch/app/vector_icons/vector_icons.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/views/download/download_shelf_view.h"
#include "components/strings/grit/components_strings.h"
#include "components/vector_icons/vector_icons.h"
#include "content/public/common/origin_util.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/theme_provider.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/text_elider.h"
#include "ui/gfx/text_utils.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/styled_label.h"

using download::DownloadItem;

namespace {

constexpr int kTextWidth = 140;

// Padding before the icon and at end of the item.
constexpr int kStartPadding = 12;

// Horizontal padding between progress indicator and filename/status text.
constexpr int kProgressTextPadding = 8;

// Size of the space used for the progress indicator.
constexpr int kProgressIndicatorSize = 25;

// The minimum vertical padding above and below contents of the download item.
constexpr int kMinimumVerticalPadding = 6;

// The normal height of the item which may be exceeded if text is large.
constexpr int kDefaultHeight = 65;

// Lock icon color.
constexpr SkColor kDownloadUnlockIconColor = SkColorSetRGB(0xC6, 0x36, 0x26);

// Decrement of lock icon height from font baseline
constexpr int kDownloadUnlockIconHeightDecr = 1;

}  // namespace

PresearchDownloadItemView::PresearchDownloadItemView(
    DownloadUIModel::DownloadUIModelPtr download,
    DownloadShelfView* parent,
    views::View* accessible_alert)
    : DownloadItemView(std::move(download), parent, accessible_alert),
      presearch_model_(model_.get()),
      is_origin_url_secure_(false) {
  // Prepare origin url font.
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  origin_url_font_list_ =
      rb.GetFontList(ui::ResourceBundle::BaseFont).DeriveWithSizeDelta(-1);
}

PresearchDownloadItemView::~PresearchDownloadItemView() {}

// View overrides.

void PresearchDownloadItemView::Layout() {
  DownloadItemView::Layout();
  // Adjust the position of the status text label.
  if (!IsShowingWarningDialog()) {
    file_name_label_->SetY(GetYForFilenameText());
    status_label_->SetY(GetYForStatusText());
  }
}

gfx::Size PresearchDownloadItemView::CalculatePreferredSize() const {
  // Call base class to get the width.
  gfx::Size size = DownloadItemView::CalculatePreferredSize();
  // Calculate the height accounting for the extra line.
  int child_height = file_name_label_->GetLineHeight() +
                     kPresearchVerticalTextPadding +
                     origin_url_font_list_.GetHeight() +
                     kPresearchVerticalTextPadding + status_label_->GetLineHeight();
  if (IsShowingWarningDialog()) {
    child_height = std::max(
        {child_height, GetButtonSize().height(), GetIcon().Size().width()});
  }
  size.set_height(
      std::max(kDefaultHeight, 2 * kMinimumVerticalPadding + child_height));
  return size;
}

void PresearchDownloadItemView::OnPaint(gfx::Canvas* canvas) {
  DownloadItemView::OnPaint(canvas);
  DrawOriginURL(canvas);
}

// download::DownloadItem::Observer overrides.

void PresearchDownloadItemView::OnDownloadUpdated() {
  // Check for conditions that would disregard origin url change and fall back
  // onto base implementation to handle them.
  if (!model()->ShouldShowInShelf() ||
      (model_->GetState() == DownloadItem::COMPLETE &&
       model()->ShouldRemoveFromShelfWhenComplete())) {
    DownloadItemView::OnDownloadUpdated();
    return;
  }

  if (IsShowingWarningDialog() != model()->IsDangerous()) {
    DownloadItemView::OnDownloadUpdated();
  } else {
    // Update origin url first so that if the base class triggers paint fast
    // enough the new origin url is used and UpdateAccessibleName can use it
    // as well.
    bool needs_repaint = false;
    bool new_is_secure = false;
    base::string16 new_origin_url =
        presearch_model_.GetOriginURLText(&new_is_secure);
    if (new_origin_url != origin_url_text_ ||
      new_is_secure != is_origin_url_secure_) {
      origin_url_text_ = new_origin_url;
      is_origin_url_secure_ = new_is_secure;
      needs_repaint = true;
    }

    DownloadItemView::OnDownloadUpdated();

    // Don't know if the base implementation triggered a repaint so trigger it
    // ourselves if we need to.
    if (needs_repaint)
      SchedulePaint();
  }

  // Update tooltip.
  base::string16 new_tip = presearch_model_.GetTooltipText();
  if (new_tip != tooltip_text_) {
    tooltip_text_ = new_tip;
    TooltipTextChanged();
  }
}

// Positioning routines.

int PresearchDownloadItemView::GetYForFilenameText() const {
  int text_height = file_name_label_->GetLineHeight();
  if (!origin_url_text_.empty())
    text_height +=
        kPresearchVerticalTextPadding + origin_url_font_list_.GetHeight();
  if (status_label_ && !status_label_->GetText().empty())
    text_height += kPresearchVerticalTextPadding + status_label_->GetLineHeight();
  return (height() - text_height) / 2;
}

int PresearchDownloadItemView::GetYForOriginURLText() const {
  int y = GetYForFilenameText();
  y += (file_name_label_->GetLineHeight() + kPresearchVerticalTextPadding);
  return y;
}

int PresearchDownloadItemView::GetYForStatusText() const {
  int y = GetYForOriginURLText();
  if (!origin_url_text_.empty())
    y += (origin_url_font_list_.GetHeight() + kPresearchVerticalTextPadding);
  return y;
}

// Drawing routines.

void PresearchDownloadItemView::DrawOriginURL(gfx::Canvas* canvas) {
  if (origin_url_text_.empty() || IsShowingWarningDialog())
    return;

  int x = kStartPadding + kProgressIndicatorSize + kProgressTextPadding;
  int text_width = kTextWidth;

  if (!is_origin_url_secure_) {
    DrawLockIcon(canvas);
    int dx = origin_url_font_list_.GetBaseline() + kOriginURLIconRightPadding;
    x += dx;
    text_width -= dx;
  }

  base::string16 originURL = gfx::ElideText(
      origin_url_text_, origin_url_font_list_, text_width, gfx::ELIDE_TAIL);
  int mirrored_x = GetMirroredXWithWidthInView(x, text_width);

  SkColor dimmed_text_color = SkColorSetA(
      GetThemeProvider()->GetColor(ThemeProperties::COLOR_BOOKMARK_TEXT), 0xC7);
  canvas->DrawStringRect(
      originURL, origin_url_font_list_, dimmed_text_color,
      gfx::Rect(mirrored_x, GetYForOriginURLText(), text_width,
                origin_url_font_list_.GetHeight()));
}

void PresearchDownloadItemView::DrawLockIcon(gfx::Canvas* canvas) {
  if (origin_url_text_.empty() || IsShowingWarningDialog())
    return;

  int mirrored_x = GetMirroredXWithWidthInView(
      kStartPadding + kProgressIndicatorSize + kProgressTextPadding,
      kTextWidth);

  // Get lock icon of the needed height.
  int dy = origin_url_font_list_.GetBaseline() - kDownloadUnlockIconHeightDecr;
  int y = GetYForOriginURLText() + (origin_url_font_list_.GetHeight() - dy) / 2;
  canvas->DrawImageInt(GetLockIcon(dy), mirrored_x, y);
}

// Get lock icon from vector icons.
gfx::ImageSkia PresearchDownloadItemView::GetLockIcon(int height) {
  return gfx::CreateVectorIcon(kDownloadUnlockIcon, height,
    kDownloadUnlockIconColor);
}

// Update accessible name with origin URL.
void PresearchDownloadItemView::SetMode(Mode mode) {
  DownloadItemView::SetMode(mode);
  if (IsShowingWarningDialog())
    return;

  if (!origin_url_text_.empty()) {
    base::string16 extra;
    if (!is_origin_url_secure_)
      extra += base::char16(' ')
                   + l10n_util::GetStringUTF16(IDS_NOT_SECURE_VERBOSE_STATE);
    extra += base::char16(' ') + origin_url_text_;
    accessible_name_ += extra;
  }
}
