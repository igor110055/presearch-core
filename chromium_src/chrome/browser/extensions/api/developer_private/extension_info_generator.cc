/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "extensions/common/manifest_handlers/incognito_info.h"

#define PRESEARCH_CREATE_EXTENSION_INFO_HELPER \
  info->is_split_mode = IncognitoInfo::IsSplitMode(&extension);
#include "../../../../../../../chrome/browser/extensions/api/developer_private/extension_info_generator.cc"
#undef PRESEARCH_CREATE_EXTENSION_INFO_HELPER
