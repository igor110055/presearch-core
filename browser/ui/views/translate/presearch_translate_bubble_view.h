/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_TRANSLATE_PRESEARCH_TRANSLATE_BUBBLE_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_TRANSLATE_PRESEARCH_TRANSLATE_BUBBLE_VIEW_H_

#include <memory>
#include <utility>

#include "chrome/browser/ui/views/translate/translate_bubble_view.h"

#include "presearch/browser/translate/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"

class PresearchTranslateIconView;

// The purpose of this subclass is to repurpose the translate bubble to install
// google translate extension, this is only used when
// ENABLE_PRESEARCH_TRANSLATE_EXTENSION is true.
class PresearchTranslateBubbleView : public TranslateBubbleView {
 public:
  PresearchTranslateBubbleView(views::View* anchor_view,
                           std::unique_ptr<TranslateBubbleModel> model,
                           translate::TranslateErrors::Type error_type,
                           content::WebContents* web_contents);
  ~PresearchTranslateBubbleView() override;

  // views::BubbleDialogDelegateView methods.
  void Init() override;

  // views::View methods.
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;

  // LocationBarBubbleDelegateView methods.
  bool ShouldShowWindowTitle() const override;

 protected:
  virtual void InstallGoogleTranslate();

 private:
  friend class PresearchTranslateBubbleViewTest;
  std::unique_ptr<views::View> PresearchCreateViewBeforeTranslate();
  void DisableOfferTranslatePref();
  void ButtonPressed(ButtonID button_id);

  // Remove this. As we replace |translate_view_|, we should destroy after
  // replacing it. However, its child view(|tabbed_pane_|) is still referenced
  // from TranslateBubbleView. Keep to prevent leak.
  std::unique_ptr<views::View> removed_translate_view_;

  DISALLOW_COPY_AND_ASSIGN(PresearchTranslateBubbleView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_TRANSLATE_PRESEARCH_TRANSLATE_BUBBLE_VIEW_H_
