/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_SIGNIN_IDENTITY_MANAGER_FACTORY_H_
#define PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_SIGNIN_IDENTITY_MANAGER_FACTORY_H_

class PresearchIdentityManagerFactory;

#define PRESEARCH_IDENTITY_MANAGER_FACTORY_H_   \
 private:                                   \
  friend class PresearchIdentityManagerFactory; \
  friend struct base::DefaultSingletonTraits<PresearchIdentityManagerFactory>;

#include "../../../../../chrome/browser/signin/identity_manager_factory.h"

#undef PRESEARCH_IDENTITY_MANAGER_FACTORY_H_

#endif  // PRESEARCH_CHROMIUM_SRC_CHROME_BROWSER_SIGNIN_IDENTITY_MANAGER_FACTORY_H_
