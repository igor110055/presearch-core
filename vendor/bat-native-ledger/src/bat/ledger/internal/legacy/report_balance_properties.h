/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_PROPERTIES_REPORT_BALANCE_PROPERTIES_H_
#define PRESEARCHLEDGER_PROPERTIES_REPORT_BALANCE_PROPERTIES_H_

#include <string>

namespace ledger {

struct ReportBalanceProperties {
  ReportBalanceProperties();
  ReportBalanceProperties(
      const ReportBalanceProperties& properties);
  ~ReportBalanceProperties();

  bool operator==(
      const ReportBalanceProperties& rhs) const;

  bool operator!=(
      const ReportBalanceProperties& rhs) const;

  double grants;
  double ad_earnings;
  double auto_contributions;
  double recurring_donations;
  double one_time_donations;
};

}  // namespace ledger

#endif  // PRESEARCHLEDGER_PROPERTIES_REPORT_BALANCE_PROPERTIES_H_
