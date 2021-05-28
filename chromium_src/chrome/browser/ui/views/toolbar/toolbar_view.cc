/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "presearch/browser/ui/views/location_bar/presearch_location_bar_view.h"
#include "chrome/browser/ui/views/profiles/avatar_toolbar_button.h"
#include "presearch/browser/ui/views/profiles/presearch_avatar_toolbar_button.h"

#define LocationBarView PresearchLocationBarView
#include "../../../../../../../chrome/browser/ui/views/toolbar/toolbar_view.cc" // NOLINT
#undef LocationBarView
#endif
