/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/profiles/presearch_renderer_updater_factory.h"

#include "presearch/browser/profiles/presearch_renderer_updater.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

PresearchRendererUpdaterFactory::PresearchRendererUpdaterFactory()
    : BrowserContextKeyedServiceFactory(
          "PresearchRendererUpdater",
          BrowserContextDependencyManager::GetInstance()) {}

PresearchRendererUpdaterFactory::~PresearchRendererUpdaterFactory() {}

// static
PresearchRendererUpdaterFactory* PresearchRendererUpdaterFactory::GetInstance() {
  return base::Singleton<PresearchRendererUpdaterFactory>::get();
}

// static
PresearchRendererUpdater* PresearchRendererUpdaterFactory::GetForProfile(
    Profile* profile) {
  return static_cast<PresearchRendererUpdater*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

KeyedService* PresearchRendererUpdaterFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new PresearchRendererUpdater(static_cast<Profile*>(context));
}

bool PresearchRendererUpdaterFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

content::BrowserContext* PresearchRendererUpdaterFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}
