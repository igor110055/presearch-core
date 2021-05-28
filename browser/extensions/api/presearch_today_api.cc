// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/browser/extensions/api/presearch_today_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "presearch/common/extensions/api/presearch_theme.h"
#include "presearch/components/presearch_today/browser/urls.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction PresearchTodayGetHostnameFunction::Run() {
  return RespondNow(OneArgument(base::Value(presearch_today::GetHostname())));
}

ExtensionFunction::ResponseAction PresearchTodayGetRegionUrlPartFunction::Run() {
  return RespondNow(OneArgument(base::Value(presearch_today::GetRegionUrlPart())));
}

}  // namespace api
}  // namespace extensions
