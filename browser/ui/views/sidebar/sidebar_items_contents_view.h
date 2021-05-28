/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ITEMS_CONTENTS_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ITEMS_CONTENTS_VIEW_H_

#include <memory>

#include "presearch/browser/ui/sidebar/sidebar_model.h"
#include "presearch/browser/ui/views/sidebar/sidebar_button_view.h"
#include "presearch/components/sidebar/sidebar_item.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/view.h"

namespace views {
class MenuRunner;
}  // namespace views

class PresearchBrowser;
class SidebarItemView;

class SidebarItemsContentsView : public views::View,
                                 public SidebarButtonView::Delegate,
                                 public views::ContextMenuController,
                                 public ui::SimpleMenuModel::Delegate {
 public:
  SidebarItemsContentsView(PresearchBrowser* browser,
                           views::DragController* drag_controller);
  ~SidebarItemsContentsView() override;

  SidebarItemsContentsView(const SidebarItemsContentsView&) = delete;
  SidebarItemsContentsView operator=(const SidebarItemsContentsView&) = delete;

  // views::View overrides:
  gfx::Size CalculatePreferredSize() const override;
  void OnThemeChanged() override;

  // SidebarButtonView::Delegate overrides:
  base::string16 GetTooltipTextFor(const views::View* view) const override;

  // views::ContextMenuController overrides:
  void ShowContextMenuForViewImpl(views::View* source,
                                  const gfx::Point& point,
                                  ui::MenuSourceType source_type) override;

  // ui::SimpleMenuModel::Delegate overrides:
  void ExecuteCommand(int command_id, int event_flags) override;

  void OnItemAdded(const sidebar::SidebarItem& item,
                   int index,
                   bool user_gesture);
  void OnItemMoved(const sidebar::SidebarItem& item, int from, int to);
  void OnItemRemoved(int index);
  void OnActiveIndexChanged(int old_index, int new_index);

  void ShowItemAddedFeedbackBubble();

  void SetImageForItem(const sidebar::SidebarItem& item,
                       const gfx::ImageSkia& image);

  // |source| is drag source view.
  // |position| is in local coordinate space of |source|.
  // Returns drag indicator index.
  int DrawDragIndicator(views::View* source, const gfx::Point& position);
  void ClearDragIndicator();

 private:
  enum ContextMenuIDs {
    kItemRemove,
  };

  void AddItemView(const sidebar::SidebarItem& item,
                   int index,
                   bool user_gesture);
  void UpdateItemViewStateAt(int index, bool active);
  void SetDefaultImageAt(int index, const sidebar::SidebarItem& item);
  bool IsBuiltInTypeItemView(views::View* view) const;

  // Called when each item is pressed.
  void OnItemPressed(const views::View* item);

  void OnContextMenuClosed();

  gfx::ImageSkia GetImageForBuiltInItems(const GURL& item_url,
                                         bool focus) const;
  void UpdateAllBuiltInItemsViewState();
  void ShowItemAddedFeedbackBubble(views::View* anchor_view);

  // When item count is five, drag indicator is drawn in front of first item.
  // If |index| is 5, it's drawn after the last item.
  // Pass -1 to remove indicator.
  void DoDrawDragIndicator(int index);
  int CalculateTargetDragIndicatorIndex(const gfx::Point& screen_position);
  SidebarItemView* GetItemViewAt(int index);

  PresearchBrowser* browser_ = nullptr;
  views::DragController* drag_controller_ = nullptr;
  views::View* view_for_context_menu_ = nullptr;
  sidebar::SidebarModel* sidebar_model_ = nullptr;
  std::unique_ptr<ui::SimpleMenuModel> context_menu_model_;
  std::unique_ptr<views::MenuRunner> context_menu_runner_;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ITEMS_CONTENTS_VIEW_H_
