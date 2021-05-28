// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_TODAY_API_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_TODAY_API_H_

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class PresearchTodayGetHostnameFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchToday.getHostname", UNKNOWN)

 protected:
  ~PresearchTodayGetHostnameFunction() override {}

  ResponseAction Run() override;
};

class PresearchTodayGetRegionUrlPartFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchToday.getRegionUrlPart", UNKNOWN)

 protected:
  ~PresearchTodayGetRegionUrlPartFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_TODAY_API_H_
