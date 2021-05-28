/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_SKU_FACTORY_H_
#define PRESEARCHLEDGER_SKU_FACTORY_H_

#include <memory>

#include "bat/ledger/internal/sku/sku.h"
#include "bat/ledger/internal/sku/sku_presearch.h"
#include "bat/ledger/internal/sku/sku_merchant.h"

namespace ledger {
class LedgerImpl;

namespace sku {

enum SKUType {
  kPresearch = 0,
  kMerchant = 1
};

class SKUFactory {
 public:
  static std::unique_ptr<SKU> Create(
      LedgerImpl* ledger,
      const SKUType type);
};

}  // namespace sku
}  // namespace ledger

#endif  // PRESEARCHLEDGER_SKU_FACTORY_H_
