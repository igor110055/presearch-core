/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define AddProfilesExtraParts AddProfilesExtraParts_ChromiumImpl
#include "../../../../../chrome/browser/profiles/chrome_browser_main_extra_parts_profiles.cc"
#undef AddProfilesExtraParts

#include "presearch/browser/browser_context_keyed_service_factories.h"

namespace {

class PresearchBrowserMainExtraPartsProfiles
    : public ChromeBrowserMainExtraPartsProfiles {
 public:
  PresearchBrowserMainExtraPartsProfiles()
      : ChromeBrowserMainExtraPartsProfiles() {}

  void PreProfileInit() override {
    ChromeBrowserMainExtraPartsProfiles::PreProfileInit();
    presearch::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchBrowserMainExtraPartsProfiles);
};

}  // namespace

namespace chrome {

void AddProfilesExtraParts(ChromeBrowserMainParts* main_parts) {
  main_parts->AddParts(std::make_unique<PresearchBrowserMainExtraPartsProfiles>());
}

}  // namespace chrome
