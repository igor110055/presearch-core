/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_PROMOTION_PROMOTION_UTIL_H_
#define PRESEARCHLEDGER_PROMOTION_PROMOTION_UTIL_H_

#include <memory>
#include <string>
#include <vector>

#include "bat/ledger/mojom_structs.h"

namespace ledger {
namespace promotion {

std::string ParseOSToString(type::OperatingSystem os);

std::string ParseClientInfoToString(type::ClientInfoPtr info);

type::PromotionType ConvertStringToPromotionType(const std::string& type);

type::ReportType ConvertPromotionTypeToReportType(
    const type::PromotionType type);

}  // namespace promotion
}  // namespace ledger

#endif  // PRESEARCHLEDGER_PROMOTION_PROMOTION_UTIL_H_
