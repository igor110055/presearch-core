/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/profiles/profile_util.h"
#include "presearch/common/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/common/webui_url_constants.h"
#include "url/gurl.h"

namespace {

bool HandleURLInParent(NavigateParams* params, Profile* profile) {
  if (profile->IsTor() &&
      !params->browser->profile()->IsOffTheRecord()) {
    return true;
  }

  return false;
}

// GetOrCreateBrowser is not accessible here
Browser* PresearchGetOrCreateBrowser(Profile* profile, bool user_gesture) {
  Browser* browser = chrome::FindTabbedBrowser(profile, false);
  return browser
             ? browser
             : Browser::Create(Browser::CreateParams(profile, user_gesture));
}

void UpdatePresearchScheme(NavigateParams* params) {
  if (params->url.SchemeIs(content::kPresearchUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    params->url = params->url.ReplaceComponents(replacements);
  }
}

void MaybeHandleInParent(NavigateParams* params, bool allow_in_incognito) {
  auto* profile = params->initiating_profile;
  if (presearch::IsSessionProfile(profile)) {
    if (!allow_in_incognito) {
      params->initiating_profile =
          profile->IsOffTheRecord()
              ? presearch::GetParentProfile(profile)->GetPrimaryOTRProfile()
              : presearch::GetParentProfile(profile);
    } else if (HandleURLInParent(params, profile)) {
      params->browser = PresearchGetOrCreateBrowser(
          presearch::GetParentProfile(profile), params->user_gesture);
    }
  }
}

bool IsHostAllowedInIncognitoPresearchImpl(const base::StringPiece& host) {
  if (host == kWalletHost ||
      host == kRewardsPageHost ||
      host == chrome::kChromeUISyncInternalsHost) {
    return false;
  }

  return true;
}

}  // namespace

#define PRESEARCH_ADJUST_NAVIGATE_PARAMS_FOR_URL           \
  UpdatePresearchScheme(params);                           \
  MaybeHandleInParent(params, IsURLAllowedInIncognito( \
                                  params->url, params->initiating_profile));

#include "../../../../../chrome/browser/ui/browser_navigator.cc"
#undef PRESEARCH_ADJUST_NAVIGATE_PARAMS_FOR_URL
