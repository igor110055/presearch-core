/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/omnibox/browser/suggested_sites_provider.h"
#include "presearch/components/omnibox/browser/topsites_provider.h"

#define PRESEARCH_AUTOCOMPLETE_CONTROLLER_AUTOCOMPLETE_CONTROLLER         \
  providers_.push_back(new TopSitesProvider(provider_client_.get())); \
  providers_.push_back(new SuggestedSitesProvider(provider_client_.get()));

#include "../../../../../components/omnibox/browser/autocomplete_controller.cc"
#undef PRESEARCH_AUTOCOMPLETE_CONTROLLER_AUTOCOMPLETE_CONTROLLER

