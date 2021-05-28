/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PREF_NAMES_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PREF_NAMES_H_

extern const char kPresearchWalletAES256GCMSivNonce[];
extern const char kPresearchWalletEncryptedSeed[];
// Deprecated in favor of kPresearchWalletWeb3Provider
extern const char kPresearchWalletEnabledDeprecated[];
extern const char kPresearchWalletPrefVersion[];
extern const char kPresearchWalletWeb3Provider[];
extern const char kLoadCryptoWalletsOnStartup[];
extern const char kOptedIntoCryptoWallets[];

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PREF_NAMES_H_
