/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_PRESEARCH_APPEARANCE_HANDLER_H_
#define PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_PRESEARCH_APPEARANCE_HANDLER_H_

#include <string>

#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

class Profile;

class PresearchAppearanceHandler : public settings::SettingsPageUIHandler {
 public:
  PresearchAppearanceHandler();
  ~PresearchAppearanceHandler() override;

  PresearchAppearanceHandler(const PresearchAppearanceHandler&) = delete;
  PresearchAppearanceHandler& operator=(const PresearchAppearanceHandler&) = delete;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void OnPresearchDarkModeChanged();
  void OnBackgroundPreferenceChanged(const std::string& pref_name);
  void OnPreferenceChanged(const std::string& pref_name);
  void SetPresearchThemeType(const base::ListValue* args);
  void GetPresearchThemeType(const base::ListValue* args);
  void GetNewTabShowsOptionsList(const base::ListValue* args);
  void ShouldShowNewTabDashboardSettings(const base::ListValue* args);

  Profile* profile_ = nullptr;
  PrefChangeRegistrar local_state_change_registrar_;
  PrefChangeRegistrar profile_state_change_registrar_;
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_PRESEARCH_APPEARANCE_HANDLER_H_
