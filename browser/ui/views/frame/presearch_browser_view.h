/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_BROWSER_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_BROWSER_VIEW_H_

#include <memory>
#include <string>

#include "presearch/browser/ui/tabs/presearch_tab_strip_model.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/views/frame/browser_view.h"

#if BUILDFLAG(ENABLE_SIDEBAR)
class ContentsLayoutManager;
class SidebarContainerView;
#endif

class PresearchBrowserView : public BrowserView {
 public:
  explicit PresearchBrowserView(std::unique_ptr<Browser> browser);
  ~PresearchBrowserView() override;

  void SetStarredState(bool is_starred) override;
  void ShowUpdateChromeDialog() override;
  ShowTranslateBubbleResult ShowTranslateBubble(
      content::WebContents* web_contents,
      translate::TranslateStep step,
      const std::string& source_language,
      const std::string& target_language,
      translate::TranslateErrors::Type error_type,
      bool is_user_gesture) override;

  void StartTabCycling() override;

 private:
  class TabCyclingEventHandler;

  // BrowserView overrides:
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;

  void StopTabCycling();

#if BUILDFLAG(ENABLE_SIDEBAR)
  sidebar::Sidebar* InitSidebar() override;
  ContentsLayoutManager* GetContentsLayoutManager() const override;

  // If sidebar is enabled, |BrowserView::contents_container_| points to
  // |presearch_contents_container_| that includes sidebar and contents container.
  // |original_contents_container_| points to original contents container that
  // includes contents & devtools webview. It's used by
  // GetContentsLayoutManager().
  views::View* original_contents_container_ = nullptr;
  SidebarContainerView* sidebar_container_view_ = nullptr;
#endif

  std::unique_ptr<TabCyclingEventHandler> tab_cycling_event_handler_;

  DISALLOW_COPY_AND_ASSIGN(PresearchBrowserView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_FRAME_PRESEARCH_BROWSER_VIEW_H_
