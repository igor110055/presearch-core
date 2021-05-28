/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_P3A_PRESEARCH_HISTOGRAM_REWRITE_H_
#define PRESEARCH_COMPONENTS_P3A_PRESEARCH_HISTOGRAM_REWRITE_H_

namespace presearch {

// Set callbacks for existing Chromium histograms that will be presearchized,
// i.e. reemitted using a different name and custom buckets.
void SetupHistogramsPresearchization();

}  // namespace presearch

#endif  // PRESEARCH_COMPONENTS_P3A_PRESEARCH_HISTOGRAM_REWRITE_H_
