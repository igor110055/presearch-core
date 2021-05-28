/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/toolbar/presearch_toolbar_actions_model.h"

#include "chrome/browser/ui/toolbar/toolbar_actions_model.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"

bool PresearchToolbarActionsModel::ShouldAddExtension(
    const extensions::Extension* extension) {
  // Don't show the Presearch and Rewards 'extensions' in the ToolbarActions
  // extensions area. They will instead be shown in the PresearchActions area.
  if (extension->id() == presearch_extension_id ||
      extension->id() == presearch_rewards_extension_id) {
    return false;
  }
  return ToolbarActionsModel::ShouldAddExtension(extension);
}
