// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_BROWSER_UI_WEBUI_NEW_TAB_PAGE_PRESEARCH_NEW_TAB_UI_H_
#define PRESEARCH_BROWSER_UI_WEBUI_NEW_TAB_PAGE_PRESEARCH_NEW_TAB_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class PresearchNewTabUI : public content::WebUIController {
 public:
  PresearchNewTabUI(content::WebUI* web_ui, const std::string& name);
  ~PresearchNewTabUI() override;
  PresearchNewTabUI(const PresearchNewTabUI&) = delete;
  PresearchNewTabUI& operator=(const PresearchNewTabUI&) = delete;
};

#endif  // PRESEARCH_BROWSER_UI_WEBUI_NEW_TAB_PAGE_PRESEARCH_NEW_TAB_UI_H_
