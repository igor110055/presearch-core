/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/presearch_extension_provider.h"

#include <algorithm>
#include <string>
#include <vector>

#include "base/strings/utf_string_conversions.h"
#include "presearch/browser/presearch_browser_process.h"
#include "presearch/components/presearch_component_updater/browser/extension_whitelist_service.h"
#include "presearch/components/presearch_component_updater/browser/local_data_files_service.h"
#include "presearch/grit/presearch_generated_resources.h"
#include "extensions/common/constants.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

bool IsBlacklisted(const extensions::Extension* extension) {
  // This is a hardcoded list of extensions to block.
  // Don't add new extensions to this list. Add them to
  // the files managed by the extension whitelist service.
  static std::vector<std::string> blacklisted_extensions(
      {// Used for tests, corresponds to
       // presearch/test/data/should-be-blocked-extension.
       "mlklomjnahgiddgfdgjhibinlfibfffc",
     });

  if (std::find(blacklisted_extensions.begin(), blacklisted_extensions.end(),
                extension->id()) != blacklisted_extensions.end())
    return true;

  return g_presearch_browser_process->extension_whitelist_service()->IsBlacklisted(
      extension->id());
}

}  // namespace

namespace extensions {

PresearchExtensionProvider::PresearchExtensionProvider() {}

PresearchExtensionProvider::~PresearchExtensionProvider() {}

std::string PresearchExtensionProvider::GetDebugPolicyProviderName() const {
#if defined(NDEBUG)
  NOTREACHED();
  return std::string();
#else
  return "Presearch Extension Provider";
#endif
}

bool PresearchExtensionProvider::UserMayLoad(const Extension* extension,
                                         base::string16* error) const {
  if (IsBlacklisted(extension)) {
    if (error) {
      *error = l10n_util::GetStringFUTF16(IDS_EXTENSION_CANT_INSTALL_ON_PRESEARCH,
                                          base::UTF8ToUTF16(extension->name()),
                                          base::UTF8ToUTF16(extension->id()));
    }
    DVLOG(1) << "Extension will not install "
             << " ID: " << base::UTF8ToUTF16(extension->id()) << ", "
             << " Name: " << base::UTF8ToUTF16(extension->name());
    return false;
  }
  return true;
}

bool PresearchExtensionProvider::MustRemainInstalled(const Extension* extension,
                                                 base::string16* error) const {
  return extension->id() == presearch_extension_id ||
         extension->id() == presearch_rewards_extension_id;
}

}  // namespace extensions
