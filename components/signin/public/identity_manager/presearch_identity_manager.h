/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_SIGNIN_PUBLIC_IDENTITY_MANAGER_PRESEARCH_IDENTITY_MANAGER_H_
#define PRESEARCH_COMPONENTS_SIGNIN_PUBLIC_IDENTITY_MANAGER_PRESEARCH_IDENTITY_MANAGER_H_

#include <memory>

#include "components/signin/public/identity_manager/identity_manager.h"

// Purpose of this class is to redirect to empty implementation
// syncer::ProfileSyncService::identity_manager_
// These must be overridden and empty:
//  GetAccountsInCookieJar
//  GetPrimaryAccountMutator - done in `BuildPresearchIdentityManagerInitParameters`

class GoogleServiceAuthError;
class PresearchAccountFetcherService;

namespace signin {

struct AccountsInCookieJarInfo;
class PrimaryAccountMutator;
class PresearchPrimaryAccountMutator;

class PresearchIdentityManager : public IdentityManager {
 public:
  explicit PresearchIdentityManager(IdentityManager::InitParameters&& parameters);
  ~PresearchIdentityManager() override;

  PresearchIdentityManager(const PresearchIdentityManager&) = delete;
  PresearchIdentityManager& operator=(const PresearchIdentityManager&) = delete;

  AccountsInCookieJarInfo GetAccountsInCookieJar() const override;
};

}  // namespace signin

#endif  // PRESEARCH_COMPONENTS_SIGNIN_PUBLIC_IDENTITY_MANAGER_PRESEARCH_IDENTITY_MANAGER_H_
