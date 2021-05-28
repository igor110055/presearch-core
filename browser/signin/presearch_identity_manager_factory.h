/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_SIGNIN_PRESEARCH_IDENTITY_MANAGER_FACTORY_H_
#define PRESEARCH_BROWSER_SIGNIN_PRESEARCH_IDENTITY_MANAGER_FACTORY_H_

#include "base/memory/singleton.h"
#include "chrome/browser/signin/identity_manager_factory.h"

namespace signin {
class PresearchIdentityManager;
}

class Profile;

class PresearchIdentityManagerFactory : public IdentityManagerFactory {
 public:
  PresearchIdentityManagerFactory(const PresearchIdentityManagerFactory&) = delete;
  PresearchIdentityManagerFactory& operator=(const PresearchIdentityManagerFactory&) =
      delete;

  // Returns an instance of the IdentityManagerFactory singleton.
  static PresearchIdentityManagerFactory* GetInstance();

  static signin::PresearchIdentityManager* GetForProfile(Profile* profile);
  static signin::PresearchIdentityManager* GetForProfileIfExists(
      const Profile* profile);

 private:
  friend struct base::DefaultSingletonTraits<PresearchIdentityManagerFactory>;
  PresearchIdentityManagerFactory();
  ~PresearchIdentityManagerFactory() override;

  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) override;
};

#endif  // PRESEARCH_BROWSER_SIGNIN_PRESEARCH_IDENTITY_MANAGER_FACTORY_H_
