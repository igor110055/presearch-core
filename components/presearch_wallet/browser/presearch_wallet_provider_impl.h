/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_PROVIDER_IMPL_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_PROVIDER_IMPL_H_

#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "presearch/components/presearch_wallet/common/presearch_wallet.mojom.h"

class PresearchWalletService;

namespace presearch_wallet {

class PresearchWalletProviderImpl final
    : public presearch_wallet::mojom::PresearchWalletProvider {
 public:
  PresearchWalletProviderImpl(const PresearchWalletProviderImpl&) = delete;
  PresearchWalletProviderImpl& operator=(const PresearchWalletProviderImpl&) = delete;
  explicit PresearchWalletProviderImpl(
      base::WeakPtr<PresearchWalletService> wallet_service);
  ~PresearchWalletProviderImpl() override;

  void Request(const std::string& json_payload,
               RequestCallback callback) override;
  void OnResponse(RequestCallback callback,
                  const int http_code,
                  const std::string& response,
                  const std::map<std::string, std::string>& headers);

 private:
  base::WeakPtr<PresearchWalletService> wallet_service_;

  base::WeakPtrFactory<PresearchWalletProviderImpl> weak_factory_;
};

}  // namespace presearch_wallet

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_WALLET_BROWSER_PRESEARCH_WALLET_PROVIDER_IMPL_H_
