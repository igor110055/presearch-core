/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_SECURITY_CONVERSIONS_CONVERSIONS_UTIL_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_SECURITY_CONVERSIONS_CONVERSIONS_UTIL_H_

#include <string>

#include "base/optional.h"

namespace ads {

struct VerifiableConversionInfo;

namespace security {

struct VerifiableConversionEnvelopeInfo;

base::Optional<VerifiableConversionEnvelopeInfo> EnvelopeSeal(
    const VerifiableConversionInfo& verifiable_conversion);

}  // namespace security
}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_SECURITY_CONVERSIONS_CONVERSIONS_UTIL_H_
