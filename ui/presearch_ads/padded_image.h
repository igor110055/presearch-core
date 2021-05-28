// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRESEARCH_UI_PRESEARCH_ADS_PADDED_IMAGE_H_
#define PRESEARCH_UI_PRESEARCH_ADS_PADDED_IMAGE_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "ui/views/controls/image_view.h"

namespace presearch_ads {

class PaddedImage : public views::ImageView {
 public:
  PaddedImage();
  ~PaddedImage() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(PaddedImage);
};

}  // namespace presearch_ads

#endif  // PRESEARCH_UI_PRESEARCH_ADS_PADDED_IMAGE_H_
