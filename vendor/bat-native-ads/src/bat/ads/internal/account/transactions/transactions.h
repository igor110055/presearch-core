/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_H_

#include <cstdint>

#include "base/time/time.h"
#include "bat/ads/transaction_info.h"

namespace ads {

struct ConfirmationInfo;

namespace transactions {

TransactionList GetCleared(const int64_t from_timestamp,
                           const int64_t to_timestamp);

TransactionList GetUncleared();

uint64_t GetCountForMonth(const base::Time& time);

void Add(const double estimated_redemption_value,
         const ConfirmationInfo& confirmation);

}  // namespace transactions
}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTIONS_H_
