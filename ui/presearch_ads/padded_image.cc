// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "presearch/ui/presearch_ads/padded_image.h"

#include <memory>
#include <utility>

#include "presearch/ui/presearch_ads/public/cpp/constants.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/painter.h"

namespace presearch_ads {

PaddedImage::PaddedImage() : views::ImageView() {
  SetBackground(views::CreateSolidBackground(kControlButtonBackgroundColor));
  SetBorder(views::CreateEmptyBorder(gfx::Insets(kControlButtonBorderSize)));
}

}  // namespace presearch_ads
