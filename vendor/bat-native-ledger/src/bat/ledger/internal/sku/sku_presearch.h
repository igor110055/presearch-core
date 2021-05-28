/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_SKU_PRESEARCH_H_
#define PRESEARCHLEDGER_SKU_PRESEARCH_H_

#include <memory>
#include <string>
#include <vector>

#include "bat/ledger/internal/sku/sku.h"
#include "bat/ledger/internal/sku/sku_common.h"
#include "bat/ledger/ledger.h"

namespace ledger {
class LedgerImpl;

namespace sku {

class SKUPresearch : public SKU  {
 public:
  explicit SKUPresearch(LedgerImpl* ledger);
  ~SKUPresearch() override;

  void Process(
      const std::vector<type::SKUOrderItem>& items,
      const std::string& wallet_type,
      ledger::SKUOrderCallback callback,
      const std::string& contribution_id = "") override;

  void Retry(
      const std::string& order_id,
      const std::string& wallet_type,
      ledger::SKUOrderCallback callback) override;

 private:
  void OrderCreated(
      const type::Result result,
      const std::string& order_id,
      const std::string& wallet_type,
      const std::string& contribution_id,
      ledger::SKUOrderCallback callback);

  void ContributionIdSaved(
      const type::Result result,
      const std::string& order_id,
      const std::string& wallet_type,
      ledger::SKUOrderCallback callback);

  void CreateTransaction(
      type::SKUOrderPtr order,
      const std::string& wallet_type,
      ledger::SKUOrderCallback callback);

  void OnOrder(
      type::SKUOrderPtr order,
      const std::string& wallet_type,
      ledger::SKUOrderCallback callback);

  LedgerImpl* ledger_;  // NOT OWNED
  std::unique_ptr<SKUCommon> common_;
};

}  // namespace sku
}  // namespace ledger

#endif  // PRESEARCHLEDGER_SKU_PRESEARCH_H_
