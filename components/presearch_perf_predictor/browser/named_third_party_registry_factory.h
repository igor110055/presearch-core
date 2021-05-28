/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_PERF_PREDICTOR_BROWSER_NAMED_THIRD_PARTY_REGISTRY_FACTORY_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_PERF_PREDICTOR_BROWSER_NAMED_THIRD_PARTY_REGISTRY_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"

class Profile;

namespace presearch_perf_predictor {

class NamedThirdPartyRegistry;

class NamedThirdPartyRegistryFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  static NamedThirdPartyRegistryFactory* GetInstance();
  static NamedThirdPartyRegistry* GetForBrowserContext(
      content::BrowserContext* context);

 private:
  friend struct base::DefaultSingletonTraits<NamedThirdPartyRegistryFactory>;
  NamedThirdPartyRegistryFactory();
  ~NamedThirdPartyRegistryFactory() override;

  NamedThirdPartyRegistryFactory(const NamedThirdPartyRegistryFactory&) =
      delete;
  NamedThirdPartyRegistryFactory& operator=(
      const NamedThirdPartyRegistryFactory&) = delete;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;
};

}  // namespace presearch_perf_predictor

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_PERF_PREDICTOR_BROWSER_NAMED_THIRD_PARTY_REGISTRY_FACTORY_H_
