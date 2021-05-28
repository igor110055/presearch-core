// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch.components/presearch_today/browser/urls.h"

#include <string>

#include "base/command_line.h"
#include "presearch.components/presearch_today/common/switches.h"
#include "presearch.components/l10n/browser/locale_helper.h"
#include "presearch.components/l10n/common/locale_util.h"

namespace presearch_today {

std::string GetHostname() {
  std::string from_switch =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kPresearchTodayHost);
  if (from_switch.empty()) {
    return "presearch-today-cdn.presearch.org";
  } else {
    return from_switch;
  }
}

std::string GetRegionUrlPart() {
  const std::string locale =
      presearch_l10n::LocaleHelper::GetInstance()->GetLocale();
  const std::string language_code = presearch_l10n::GetLanguageCode(locale);
  // TODO(petemill): Have a remotely-updatable list of supported language
  // variations.
  if (language_code == "ja") {
    return "ja";
  }
  return "";
}

}  // namespace presearch_today
