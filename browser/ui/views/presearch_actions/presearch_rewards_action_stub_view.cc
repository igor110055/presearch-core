// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/browser/ui/views/presearch_actions/presearch_rewards_action_stub_view.h"

#include <string>
#include <memory>
#include <utility>

#include "presearch/browser/ui/presearch_actions/presearch_action_icon_with_badge_image_source.h"  // NOLINT
#include "presearch/components/presearch_rewards/common/pref_names.h"
#include "presearch/components/presearch_rewards/resources/extension/grit/presearch_rewards_extension_resources.h"  // NOLINT
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "components/prefs/pref_service.h"
#include "extensions/common/constants.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_rep_default.h"
#include "ui/gfx/image/canvas_image_source.h"
#include "ui/views/view.h"
#include "ui/views/view_class_properties.h"
#include "ui/views/animation/ink_drop_impl.h"
#include "ui/views/controls/button/label_button_border.h"
#include "ui/views/controls/highlight_path_generator.h"

namespace {

constexpr SkColor kRewardsBadgeBg = SkColorSetRGB(0x2d, 0x8e, 0xff);

class PresearchRewardsActionStubViewHighlightPathGenerator
      : public views::HighlightPathGenerator {
 public:
  PresearchRewardsActionStubViewHighlightPathGenerator() = default;

  // HighlightPathGenerator
  SkPath GetHighlightPath(const views::View* view) override {
    return static_cast<const PresearchRewardsActionStubView*>(view)
        ->GetHighlightPath();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchRewardsActionStubViewHighlightPathGenerator);
};

}  // namespace

PresearchRewardsActionStubView::PresearchRewardsActionStubView(
    Profile* profile,
    PresearchRewardsActionStubView::Delegate* delegate)
    : LabelButton(
          base::BindRepeating(&PresearchRewardsActionStubView::ButtonPressed,
                              base::Unretained(this)),
          base::string16()),
      profile_(profile),
      delegate_(delegate) {
  SetInkDropMode(InkDropMode::ON);
  SetHasInkDropActionOnClick(true);
  SetHorizontalAlignment(gfx::ALIGN_CENTER);
  SetInkDropVisibleOpacity(kToolbarInkDropVisibleOpacity);
  // Create badge-and-image source like an extension icon would
  auto preferred_size = GetPreferredSize();
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  std::unique_ptr<IconWithBadgeImageSource> image_source(
      new PresearchActionIconWithBadgeImageSource(preferred_size));
  // Set icon on badge using actual extension icon resource
  gfx::ImageSkia image;
  const SkBitmap bitmap = rb.GetImageNamed(IDR_PRESEARCH_REWARDS_ICON_64)
      .AsBitmap();
  float scale = static_cast<float>(bitmap.width()) / kPresearchActionGraphicSize;
  image.AddRepresentation(gfx::ImageSkiaRep(bitmap, scale));
  image_source->SetIcon(gfx::Image(image));
  // Set text on badge
  std::unique_ptr<IconWithBadgeImageSource::Badge> badge;
  // TODO(petemill): Provide an observer if this value is expected to change
  // during runtime. At time of implementation, this would only be different
  // after a restart.
  badge_text_pref_.Init(
    presearch_rewards::prefs::kBadgeText, profile->GetPrefs());
  badge.reset(new IconWithBadgeImageSource::Badge(
          badge_text_pref_.GetValue(),
          SK_ColorWHITE,
          kRewardsBadgeBg));
  image_source->SetBadge(std::move(badge));
  image_source->set_paint_page_action_decoration(false);
  gfx::ImageSkia icon(gfx::Image(
      gfx::ImageSkia(
          std::move(image_source),
          preferred_size))
      .AsImageSkia());
  // Use badge-and-icon source for button's image in all states
  SetImage(views::Button::STATE_NORMAL, icon);
  // Install highlight path generator
  views::HighlightPathGenerator::Install(
      this,
      std::make_unique<PresearchRewardsActionStubViewHighlightPathGenerator>());
}

PresearchRewardsActionStubView::~PresearchRewardsActionStubView() {}

SkPath PresearchRewardsActionStubView::GetHighlightPath() const {
  // Set the highlight path for the toolbar button,
  // making it inset so that the badge can show outside it in the
  // fake margin on the right that we are creating.
  gfx::Insets highlight_insets(0, 0, 0, kPresearchActionRightMargin);
  gfx::Rect rect(GetPreferredSize());
  rect.Inset(highlight_insets);
  const int radii = ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
      views::EMPHASIS_MAXIMUM, rect.size());
  SkPath path;
  path.addRoundRect(gfx::RectToSkRect(rect), radii, radii);
  return path;
}

void PresearchRewardsActionStubView::ButtonPressed() {
  // We only show the default badge text once, so once the button
  // is clicked then change it back. We consider pressing the button
  // as an action to 'dismiss' the badge notification.
  // This cannot be done from the rewards service since it is not
  // involved in showing the pre-opt-in panel.
  if (badge_text_pref_.GetValue() != "") {
    profile_->GetPrefs()->SetString(presearch_rewards::prefs::kBadgeText,
        "");
  }
  delegate_->OnRewardsStubButtonClicked();
}

gfx::Size PresearchRewardsActionStubView::CalculatePreferredSize() const {
  return delegate_->GetToolbarActionSize();
}

std::unique_ptr<views::LabelButtonBorder> PresearchRewardsActionStubView::
    CreateDefaultBorder() const {
  std::unique_ptr<views::LabelButtonBorder> border =
      LabelButton::CreateDefaultBorder();
  border->set_insets(
      gfx::Insets(0, 0, 0, 0));
  return border;
}

SkColor PresearchRewardsActionStubView::GetInkDropBaseColor() const {
  return GetToolbarInkDropBaseColor(this);
}

std::unique_ptr<views::InkDropHighlight>
PresearchRewardsActionStubView::CreateInkDropHighlight() const {
  return CreateToolbarInkDropHighlight(this);
}
