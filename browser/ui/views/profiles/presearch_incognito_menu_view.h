/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_PROFILES_PRESEARCH_INCOGNITO_MENU_VIEW_H_
#define PRESEARCH_BROWSER_UI_VIEWS_PROFILES_PRESEARCH_INCOGNITO_MENU_VIEW_H_

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"

class PresearchIncognitoMenuView : public IncognitoMenuView {
 public:
  using IncognitoMenuView::IncognitoMenuView;
  ~PresearchIncognitoMenuView() override = default;

  // ProfileMenuViewBase:
  void BuildMenu() override;

 private:
  friend class IncognitoMenuView;

  // views::BubbleDialogDelegateView:
  base::string16 GetAccessibleWindowTitle() const override;

    // Button actions.
  void OnExitButtonClicked() override;

  void AddTorButton();
  void OnTorProfileButtonClicked();

  DISALLOW_COPY_AND_ASSIGN(PresearchIncognitoMenuView);
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_PROFILES_PRESEARCH_INCOGNITO_MENU_VIEW_H_
