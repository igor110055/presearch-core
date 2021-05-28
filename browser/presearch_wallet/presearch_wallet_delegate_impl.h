/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_WALLET_PRESEARCH_WALLET_DELEGATE_IMPL_H_
#define PRESEARCH_BROWSER_PRESEARCH_WALLET_PRESEARCH_WALLET_DELEGATE_IMPL_H_

#include "presearch/components/presearch_wallet/browser/presearch_wallet_delegate.h"

class PresearchWalletDelegateImpl : public PresearchWalletDelegate {
 public:
  ~PresearchWalletDelegateImpl() override;
  void MaybeLoadCryptoWalletsExtension(
      content::BrowserContext* context) override;
};

#endif  // PRESEARCH_BROWSER_PRESEARCH_WALLET_PRESEARCH_WALLET_DELEGATE_IMPL_H_
