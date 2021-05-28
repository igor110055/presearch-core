/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_CONSTANTS_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_CONSTANTS_H_

enum class PresearchWalletWeb3ProviderTypes {
  ASK,
  NONE,
  CRYPTO_WALLETS,
  METAMASK,
  PRESEARCH_WALLET
};

extern const char ethereum_remote_client_extension_id[];
extern const char ethereum_remote_client_extension_name[];
extern const char ethereum_remote_client_extension_public_key[];
extern const char ethereum_remote_client_base_url[];
extern const char ethereum_remote_client_phishing_url[];
extern const char ethereum_remote_client_ens_redirect_url[];
extern const char ethereum_remote_client_host[];
extern const char metamask_extension_id[];

namespace presearch_wallet {

enum class Network {
  kMainnet,
  kRinkeby,
  kRopsten,
  kGoerli,
  kKovan,
  kLocalhost,
  kCustom,
};

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_CONSTANTS_H_
