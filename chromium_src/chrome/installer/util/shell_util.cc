/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define PRESEARCH_IPFS L"ipfs"
#define PRESEARCH_IPNS L"ipns"

#define PRESEARCH_GET_TARGET_FOR_DEFAULT_APP_SETTINGS                         \
  if (base::EqualsCaseInsensitiveASCII(protocol, PRESEARCH_IPFS))             \
    return base::StringPrintf(kSystemSettingsDefaultAppsFormat, L"IPFS"); \
  if (base::EqualsCaseInsensitiveASCII(protocol, PRESEARCH_IPNS))             \
    return base::StringPrintf(kSystemSettingsDefaultAppsFormat, L"IPNS");

#include "../../../../../chrome/installer/util/shell_util.cc"
#undef PRESEARCH_GET_TARGET_FOR_DEFAULT_APP_SETTINGS
#undef PRESEARCH_IPFS
#undef PRESEARCH_IPNS
