/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_RECOVERY_RECOVERY_H_
#define PRESEARCHLEDGER_RECOVERY_RECOVERY_H_

#include <memory>

#include "bat/ledger/internal/recovery/recovery_empty_balance.h"

namespace ledger {
class LedgerImpl;

namespace recovery {

class Recovery {
 public:
  explicit Recovery(LedgerImpl* ledger);
  ~Recovery();

  void Check();

 private:
  LedgerImpl* ledger_;  // NOT OWNED
  std::unique_ptr<EmptyBalance> empty_balance_;
};

}  // namespace recovery
}  // namespace ledger

#endif  // PRESEARCHLEDGER_RECOVERY_RECOVERY_H_
