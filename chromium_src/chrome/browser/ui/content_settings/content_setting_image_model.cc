/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/content_settings/content_setting_image_model.h"

#include "presearch/browser/ui/content_settings/presearch_content_setting_image_models.h"

#define GenerateContentSettingImageModels \
  GenerateContentSettingImageModels_ChromiumImpl
#include "../../../../../../chrome/browser/ui/content_settings/content_setting_image_model.cc"
#undef GenerateContentSettingImageModels

std::vector<std::unique_ptr<ContentSettingImageModel>>
ContentSettingImageModel::GenerateContentSettingImageModels() {
  std::vector<std::unique_ptr<ContentSettingImageModel>> result =
      GenerateContentSettingImageModels_ChromiumImpl();
  PresearchGenerateContentSettingImageModels(&result);
  return result;
}
