/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_REWARDS_REWARDS_SERVICE_FACTORY_H_
#define PRESEARCH_BROWSER_PRESEARCH_REWARDS_REWARDS_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "presearch/components/presearch_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile_manager_observer.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;

namespace presearch_rewards {
class RewardsService;

// Singleton that owns all RewardsService and associates them with Profiles.
class RewardsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  RewardsServiceFactory(const RewardsServiceFactory&) = delete;
  RewardsServiceFactory& operator=(const RewardsServiceFactory&) = delete;

  static presearch_rewards::RewardsService* GetForProfile(Profile* profile);

  static RewardsServiceFactory* GetInstance();

  static void SetServiceForTesting(RewardsService* service);

 private:
  friend struct base::DefaultSingletonTraits<RewardsServiceFactory>;

  RewardsServiceFactory();
  ~RewardsServiceFactory() override = default;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
};

}  // namespace presearch_rewards

#endif  // PRESEARCH_BROWSER_PRESEARCH_REWARDS_REWARDS_SERVICE_FACTORY_H_
