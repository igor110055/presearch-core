/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/macros.h"
#include "presearch/browser/browser_context_keyed_service_factories.h"
#include "chrome/browser/profiles/chrome_browser_main_extra_parts_profiles.h"

namespace {

class PresearchBrowserMainExtraPartsProfiles
    : public ChromeBrowserMainExtraPartsProfiles {
 public:
  PresearchBrowserMainExtraPartsProfiles() = default;
  ~PresearchBrowserMainExtraPartsProfiles() override = default;

  static void EnsureBrowserContextKeyedServiceFactoriesBuilt() {
    ChromeBrowserMainExtraPartsProfiles::
        EnsureBrowserContextKeyedServiceFactoriesBuilt();
    presearch::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchBrowserMainExtraPartsProfiles);
};

}  // namespace

#define ChromeBrowserMainExtraPartsProfiles PresearchBrowserMainExtraPartsProfiles
#include "../../../../chrome/browser/startup_data.cc"
#undef ChromeBrowserMainExtraPartsProfiles
