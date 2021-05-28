/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_USER_DATA_CONFIRMATION_CONVERSION_DTO_USER_DATA_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_USER_DATA_CONFIRMATION_CONVERSION_DTO_USER_DATA_H_

#include "bat/ads/internal/conversions/conversion_queue_item_info.h"

#include "base/values.h"

namespace ads {
namespace dto {
namespace user_data {

base::DictionaryValue GetConversion(
    const ConversionQueueItemInfo& conversion_queue_item);

}  // namespace user_data
}  // namespace dto
}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_USER_DATA_CONFIRMATION_CONVERSION_DTO_USER_DATA_H_
