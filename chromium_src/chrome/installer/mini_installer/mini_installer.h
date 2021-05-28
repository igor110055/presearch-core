// Copyright (c) 2020 The Presearch Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_

#include "../../../../../chrome/installer/mini_installer/mini_installer.h"

#define StandardReferralCodeLen 6

namespace mini_installer {
typedef StackString<128> ReferralCodeString;

// Populates |referral_code| with a Presearch referral code if one is
// present in the installer filename. This may be a standard or an
// extended referral code.

bool ParseReferralCode(const wchar_t* installer_filename,
                       ReferralCodeString* referral_code);
}  // namespace mini_installer

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_INSTALLER_MINI_INSTALLER_MINI_INSTALLER_H_
