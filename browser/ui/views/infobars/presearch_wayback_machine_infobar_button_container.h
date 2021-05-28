/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_BUTTON_CONTAINER_H_
#define PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_BUTTON_CONTAINER_H_

#include "ui/views/view.h"
#include "ui/views/controls/button/button.h"

class PresearchWaybackMachineInfoBarThrobber;

// This manages button and throbber controls.
// buttons occupies all this containers area and throbber runs over the button.
// When throbbing is requested, button extends its right inset and throbber runs
// on that area.
class PresearchWaybackMachineInfoBarButtonContainer : public views::View {
 public:
  explicit PresearchWaybackMachineInfoBarButtonContainer(
      views::Button::PressedCallback callback);
  ~PresearchWaybackMachineInfoBarButtonContainer() override;

  PresearchWaybackMachineInfoBarButtonContainer(
      const PresearchWaybackMachineInfoBarButtonContainer&) = delete;
  PresearchWaybackMachineInfoBarButtonContainer& operator=(
      const PresearchWaybackMachineInfoBarButtonContainer&) = delete;

  void StartThrobber();
  void StopThrobber();

  // views::View overrides:
  void Layout() override;
  gfx::Size CalculatePreferredSize() const override;

 private:
  void AdjustButtonInsets(bool add_insets);

  PresearchWaybackMachineInfoBarThrobber* throbber_ = nullptr;
  views::View* button_ = nullptr;
};

#endif  // PRESEARCH_BROWSER_UI_VIEWS_INFOBARS_PRESEARCH_WAYBACK_MACHINE_INFOBAR_BUTTON_CONTAINER_H_
