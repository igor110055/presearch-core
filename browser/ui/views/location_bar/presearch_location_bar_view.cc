/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/location_bar/presearch_location_bar_view.h"

#include <memory>
#include <utility>

#include "presearch/app/vector_icons/vector_icons.h"
#include "presearch/browser/profiles/profile_util.h"
#include "presearch/browser/themes/presearch_theme_service.h"
#include "presearch/browser/ui/views/presearch_actions/presearch_actions_container.h"
#include "presearch/browser/ui/views/toolbar/presearch_toolbar_view.h"
#include "presearch/grit/presearch_theme_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/omnibox/omnibox_theme.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/grit/chromium_strings.h"
#include "components/grit/presearch_components_strings.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "components/version_info/channel.h"
#include "content/public/browser/navigation_entry.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/controls/highlight_path_generator.h"

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/browser/ui/views/location_bar/onion_location_view.h"
#endif
#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/browser/ui/views/location_bar/ipfs_location_view.h"
#include "presearch/components/ipfs/ipfs_constants.h"
#include "presearch/components/ipfs/ipfs_utils.h"
#endif

namespace {

class PresearchLocationBarViewFocusRingHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  PresearchLocationBarViewFocusRingHighlightPathGenerator() = default;

  // HighlightPathGenerator
  SkPath GetHighlightPath(const views::View* view) override {
    return static_cast<const PresearchLocationBarView*>(view)
        ->GetFocusRingHighlightPath();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchLocationBarViewFocusRingHighlightPathGenerator);
};

base::Optional<SkColor> GetFocusRingColor(Profile* profile) {
  constexpr SkColor kPrivateFocusRingColor = SkColorSetRGB(0xC6, 0xB3, 0xFF);
  constexpr SkColor kTorPrivateFocusRingColor = SkColorSetRGB(0xCF, 0xAB, 0xE2);
  if (presearch::IsRegularProfile(profile) || profile->IsGuestSession()) {
    // Don't update color.
    return base::nullopt;
  }
  if (profile->IsTor())
    return kTorPrivateFocusRingColor;

  // Private window.
  return kPrivateFocusRingColor;
}

}  // namespace

void PresearchLocationBarView::Init() {
  // base method calls Update and Layout
  LocationBarView::Init();
  // Change focus ring highlight path
  if (focus_ring_) {
    focus_ring_->SetPathGenerator(
        std::make_unique<
            PresearchLocationBarViewFocusRingHighlightPathGenerator>());
    if (const auto color = GetFocusRingColor(profile()))
      focus_ring_->SetColor(color.value());
  }
#if BUILDFLAG(ENABLE_TOR)
  onion_location_view_ = new OnionLocationView(browser_->profile());
  AddChildView(onion_location_view_);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ipfs_location_view_ = new IPFSLocationView(browser_->profile());
  AddChildView(ipfs_location_view_);
#endif

  // presearch action buttons
  presearch_actions_ = new PresearchActionsContainer(browser_, profile());
  presearch_actions_->Init();
  AddChildView(presearch_actions_);
  // Call Update again to cause a Layout
  Update(nullptr);

  // Stop slide animation for all content settings views icon.
  for (auto* content_setting_view : content_setting_views_)
    content_setting_view->disable_animation();
}

bool PresearchLocationBarView::ShouldShowIPFSLocationView() const {
#if BUILDFLAG(IPFS_ENABLED)
  const GURL& url = GetLocationBarModel()->GetURL();
  if (!ipfs::IsIPFSScheme(url) || !ipfs::IsIpfsEnabled(profile_) ||
      !ipfs::IsLocalGatewayConfigured(profile_))
    return false;

  return true;
#else
  return false;
#endif
}

void PresearchLocationBarView::Update(content::WebContents* contents) {
  // base Init calls update before our Init is run, so our children
  // may not be initialized yet
  if (presearch_actions_) {
    presearch_actions_->Update();
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_)
    onion_location_view_->Update(contents);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_)
    ipfs_location_view_->Update(contents);
#endif

  LocationBarView::Update(contents);

  if (!ShouldShowIPFSLocationView())
    return;
  // Secure display text for a page was set by chromium.
  // We do not want to override this.
  if (!GetLocationBarModel()->GetSecureDisplayText().empty())
    return;
  auto badge_text = l10n_util::GetStringUTF16(IDS_IPFS_BADGE_TITLE);
  location_icon_view()->SetLabel(badge_text);
}

ui::ImageModel PresearchLocationBarView::GetLocationIcon(
    LocationIconView::Delegate::IconFetchedCallback on_icon_fetched) const {
  if (!ShouldShowIPFSLocationView() ||
      !omnibox_view_->model()->ShouldShowCurrentPageIcon())
    return LocationBarView::GetLocationIcon(std::move(on_icon_fetched));

  auto& bundle = ui::ResourceBundle::GetSharedInstance();
  const auto& ipfs_logo = *bundle.GetImageSkiaNamed(IDR_PRESEARCH_IPFS_LOGO);
  return ui::ImageModel::FromImageSkia(ipfs_logo);
}

void PresearchLocationBarView::OnChanged() {
  if (presearch_actions_) {
    // Do not show actions whilst omnibar is open or url is being edited
    const bool should_hide =
        ShouldHidePageActionIcons() && !omnibox_view_->GetText().empty();
    presearch_actions_->SetShouldHide(should_hide);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_)
    onion_location_view_->Update(
        browser_->tab_strip_model()->GetActiveWebContents());
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_)
    ipfs_location_view_->Update(
        browser_->tab_strip_model()->GetActiveWebContents());
#endif

  // OnChanged calls Layout
  LocationBarView::OnChanged();
}

std::vector<views::View*> PresearchLocationBarView::GetTrailingViews() {
  std::vector<views::View*> views;
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_)
    views.push_back(onion_location_view_);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_)
    views.push_back(ipfs_location_view_);
#endif

  if (presearch_actions_)
    views.push_back(presearch_actions_);

  return views;
}

gfx::Size PresearchLocationBarView::CalculatePreferredSize() const {
  gfx::Size min_size = LocationBarView::CalculatePreferredSize();
  if (presearch_actions_ && presearch_actions_->GetVisible()) {
    const int presearch_actions_min = presearch_actions_->GetMinimumSize().width();
    const int extra_width = presearch_actions_min +
                              GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING);
    min_size.Enlarge(extra_width, 0);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_ && onion_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
        onion_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_ && ipfs_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
                            ipfs_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif

  return min_size;
}

void PresearchLocationBarView::OnThemeChanged() {
  LocationBarView::OnThemeChanged();

  if (!IsInitialized())
    return;

  Update(nullptr);
  RefreshBackground();
}

void PresearchLocationBarView::ChildPreferredSizeChanged(views::View* child) {
  LocationBarView::ChildPreferredSizeChanged(child);

  if (child != presearch_actions_)
    return;

  Layout();
}

int PresearchLocationBarView::GetBorderRadius() const {
  return ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
      views::EMPHASIS_HIGH, size());
}

SkPath PresearchLocationBarView::GetFocusRingHighlightPath() const {
  const SkScalar radius = GetBorderRadius();
  return SkPath().addRoundRect(gfx::RectToSkRect(GetLocalBounds()),
                               radius, radius);
}

ContentSettingImageView*
PresearchLocationBarView::GetContentSettingsImageViewForTesting(size_t idx) {
  DCHECK(idx < content_setting_views_.size());
  return content_setting_views_[idx];
}
