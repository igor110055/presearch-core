/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/frame/presearch_browser_view.h"

#include <utility>

#include "presearch/browser/sparkle_buildflags.h"
#include "presearch/browser/translate/buildflags/buildflags.h"
#include "presearch/browser/ui/views/toolbar/bookmark_button.h"
#include "presearch/browser/ui/views/toolbar/presearch_toolbar_view.h"
#include "extensions/buildflags/buildflags.h"
#include "ui/events/event_observer.h"
#include "ui/views/event_monitor.h"

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "presearch/browser/ui/presearch_browser.h"
#include "presearch/browser/ui/sidebar/sidebar_utils.h"
#include "presearch/browser/ui/views/frame/presearch_contents_layout_manager.h"
#include "presearch/browser/ui/views/sidebar/sidebar_container_view.h"
#include "chrome/browser/ui/views/frame/contents_layout_manager.h"
#include "ui/views/layout/fill_layout.h"
#endif

#if BUILDFLAG(ENABLE_SPARKLE)
#include "presearch/browser/ui/views/update_recommended_message_box_mac.h"
#endif

#if BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_EXTENSION)
#include "extensions/browser/extension_registry.h"
#include "extensions/common/constants.h"
#endif

class PresearchBrowserView::TabCyclingEventHandler : public ui::EventObserver,
                                                 public views::WidgetObserver {
 public:
  explicit TabCyclingEventHandler(PresearchBrowserView* browser_view)
      : browser_view_(browser_view) {
    Start();
  }

  ~TabCyclingEventHandler() override {
    Stop();
  }

  TabCyclingEventHandler(const TabCyclingEventHandler&) = delete;
  TabCyclingEventHandler& operator=(const TabCyclingEventHandler&) = delete;

 private:
  // ui::EventObserver overrides:
  void OnEvent(const ui::Event& event) override {
    if (event.type() == ui::ET_KEY_RELEASED &&
        event.AsKeyEvent()->key_code() == ui::VKEY_CONTROL) {
      // Ctrl key was released, stop the tab cycling
      Stop();
      return;
    }

    if (event.type() == ui::ET_MOUSE_PRESSED)
      Stop();
  }

  // views::WidgetObserver overrides:
  void OnWidgetActivationChanged(views::Widget* widget, bool active) override {
    // We should stop cycling if other application gets active state.
    if (!active)
      Stop();
  }

  // Handle Browser widget closing while tab Cycling is in-progress.
  void OnWidgetClosing(views::Widget* widget) override {
    Stop();
  }

  void Start() {
    // Add the event handler
    auto* widget = browser_view_->GetWidget();
    if (widget->GetNativeWindow()) {
      monitor_ = views::EventMonitor::CreateWindowMonitor(
          this,
          widget->GetNativeWindow(),
          {ui::ET_MOUSE_PRESSED, ui::ET_KEY_RELEASED});
    }

    widget->AddObserver(this);
  }

  void Stop() {
    if (!monitor_.get())
      // We already stopped
      return;

    // Remove event handler
    auto* widget = browser_view_->GetWidget();
    monitor_.reset();
    widget->RemoveObserver(this);
    browser_view_->StopTabCycling();
  }

  PresearchBrowserView* browser_view_;
  std::unique_ptr<views::EventMonitor> monitor_;
};

PresearchBrowserView::PresearchBrowserView(std::unique_ptr<Browser> browser)
    : BrowserView(std::move(browser)) {
#if BUILDFLAG(ENABLE_SIDEBAR)
  // Only normal window (tabbed) should have sidebar.
  if (!sidebar::CanUseSidebar(browser_->profile()) ||
      !browser_->is_type_normal()) {
    return;
  }

  auto presearch_contents_container = std::make_unique<views::View>();

  // Wrap |contents_container_| within our new |presearch_contents_container_|.
  // |presearch_contents_container_| also contains sidebar.
  auto orignal_contents_container = RemoveChildViewT(contents_container_);
  sidebar_container_view_ = presearch_contents_container->AddChildView(
      std::make_unique<SidebarContainerView>(
          static_cast<PresearchBrowser*>(browser_.get())));
  original_contents_container_ = presearch_contents_container->AddChildView(
      std::move(orignal_contents_container));
  presearch_contents_container->SetLayoutManager(
      std::make_unique<PresearchContentsLayoutManager>(
          sidebar_container_view_, original_contents_container_));
  contents_container_ = AddChildView(std::move(presearch_contents_container));
  set_contents_view(contents_container_);

  // Make sure |find_bar_host_view_| is the last child of BrowserView by
  // re-ordering. FindBarHost widgets uses this view as a  kHostViewKey.
  // See the comments of BrowserView::find_bar_host_view().
  ReorderChildView(find_bar_host_view_, -1);
#endif
}

