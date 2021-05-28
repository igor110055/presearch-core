/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_STRING_UTIL_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_STRING_UTIL_H_

#include <string>
#include <vector>

namespace ads {

std::string StripNonAlphaCharacters(const std::string& value);

std::string StripNonAlphaNumericCharacters(const std::string& value);

bool IsLatinAlphaNumeric(const std::string& value);

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_STRING_UTIL_H_
