/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_THEME_API_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_THEME_API_H_

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class PresearchThemeGetPresearchThemeListFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchTheme.getPresearchThemeList", UNKNOWN)

 protected:
  ~PresearchThemeGetPresearchThemeListFunction() override {}

  ResponseAction Run() override;
};

class PresearchThemeGetPresearchThemeTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchTheme.getPresearchThemeType", UNKNOWN)

 protected:
  ~PresearchThemeGetPresearchThemeTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchThemeSetPresearchThemeTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchTheme.setPresearchThemeType", UNKNOWN)

 protected:
  ~PresearchThemeSetPresearchThemeTypeFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_THEME_API_H_
