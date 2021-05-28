/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/version_info.h"

#define GetVersionNumber GetPresearchVersionNumberForDisplay
#include "../../../../../../../chrome/browser/ui/webui/version/version_ui.cc"
#undef GetVersionNumber
