// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_ACTIONS_PRESEARCH_REWARDS_ACTION_STUB_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_ACTIONS_PRESEARCH_REWARDS_ACTION_STUB_VIEW_H_

#include <memory>

#include "components/prefs/pref_member.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/view.h"

class Profile;

// A button to take the place of an extension that will be loaded in the future.
// Call SetImage with the PresearchActionIconWithBadgeImageSource
// Call highlight etc from ToolbarActionView
class PresearchRewardsActionStubView : public views::LabelButton {
 public:
  class Delegate {
   public:
    virtual void OnRewardsStubButtonClicked() = 0;
    virtual gfx::Size GetToolbarActionSize() = 0;
   protected:
    ~Delegate() {}
  };

  explicit PresearchRewardsActionStubView(Profile* profile, Delegate* delegate);
  ~PresearchRewardsActionStubView() override;

  // views::LabelButton:
  std::unique_ptr<views::LabelButtonBorder> CreateDefaultBorder()
      const override;
  SkColor GetInkDropBaseColor() const override;
  std::unique_ptr<views::InkDropHighlight> CreateInkDropHighlight()
      const override;

  SkPath GetHighlightPath() const;

 private:
  gfx::Size CalculatePreferredSize() const override;
  void ButtonPressed();

  StringPrefMember badge_text_pref_;
  Profile* profile_;
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(PresearchRewardsActionStubView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_PRESEARCH_ACTIONS_PRESEARCH_REWARDS_ACTION_STUB_VIEW_H_
