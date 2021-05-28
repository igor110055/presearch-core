/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CONVERSIONS_SORTS_CONVERSIONS_DESCENDING_SORT_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CONVERSIONS_SORTS_CONVERSIONS_DESCENDING_SORT_H_

#include "bat/ads/internal/conversions/sorts/conversions_sort.h"

namespace ads {

class ConversionsDescendingSort : public ConversionsSort {
 public:
  ConversionsDescendingSort();
  ~ConversionsDescendingSort() override;

  ConversionList Apply(const ConversionList& list) const override;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CONVERSIONS_SORTS_CONVERSIONS_DESCENDING_SORT_H_
