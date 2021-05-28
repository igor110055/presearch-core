/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSIONS_BROWSER_API_PROVIDER_H_
#define PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSIONS_BROWSER_API_PROVIDER_H_

#include "base/macros.h"
#include "extensions/browser/extensions_browser_api_provider.h"

namespace extensions {

class PresearchExtensionsBrowserAPIProvider : public ExtensionsBrowserAPIProvider {
 public:
  PresearchExtensionsBrowserAPIProvider();
  ~PresearchExtensionsBrowserAPIProvider() override;

  void RegisterExtensionFunctions(ExtensionFunctionRegistry* registry) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchExtensionsBrowserAPIProvider);
};

}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSIONS_BROWSER_API_PROVIDER_H_
