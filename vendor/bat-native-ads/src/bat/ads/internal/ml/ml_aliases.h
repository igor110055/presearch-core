/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_ML_ALIASES_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_ML_ALIASES_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ads {
namespace ml {

class Transformation;

using PredictionMap = std::map<std::string, double>;
using TransformationPtr = std::unique_ptr<Transformation>;
using TransformationVector = std::vector<TransformationPtr>;

}  // namespace ml
}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_ML_ALIASES_H_
