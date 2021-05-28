/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_CREATE_CONFIRMATION_URL_REQUEST_BUILDER_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_CREATE_CONFIRMATION_URL_REQUEST_BUILDER_H_

#include <string>
#include <vector>

#include "bat/ads/internal/account/confirmations/confirmation_info.h"
#include "bat/ads/internal/server/url_request_builder.h"

namespace ads {

namespace privacy {
struct UnblindedTokenInfo;
}  // namespace privacy

class CreateConfirmationUrlRequestBuilder : UrlRequestBuilder {
 public:
  explicit CreateConfirmationUrlRequestBuilder(
      const ConfirmationInfo& confirmation);

  ~CreateConfirmationUrlRequestBuilder() override;

  UrlRequestPtr Build() override;

 private:
  ConfirmationInfo confirmation_;

  std::string BuildUrl() const;

  std::vector<std::string> BuildHeaders() const;

  std::string BuildBody() const;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_CREATE_CONFIRMATION_URL_REQUEST_BUILDER_H_
