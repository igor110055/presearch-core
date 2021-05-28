/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WAYBACK_MACHINE_PRESEARCH_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WAYBACK_MACHINE_PRESEARCH_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_

#include <memory>

#include "components/infobars/core/infobar_delegate.h"

class PresearchWaybackMachineInfoBarDelegate : public infobars::InfoBarDelegate {
 public:
  PresearchWaybackMachineInfoBarDelegate();
  ~PresearchWaybackMachineInfoBarDelegate() override;

  PresearchWaybackMachineInfoBarDelegate(
      const PresearchWaybackMachineInfoBarDelegate&) = delete;
  PresearchWaybackMachineInfoBarDelegate& operator=(
      const PresearchWaybackMachineInfoBarDelegate&) = delete;

 private:
  // infobars::InfoBarDelegate overrides:
  InfoBarIdentifier GetIdentifier() const override;
  bool EqualsDelegate(
      infobars::InfoBarDelegate* delegate) const override;
};

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WAYBACK_MACHINE_PRESEARCH_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_
