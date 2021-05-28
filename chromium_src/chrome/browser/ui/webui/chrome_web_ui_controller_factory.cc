/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/presearch_web_ui_controller_factory.h"

#define PRESEARCH_CHROME_WEBUI_CONTROLLER_FACTORY \
  return PresearchWebUIControllerFactory::GetInstance();

#include "../../../../../../chrome/browser/ui/webui/chrome_web_ui_controller_factory.cc"
#undef PRESEARCH_CHROME_WEBUI_CONTROLLER_FACTORY
