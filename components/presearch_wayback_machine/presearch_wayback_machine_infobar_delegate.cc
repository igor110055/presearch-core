/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_wayback_machine/presearch_wayback_machine_infobar_delegate.h"

using InfoBarIdentifier = infobars::InfoBarDelegate::InfoBarIdentifier;

PresearchWaybackMachineInfoBarDelegate::
PresearchWaybackMachineInfoBarDelegate() = default;

PresearchWaybackMachineInfoBarDelegate::
~PresearchWaybackMachineInfoBarDelegate() = default;

InfoBarIdentifier PresearchWaybackMachineInfoBarDelegate::GetIdentifier() const {
  return WAYBACK_MACHINE_INFOBAR_DELEGATE;
}

bool PresearchWaybackMachineInfoBarDelegate::EqualsDelegate(
    infobars::InfoBarDelegate* delegate) const {
  return delegate->GetIdentifier() == GetIdentifier();
}
