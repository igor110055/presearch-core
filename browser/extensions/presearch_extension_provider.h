/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSION_PROVIDER_H_
#define PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSION_PROVIDER_H_

#include <string>

#include "extensions/browser/management_policy.h"

namespace extensions {

class PresearchExtensionProvider : public ManagementPolicy::Provider {
 public:
  PresearchExtensionProvider();
  ~PresearchExtensionProvider() override;
  std::string GetDebugPolicyProviderName() const override;
  bool UserMayLoad(const Extension* extension,
                   base::string16* error) const override;
  bool MustRemainInstalled(const Extension* extension,
                           base::string16* error) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PresearchExtensionProvider);
};

}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSION_PROVIDER_H_
