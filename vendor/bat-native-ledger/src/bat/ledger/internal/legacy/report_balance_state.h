/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_LEGACY_REPORT_BALANCE_STATE_H_
#define PRESEARCHLEDGER_LEGACY_REPORT_BALANCE_STATE_H_

#include <string>

#include "bat/ledger/internal/legacy/state_reader.h"
#include "bat/ledger/internal/legacy/state_writer.h"
#include "bat/ledger/internal/legacy/report_balance_properties.h"
#include "rapidjson/writer.h"

namespace ledger {

using JsonWriter = rapidjson::Writer<rapidjson::StringBuffer>;

class ReportBalanceState
    : public state::Reader<ReportBalanceProperties>,
      public state::Writer<JsonWriter*, ReportBalanceProperties> {
 public:
  ReportBalanceState();
  ~ReportBalanceState();

  bool FromJson(
      const std::string& json,
      ReportBalanceProperties* properties) const override;

  bool FromDict(
      const base::DictionaryValue* dictionary,
      ReportBalanceProperties* properties) const override;

  bool ToJson(
      JsonWriter* writer,
      const ReportBalanceProperties& state) const override;

  std::string ToJson(
      const ReportBalanceProperties& state) const override;
};

}  // namespace ledger

#endif  // PRESEARCHLEDGER_LEGACY_REPORT_BALANCE_STATE_H_
