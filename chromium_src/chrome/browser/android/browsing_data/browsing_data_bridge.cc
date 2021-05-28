/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define PRESEARCH_CLEAR_BROWSING_DATA                          \
  remove_mask |= BrowsingDataRemover::DATA_TYPE_DOWNLOADS; \
  break;                                                   \

#include "../../../../../../chrome/browser/android/browsing_data/browsing_data_bridge.cc"
#undef PRESEARCH_CLEAR_BROWSING_DATA
