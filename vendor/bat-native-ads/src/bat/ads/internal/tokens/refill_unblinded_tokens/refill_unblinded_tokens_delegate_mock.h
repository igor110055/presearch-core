/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REFILL_UNBLINDED_TOKENS_REFILL_UNBLINDED_TOKENS_DELEGATE_MOCK_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REFILL_UNBLINDED_TOKENS_REFILL_UNBLINDED_TOKENS_DELEGATE_MOCK_H_

#include "bat/ads/internal/tokens/refill_unblinded_tokens/refill_unblinded_tokens_delegate.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace ads {

class RefillUnblindedTokensDelegateMock : public RefillUnblindedTokensDelegate {
 public:
  RefillUnblindedTokensDelegateMock();

  ~RefillUnblindedTokensDelegateMock() override;

  RefillUnblindedTokensDelegateMock(const RefillUnblindedTokensDelegateMock&) =
      delete;
  RefillUnblindedTokensDelegateMock& operator=(
      const RefillUnblindedTokensDelegateMock&) = delete;

  MOCK_METHOD(void, OnDidRefillUnblindedTokens, ());

  MOCK_METHOD(void, OnFailedToRefillUnblindedTokens, ());

  MOCK_METHOD(void, OnWillRetryRefillingUnblindedTokens, ());
  MOCK_METHOD(void, OnDidRetryRefillingUnblindedTokens, ());
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REFILL_UNBLINDED_TOKENS_REFILL_UNBLINDED_TOKENS_DELEGATE_MOCK_H_
