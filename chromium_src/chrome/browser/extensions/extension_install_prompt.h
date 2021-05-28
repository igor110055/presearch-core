/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_PROMPT_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_PROMPT_H_

#define GetDialogTitle    \
  GetDialogTitle_ChromiumImpl() const; \
  base::string16 GetDialogTitle

#include "../../../../../chrome/browser/extensions/extension_install_prompt.h"
#undef GetDialogTitle

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_PROMPT_H_

