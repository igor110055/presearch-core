/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_SETTINGS_UI_H_
#define PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_SETTINGS_UI_H_

#include <memory>
#include <string>

#include "chrome/browser/ui/webui/settings/settings_ui.h"

namespace content {
class WebUIDataSource;
}

class Profile;

class PresearchSettingsUI : public settings::SettingsUI {
 public:
  PresearchSettingsUI(content::WebUI* web_ui, const std::string& host);
  ~PresearchSettingsUI() override;

  static void AddResources(content::WebUIDataSource* html_source,
                           Profile* profile);

  DISALLOW_COPY_AND_ASSIGN(PresearchSettingsUI);
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_SETTINGS_UI_H_
