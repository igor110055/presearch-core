/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_WELCOME_UI_H_
#define PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_WELCOME_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class PresearchWelcomeUI : public content::WebUIController {
 public:
  PresearchWelcomeUI(content::WebUI* web_ui, const std::string& host);
  ~PresearchWelcomeUI() override;
  PresearchWelcomeUI(const PresearchWelcomeUI&) = delete;
  PresearchWelcomeUI& operator=(const PresearchWelcomeUI&) = delete;
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_PRESEARCH_WELCOME_UI_H_
