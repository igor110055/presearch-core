/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSION_SERVICE_H_
#define PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSION_SERVICE_H_

#include "chrome/browser/extensions/extension_service.h"

namespace extensions {

class PresearchExtensionService : public ExtensionService {
 public:
  // Constructor stores pointers to |profile| and |extension_prefs| but
  // ownership remains at caller.
  PresearchExtensionService(Profile* profile,
                        const base::CommandLine* command_line,
                        const base::FilePath& install_directory,
                        ExtensionPrefs* extension_prefs,
                        Blocklist* blocklist,
                        bool autoupdate_enabled,
                        bool extensions_enabled,
                        base::OneShotEvent* ready);
  ~PresearchExtensionService() override;

  void AddComponentExtension(const Extension* extension) override;

  DISALLOW_COPY_AND_ASSIGN(PresearchExtensionService);
};

}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_PRESEARCH_EXTENSION_SERVICE_H_
