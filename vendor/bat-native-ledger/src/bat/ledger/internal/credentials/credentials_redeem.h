/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_CREDENTIALS_CREDENTIALS_REDEEM_H_
#define PRESEARCHLEDGER_CREDENTIALS_CREDENTIALS_REDEEM_H_

#include <vector>
#include <string>

#include "bat/ledger/ledger.h"

namespace ledger {
namespace credential {

struct CredentialsRedeem {
  CredentialsRedeem();
  CredentialsRedeem(const CredentialsRedeem& info);
  ~CredentialsRedeem();

  std::string publisher_key;
  type::RewardsType type;
  type::ContributionProcessor processor;
  std::vector<type::UnblindedToken> token_list;
  std::string order_id;
  std::string contribution_id;
};

}  // namespace credential
}  // namespace ledger

#endif  // PRESEARCHLEDGER_CREDENTIALS_CREDENTIALS_REDEEM_H_
