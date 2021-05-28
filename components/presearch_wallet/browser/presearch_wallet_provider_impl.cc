/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_wallet/browser/presearch_wallet_provider_impl.h"

#include <utility>

#include "presearch/components/presearch_wallet/browser/presearch_wallet_service.h"

namespace presearch_wallet {

PresearchWalletProviderImpl::PresearchWalletProviderImpl(
    base::WeakPtr<PresearchWalletService> wallet_service)
    : wallet_service_(wallet_service), weak_factory_(this) {}

PresearchWalletProviderImpl::~PresearchWalletProviderImpl() {}

void PresearchWalletProviderImpl::Request(const std::string& json_payload,
                                      RequestCallback callback) {
  if (!wallet_service_)
    return;

  auto* controller = wallet_service_->controller();
  controller->Request(
      json_payload,
      base::BindOnce(&PresearchWalletProviderImpl::OnResponse,
                     weak_factory_.GetWeakPtr(), std::move(callback)),
      true);
}

void PresearchWalletProviderImpl::OnResponse(
    RequestCallback callback,
    const int http_code,
    const std::string& response,
    const std::map<std::string, std::string>& headers) {
  // Do we need to pass headers map to a renderer? We would need to convert
  // it to base::flat_map in that case
  std::move(callback).Run(http_code, response);
}

}  // namespace presearch_wallet
