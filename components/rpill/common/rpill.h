/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_RPILL_COMMON_RPILL_H_
#define PRESEARCH_COMPONENTS_RPILL_COMMON_RPILL_H_

#include "base/callback_forward.h"

namespace presearch_rpill {

using IsUncertainFutureCallback = base::OnceCallback<void(const bool)>;

// Detects if the future is uncertain or bright. The specified |callback| wil be
// run upon completion with the result.
void DetectUncertainFuture(IsUncertainFutureCallback callback);

}  // namespace presearch_rpill

#endif  // PRESEARCH_COMPONENTS_RPILL_COMMON_RPILL_H_
