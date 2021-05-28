/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_INFO_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_INFO_H_

#include <string>

#include "bat/ads/internal/ml/ml_aliases.h"
#include "bat/ads/internal/ml/model/linear/linear.h"
#include "bat/ads/internal/ml/transformation/transformation.h"

namespace ads {
namespace ml {
namespace pipeline {

struct PipelineInfo {
  PipelineInfo();

  PipelineInfo(const PipelineInfo& pinfo);

  ~PipelineInfo();

  PipelineInfo(const int& version,
               const std::string& timestamp,
               const std::string& locale,
               const TransformationVector& transformations,
               const model::Linear& linear_model);

  int version;
  std::string timestamp;
  std::string locale;
  TransformationVector transformations;
  model::Linear linear_model;
};

}  // namespace pipeline
}  // namespace ml
}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ML_PIPELINE_PIPELINE_INFO_H_
