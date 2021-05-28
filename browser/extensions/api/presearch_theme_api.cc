/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/presearch_theme_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/extensions/api/presearch_theme.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction PresearchThemeGetPresearchThemeListFunction::Run() {
  std::string json_string;
  base::JSONWriter::Write(dark_mode::GetPresearchDarkModeTypeList(), &json_string);
  return RespondNow(OneArgument(base::Value(json_string)));
}

ExtensionFunction::ResponseAction PresearchThemeGetPresearchThemeTypeFunction::Run() {
  const std::string theme_type =
      dark_mode::GetStringFromPresearchDarkModeType(
          dark_mode::GetActivePresearchDarkModeType());
  return RespondNow(OneArgument(base::Value(theme_type)));
}

ExtensionFunction::ResponseAction PresearchThemeSetPresearchThemeTypeFunction::Run() {
  std::unique_ptr<presearch_theme::SetPresearchThemeType::Params> params(
      presearch_theme::SetPresearchThemeType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  dark_mode::SetPresearchDarkModeType(params->type);

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
