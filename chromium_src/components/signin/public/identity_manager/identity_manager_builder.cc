/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../../../../../../components/signin/public/identity_manager/identity_manager_builder.cc"

#include "presearch/components/signin/internal/identity_manager/presearch_primary_account_mutator_impl.h"
#include "presearch/components/signin/public/identity_manager/presearch_identity_manager.h"

namespace signin {

namespace {

IdentityManager::InitParameters BuildPresearchIdentityManagerInitParameters(
    IdentityManagerBuildParams* params) {
  IdentityManager::InitParameters init_params =
      BuildIdentityManagerInitParameters(params);

  init_params.primary_account_mutator =
      std::make_unique<PresearchPrimaryAccountMutatorImpl>(
          init_params.account_tracker_service.get(),
          init_params.token_service.get(),
          init_params.primary_account_manager.get(), params->pref_service,
          params->account_consistency);

  return init_params;
}

}  // namespace

std::unique_ptr<PresearchIdentityManager> BuildPresearchIdentityManager(
    IdentityManagerBuildParams* params) {
  return std::make_unique<PresearchIdentityManager>(
      BuildPresearchIdentityManagerInitParameters(params));
}

}  // namespace signin
