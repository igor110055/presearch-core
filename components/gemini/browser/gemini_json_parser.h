/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_GEMINI_BROWSER_GEMINI_JSON_PARSER_H_
#define PRESEARCH_COMPONENTS_GEMINI_BROWSER_GEMINI_JSON_PARSER_H_

#include <string>
#include <vector>

#include "presearch/components/gemini/browser/gemini_service.h"

class GeminiJSONParser {
 public:
  static bool GetTokensFromJSON(const std::string& json,
                                std::string* access_token,
                                std::string* refresh_token);
  static bool GetTickerPriceFromJSON(const std::string& json,
                                     std::string* price);
  static bool GetAccountBalancesFromJSON(const std::string& json,
      GeminiAccountBalances* balances);
  static bool GetDepositInfoFromJSON(const std::string& json,
                                     std::string* address);
  static bool GetOrderQuoteInfoFromJSON(const std::string& json,
                                        std::string *quote_id,
                                        std::string *quantity,
                                        std::string *fee,
                                        std::string *price,
                                        std::string *total_price,
                                        std::string *error);
};

#endif  // PRESEARCH_COMPONENTS_GEMINI_BROWSER_GEMINI_JSON_PARSER_H_
