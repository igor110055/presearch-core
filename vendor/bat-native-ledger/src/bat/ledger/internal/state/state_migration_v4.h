/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_BAT_STATE_STATE_MIGRATION_V4_H_
#define PRESEARCHLEDGER_BAT_STATE_STATE_MIGRATION_V4_H_

#include "bat/ledger/ledger.h"

namespace ledger {
class LedgerImpl;

namespace state {

class StateMigrationV4 {
 public:
  explicit StateMigrationV4(LedgerImpl* ledger);
  ~StateMigrationV4();

  void Migrate(ledger::ResultCallback callback);

 private:
  LedgerImpl* ledger_;  // NOT OWNED
};

}  // namespace state
}  // namespace ledger

#endif  // PRESEARCHLEDGER_BAT_STATE_STATE_MIGRATION_V4_H_
