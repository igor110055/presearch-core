/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_WALLET_PRESEARCH_WALLET_CONTEXT_UTILS_H_
#define PRESEARCH_BROWSER_PRESEARCH_WALLET_PRESEARCH_WALLET_CONTEXT_UTILS_H_

namespace content {
class BrowserContext;
}

namespace presearch_wallet {

bool IsAllowedForContext(content::BrowserContext* context);

}  // namespace presearch_wallet

#endif  // PRESEARCH_BROWSER_PRESEARCH_WALLET_PRESEARCH_WALLET_CONTEXT_UTILS_H_
