/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_extension_management.h"

#define PRESEARCH_EXTENSION_MANAGEMENT_FACTORY_BUILD_SERVICE_INSTANCE_FOR \
  return new PresearchExtensionManagement(Profile::FromBrowserContext(context));

#include "../../../../../chrome/browser/extensions/extension_management.cc"
#undef PRESEARCH_EXTENSION_MANAGEMENT_FACTORY_BUILD_SERVICE_INSTANCE_FOR
