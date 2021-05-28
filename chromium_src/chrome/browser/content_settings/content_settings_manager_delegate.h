/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_MANAGER_DELEGATE_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_MANAGER_DELEGATE_H_

#define PRESEARCH_CONTENT_SETTINGS_MANAGER_DELEGATE_H_  \
 private:                                           \
  friend class PresearchContentSettingsManagerDelegate; \
                                                    \
 public:
#include "../../../../../chrome/browser/content_settings/content_settings_manager_delegate.h"
#undef PRESEARCH_CONTENT_SETTINGS_MANAGER_DELEGATE_H_

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_MANAGER_DELEGATE_H_
