/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMMON_PRESEARCH_SWITCHES_H_
#define PRESEARCH_COMMON_PRESEARCH_SWITCHES_H_

namespace switches {

// All switches in alphabetical order. The switches should be documented
// alongside the definition of their values in the .cc file.
extern const char kDisablePresearchExtension[];

extern const char kDisablePresearchRewardsExtension[];

extern const char kDisablePresearchUpdate[];

extern const char kDisableWebTorrentExtension[];

extern const char kDisablePresearchWaybackMachineExtension[];

extern const char kRewards[];

extern const char kDarkMode[];

extern const char kDisableMachineId[];

extern const char kDisableEncryptionWin[];

extern const char kEnableSmartTrackingProtection[];

extern const char kFastWidevineBundleUpdate[];

extern const char kComponentUpdateIntervalInSec[];

extern const char kDisableDnsOverHttps[];

}  // namespace switches

#endif  // PRESEARCH_COMMON_PRESEARCH_SWITCHES_H_
