/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_DEFAULT_PRESEARCH_SHIELDS_HANDLER_H_
#define PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_DEFAULT_PRESEARCH_SHIELDS_HANDLER_H_

#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"

class Profile;

class DefaultPresearchShieldsHandler : public settings::SettingsPageUIHandler {
 public:
  DefaultPresearchShieldsHandler() = default;
  ~DefaultPresearchShieldsHandler() override = default;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void SetAdControlType(const base::ListValue* args);
  void IsAdControlEnabled(const base::ListValue* args);
  void SetCosmeticFilteringControlType(const base::ListValue* args);
  void IsFirstPartyCosmeticFilteringEnabled(const base::ListValue* args);
  void SetCookieControlType(const base::ListValue* args);
  void GetCookieControlType(const base::ListValue* args);
  void SetFingerprintingControlType(const base::ListValue* args);
  void GetFingerprintingControlType(const base::ListValue* args);
  void SetHTTPSEverywhereEnabled(const base::ListValue* args);
  void GetHTTPSEverywhereEnabled(const base::ListValue* args);
  void SetNoScriptControlType(const base::ListValue* args);
  void GetNoScriptControlType(const base::ListValue* args);

  Profile* profile_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(DefaultPresearchShieldsHandler);
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_SETTINGS_DEFAULT_PRESEARCH_SHIELDS_HANDLER_H_
