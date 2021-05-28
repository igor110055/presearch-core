/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/profiles/profile_util.h"

#define PRESEARCH_BUILD_SERVICE_INSTANCE_FOR presearch::IsSessionProfile(profile) ||
#include "../../../../../chrome/browser/content_settings/host_content_settings_map_factory.cc"
#undef  PRESEARCH_BUILD_SERVICE_INSTANCE_FOR
