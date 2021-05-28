/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_extensions_browser_api_provider.h"

#include "presearch/common/extensions/api/generated_api_registration.h"
#include "extensions/browser/extension_function_registry.h"

namespace extensions {

PresearchExtensionsBrowserAPIProvider::PresearchExtensionsBrowserAPIProvider() =
    default;
PresearchExtensionsBrowserAPIProvider::~PresearchExtensionsBrowserAPIProvider() =
    default;

void PresearchExtensionsBrowserAPIProvider::RegisterExtensionFunctions(
    ExtensionFunctionRegistry* registry) {
  // Generated APIs from Presearch.
  api::PresearchGeneratedFunctionRegistry::RegisterAll(registry);
}

}  // namespace extensions
