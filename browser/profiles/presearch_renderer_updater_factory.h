/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PROFILES_PRESEARCH_RENDERER_UPDATER_FACTORY_H_
#define PRESEARCH_BROWSER_PROFILES_PRESEARCH_RENDERER_UPDATER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;
class PresearchRendererUpdater;

// Singleton that creates/deletes PresearchRendererUpdater as new Profiles are
// created/shutdown.
class PresearchRendererUpdaterFactory : public BrowserContextKeyedServiceFactory {
 public:
  // Returns an instance of the PresearchRendererUpdaterFactory singleton.
  static PresearchRendererUpdaterFactory* GetInstance();

  // Returns the instance of RendererUpdater for the passed |profile|.
  static PresearchRendererUpdater* GetForProfile(Profile* profile);

  PresearchRendererUpdaterFactory(const PresearchRendererUpdaterFactory&) = delete;
  PresearchRendererUpdaterFactory& operator=(const PresearchRendererUpdaterFactory&) =
      delete;

 protected:
  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;

 private:
  friend struct base::DefaultSingletonTraits<PresearchRendererUpdaterFactory>;

  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  PresearchRendererUpdaterFactory();
  ~PresearchRendererUpdaterFactory() override;
};

#endif  // PRESEARCH_BROWSER_PROFILES_PRESEARCH_RENDERER_UPDATER_FACTORY_H_
