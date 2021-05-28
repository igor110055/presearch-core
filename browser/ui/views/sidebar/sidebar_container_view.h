/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_CONTAINER_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_CONTAINER_VIEW_H_

#include "base/scoped_observer.h"
#include "presearch/browser/ui/sidebar/sidebar.h"
#include "presearch/browser/ui/sidebar/sidebar_model.h"
#include "ui/views/view.h"

namespace views {
class WebView;
}  // namespace views

class PresearchBrowser;

class SidebarControlView;

// This view is the parent view of all sidebar ui.
// Thi will include sidebar items button, add button, settings button and panel
// view.
class SidebarContainerView : public sidebar::Sidebar,
                             public views::View,
                             public sidebar::SidebarModel::Observer {
 public:
  explicit SidebarContainerView(PresearchBrowser* browser);
  ~SidebarContainerView() override;

  SidebarContainerView(const SidebarContainerView&) = delete;
  SidebarContainerView& operator=(const SidebarContainerView&) = delete;

  void Init();

  // Sidebar overrides:
  void ShowSidebar(bool show) override;
  void UpdateSidebar() override;

  // views::View overrides:
  void Layout() override;
  gfx::Size CalculatePreferredSize() const override;
  void OnThemeChanged() override;

  // sidebar::SidebarModel::Observer overrides:
  void OnActiveIndexChanged(int old_index, int new_index) override;

 private:
  void AddChildViews();
  void UpdateBackgroundAndBorder();
  void UpdateChildViewVisibility();

  PresearchBrowser* browser_ = nullptr;
  sidebar::SidebarModel* sidebar_model_ = nullptr;
  views::WebView* sidebar_panel_view_ = nullptr;
  SidebarControlView* sidebar_control_view_ = nullptr;
  bool initialized_ = false;
  ScopedObserver<sidebar::SidebarModel, sidebar::SidebarModel::Observer>
      observed_{this};
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_CONTAINER_VIEW_H_
