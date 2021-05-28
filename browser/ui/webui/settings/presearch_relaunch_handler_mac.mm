/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/settings/presearch_relaunch_handler_mac.h"

#include "base/bind.h"
#import "presearch/browser/mac/sparkle_glue.h"

void PresearchRelaunchHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "relaunchOnMac",
      base::BindRepeating(&PresearchRelaunchHandler::Relaunch,
                          base::Unretained(this)));
}

void PresearchRelaunchHandler::Relaunch(const base::ListValue* args) {
  [[SparkleGlue sharedSparkleGlue] relaunch];
}