PresearchBrowserView::~PresearchBrowserView() {
  tab_cycling_event_handler_.reset();
  DCHECK(!tab_cycling_event_handler_);
}

#if BUILDFLAG(ENABLE_SIDEBAR)
sidebar::Sidebar* PresearchBrowserView::InitSidebar() {
  // Start Sidebar UI initialization.
  DCHECK(sidebar_container_view_);
  sidebar_container_view_->Init();
  return sidebar_container_view_;
}

ContentsLayoutManager* PresearchBrowserView::GetContentsLayoutManager() const {
  if (sidebar::CanUseSidebar(browser_->profile()) &&
      browser_->is_type_normal()) {
    return static_cast<ContentsLayoutManager*>(
        original_contents_container_->GetLayoutManager());
  }

  return BrowserView::GetContentsLayoutManager();
}
#endif

void PresearchBrowserView::SetStarredState(bool is_starred) {
  BookmarkButton* button =
      static_cast<PresearchToolbarView*>(toolbar())->bookmark_button();
  if (button)
    button->SetToggled(is_starred);
}

void PresearchBrowserView::ShowUpdateChromeDialog() {
#if BUILDFLAG(ENABLE_SPARKLE)
  // On mac, sparkle frameworks's relaunch api is used.
  UpdateRecommendedMessageBoxMac::Show(GetNativeWindow());
#else
  BrowserView::ShowUpdateChromeDialog();
#endif
}

// The translate bubble will be shown if ENABLE_PRESEARCH_TRANSLATE_GO or
// ENABLE_PRESEARCH_TRANSLATE_EXTENSIONS build flag is enabled and Google Translate
// is not installed. In ENABLE_PRESEARCH_TRANSLATE case, we utilize chromium's
// translate UI directly along with go-translate. In
// ENABLE_PRESEARCH_TRANSLATE_EXTENSION case, we repurpose the translate bubble to
// offer Google Translate extension installation, and the bubble will only be
// shown when Google Translate is not installed.
ShowTranslateBubbleResult PresearchBrowserView::ShowTranslateBubble(
    content::WebContents* web_contents,
    translate::TranslateStep step,
    const std::string& source_language,
    const std::string& target_language,
    translate::TranslateErrors::Type error_type,
    bool is_user_gesture) {
#if BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_GO)
  return BrowserView::ShowTranslateBubble(web_contents,
                                          step,
                                          source_language,
                                          target_language,
                                          error_type,
                                          is_user_gesture);
#elif BUILDFLAG(ENABLE_PRESEARCH_TRANSLATE_EXTENSION)
  if (!extensions::ExtensionRegistry::Get(GetProfile())
      ->GetInstalledExtension(google_translate_extension_id)) {
    return BrowserView::ShowTranslateBubble(web_contents,
                                            step,
                                            source_language,
                                            target_language,
                                            error_type,
                                            is_user_gesture);
  }
#endif
  return ShowTranslateBubbleResult::BROWSER_WINDOW_NOT_VALID;
}

void PresearchBrowserView::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  BrowserView::OnTabStripModelChanged(tab_strip_model, change, selection);

  if (change.type() != TabStripModelChange::kSelectionOnly) {
    // Stop tab cycling if tab is closed dusing the cycle.
    // This can happen when tab is closed by shortcut (ex, ctrl + F4).
    // After stopping, current tab cycling, new tab cycling will be started.
    StopTabCycling();
  }
}

void PresearchBrowserView::StartTabCycling() {
  tab_cycling_event_handler_ = std::make_unique<TabCyclingEventHandler>(this);
}

void PresearchBrowserView::StopTabCycling() {
  tab_cycling_event_handler_.reset();
  static_cast<PresearchTabStripModel*>(browser()->tab_strip_model())->
      StopMRUCycling();
}
