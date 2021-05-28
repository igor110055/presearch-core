/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/themes/presearch_theme_service.h"
// Below files are included in advance to prevent overriding
// GetBrowserContextToUse.
#include "chrome/browser/extensions/extension_system_factory.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "extensions/browser/extension_prefs_factory.h"
#include "extensions/browser/extension_registry_factory.h"

#if !defined(OS_LINUX)
#define PRESEARCH_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR \
  using ThemeService = PresearchThemeService;
#else
// On Linux ThemeServiceAuraLinux derives from PresearchThemeService instead.
#define PRESEARCH_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR
#endif

#define GetBrowserContextToUse GetBrowserContextToUse_ChromiumImpl

#include "../../../../../chrome/browser/themes/theme_service_factory.cc"
#undef PRESEARCH_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR
#undef GetBrowserContextToUse

content::BrowserContext* ThemeServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  if (context->IsTor())
    return context;
  return ThemeServiceFactory::GetBrowserContextToUse_ChromiumImpl(context);
}
