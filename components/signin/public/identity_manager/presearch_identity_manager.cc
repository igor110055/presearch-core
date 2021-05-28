/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/signin/public/identity_manager/presearch_identity_manager.h"

#include <utility>
#include <vector>

#include "components/signin/public/base/signin_client.h"
#include "components/signin/public/identity_manager/accounts_in_cookie_jar_info.h"
#include "components/signin/public/identity_manager/identity_manager.h"

namespace signin {

PresearchIdentityManager::PresearchIdentityManager(
    IdentityManager::InitParameters&& parameters)
    : IdentityManager(std::move(parameters)) {}

PresearchIdentityManager::~PresearchIdentityManager() {}

AccountsInCookieJarInfo PresearchIdentityManager::GetAccountsInCookieJar() const {
  // accounts_in_cookie_jar_info.accounts_are_fresh must be false,
  // see `ProfileSyncService::OnEngineInitialized`
  return AccountsInCookieJarInfo(false, std::vector<gaia::ListedAccount>(),
                                 std::vector<gaia::ListedAccount>());
}

}  // namespace signin
