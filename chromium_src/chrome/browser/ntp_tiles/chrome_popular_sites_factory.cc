/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ntp_tiles/chrome_popular_sites_factory.h"

#include "presearch/components/ntp_tiles/presearch_popular_sites_impl.h"

#define PopularSitesImpl PresearchPopularSitesImpl
#include "../../../../../chrome/browser/ntp_tiles/chrome_popular_sites_factory.cc"  // NOLINT
#undef PopularSitesImpl
