/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_wallet/presearch_wallet_service_factory.h"

#include <memory>

#include "presearch/browser/presearch_wallet/presearch_wallet_delegate_impl.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_service.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/browser/extension_registry_factory.h"
#include "extensions/browser/extension_system.h"
#include "extensions/browser/extension_system_provider.h"
#include "extensions/browser/extensions_browser_client.h"
#endif  // #if BUILDFLAG(ENABLE_EXTENSIONS)

// static
PresearchWalletServiceFactory* PresearchWalletServiceFactory::GetInstance() {
  return base::Singleton<PresearchWalletServiceFactory>::get();
}

// static
PresearchWalletService* PresearchWalletServiceFactory::GetForContext(
    content::BrowserContext* context) {
  return static_cast<PresearchWalletService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

PresearchWalletServiceFactory::PresearchWalletServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "PresearchWalletService",
          BrowserContextDependencyManager::GetInstance()) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  DependsOn(extensions::ExtensionRegistryFactory::GetInstance());
  DependsOn(
      extensions::ExtensionsBrowserClient::Get()->GetExtensionSystemFactory());
#endif  // #if BUILDFLAG(ENABLE_EXTENSIONS)
}

PresearchWalletServiceFactory::~PresearchWalletServiceFactory() {}

KeyedService* PresearchWalletServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new PresearchWalletService(context,
                                std::make_unique<PresearchWalletDelegateImpl>());
}

content::BrowserContext* PresearchWalletServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}
