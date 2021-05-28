/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/presearch_together_api.h"

#include <string>
#include <memory>

#include "base/environment.h"
#include "presearch/components/presearch_together/browser/regions.h"
#include "presearch/components/ntp_widget_utils/browser/ntp_widget_utils_region.h"
#include "chrome/browser/profiles/profile.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction
PresearchTogetherIsSupportedFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());

  if (profile->IsTor()) {
    return RespondNow(Error("Not available in Tor profile"));
  }

  bool is_supported = ntp_widget_utils::IsRegionSupported(
      profile->GetPrefs(), presearch_together::unsupported_regions, false);
  return RespondNow(OneArgument(base::Value(is_supported)));
}

}  // namespace api
}  // namespace extensions
