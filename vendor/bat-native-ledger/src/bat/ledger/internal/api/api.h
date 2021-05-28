/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_API_API_H_
#define PRESEARCHLEDGER_API_API_H_

#include <memory>

#include "bat/ledger/ledger.h"
#include "bat/ledger/internal/api/api_parameters.h"
#include "bat/ledger/mojom_structs.h"

namespace ledger {
class LedgerImpl;

namespace api {

class API {
 public:
  explicit API(LedgerImpl* ledger);
  ~API();

  void Initialize();

  void FetchParameters(ledger::GetRewardsParametersCallback callback);

 private:
  LedgerImpl* ledger_;  // NOT OWNED
  std::unique_ptr<APIParameters> parameters_;
};

}  // namespace api
}  // namespace ledger

#endif  // PRESEARCHLEDGER_API_API_H_
