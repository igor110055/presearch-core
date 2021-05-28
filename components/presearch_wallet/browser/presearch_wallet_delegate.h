/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
+ * This Source Code Form is subject to the terms of the Mozilla Public
+ * License, v. 2.0. If a copy of the MPL was not distributed with this file,
+ * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_DELEGATE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_DELEGATE_H_

namespace content {
class BrowserContext;
}  // namespace content

class PresearchWalletDelegate {
 public:
  virtual ~PresearchWalletDelegate() = default;
  virtual void MaybeLoadCryptoWalletsExtension(
      content::BrowserContext* context) = 0;
};

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_DELEGATE_H_
