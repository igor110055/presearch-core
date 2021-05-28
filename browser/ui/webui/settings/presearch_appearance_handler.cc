/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/settings/presearch_appearance_handler.h"

#include "base/bind.h"
#include "base/metrics/histogram_macros.h"
#include "base/strings/string_number_conversions.h"
#include "presearch/browser/new_tab/new_tab_shows_options.h"
#include "presearch/browser/profiles/profile_util.h"
#include "presearch/browser/themes/presearch_dark_mode_utils.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/ntp_background_images/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search/instant_service.h"
#include "chrome/browser/search/instant_service_factory.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"


PresearchAppearanceHandler::PresearchAppearanceHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kPresearchDarkMode,
      base::Bind(&PresearchAppearanceHandler::OnPresearchDarkModeChanged,
                 base::Unretained(this)));
}

PresearchAppearanceHandler::~PresearchAppearanceHandler() = default;

// TODO(simonhong): Use separate handler for NTP settings.
void PresearchAppearanceHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  profile_state_change_registrar_.Init(profile_->GetPrefs());
  profile_state_change_registrar_.Add(
      kNewTabPageShowsOptions,
      base::BindRepeating(&PresearchAppearanceHandler::OnPreferenceChanged,
      base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePageIsNewTabPage,
      base::BindRepeating(&PresearchAppearanceHandler::OnPreferenceChanged,
      base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePage,
      base::BindRepeating(&PresearchAppearanceHandler::OnPreferenceChanged,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setPresearchThemeType",
      base::BindRepeating(&PresearchAppearanceHandler::SetPresearchThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getPresearchThemeType",
      base::BindRepeating(&PresearchAppearanceHandler::GetPresearchThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabShowsOptionsList",
      base::BindRepeating(&PresearchAppearanceHandler::GetNewTabShowsOptionsList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "shouldShowNewTabDashboardSettings",
      base::BindRepeating(
          &PresearchAppearanceHandler::ShouldShowNewTabDashboardSettings,
          base::Unretained(this)));
}

void PresearchAppearanceHandler::SetPresearchThemeType(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();

  int int_type;
  args->GetInteger(0, &int_type);
  dark_mode::SetPresearchDarkModeType(
      static_cast<dark_mode::PresearchDarkModeType>(int_type));
}

void PresearchAppearanceHandler::GetPresearchThemeType(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);

  AllowJavascript();
  // GetPresearchThemeType() should be used because settings option displays all
  // available options including default.
  ResolveJavascriptCallback(
      args->GetList()[0],
      base::Value(static_cast<int>(dark_mode::GetPresearchDarkModeType())));
}

void PresearchAppearanceHandler::OnPresearchDarkModeChanged() {
  // GetPresearchThemeType() should be used because settings option displays all
  // available options including default.
  if (IsJavascriptAllowed()) {
    FireWebUIListener(
        "presearch-theme-type-changed",
        base::Value(static_cast<int>(dark_mode::GetPresearchDarkModeType())));
  }
}

void PresearchAppearanceHandler::OnBackgroundPreferenceChanged(
    const std::string& pref_name) {
  presearch::RecordSponsoredImagesEnabledP3A(profile_);
}

void PresearchAppearanceHandler::OnPreferenceChanged(const std::string& pref_name) {
  if (IsJavascriptAllowed()) {
    if (pref_name == kNewTabPageShowsOptions ||
        pref_name == prefs::kHomePage ||
        pref_name == prefs::kHomePageIsNewTabPage) {
      FireWebUIListener(
          "show-new-tab-dashboard-settings-changed",
          base::Value(presearch::ShouldNewTabShowDashboard(profile_)));
      return;
    }
  }
}

void PresearchAppearanceHandler::GetNewTabShowsOptionsList(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args->GetList()[0],
                            presearch::GetNewTabShowsOptionsList(profile_));
}

void PresearchAppearanceHandler::ShouldShowNewTabDashboardSettings(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0],
      base::Value(presearch::ShouldNewTabShowDashboard(profile_)));
}
