/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_PRIMARY_ACCOUNT_MUTATOR_IMPL_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_PRIMARY_ACCOUNT_MUTATOR_IMPL_H_

#include "components/signin/internal/identity_manager/primary_account_manager.h"
#include "components/signin/public/identity_manager/primary_account_mutator.h"

#define RevokeSyncConsent                                                      \
  RevokeSyncConsent_ChromiumImpl(signin_metrics::ProfileSignout source_metric, \
                                 signin_metrics::SignoutDelete delete_metric); \
  void RevokeSyncConsent

#include "../../../../../../components/signin/internal/identity_manager/primary_account_mutator_impl.h"

#undef RevokeSyncConsent

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_PRIMARY_ACCOUNT_MUTATOR_IMPL_H_
