/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_SERVER_URL_REQUEST_BUILDER_H_
#define PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_SERVER_URL_REQUEST_BUILDER_H_

#include "bat/ads/mojom.h"

namespace ads {

class UrlRequestBuilder {
 public:
  virtual ~UrlRequestBuilder() = default;

  virtual UrlRequestPtr Build() = 0;
};

}  // namespace ads

#endif  // PRESEARCH_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_SERVER_URL_REQUEST_BUILDER_H_
