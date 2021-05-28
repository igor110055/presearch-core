/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_
#define PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_

#include "base/gtest_prod_util.h"

namespace syncer {

FORWARD_DECLARE_TEST(PresearchModelTypeWorkerTest, ResetProgressMarker);
FORWARD_DECLARE_TEST(PresearchModelTypeWorkerTest, ResetProgressMarkerMaxPeriod);

}  // namespace syncer

#define PRESEARCH_MODEL_TYPE_WORKER_H_                                         \
 private:                                                                  \
  friend class PresearchModelTypeWorker;                                       \
  friend class PresearchModelTypeWorkerTest;                                   \
  FRIEND_TEST_ALL_PREFIXES(PresearchModelTypeWorkerTest, ResetProgressMarker); \
  FRIEND_TEST_ALL_PREFIXES(PresearchModelTypeWorkerTest,                       \
                           ResetProgressMarkerMaxPeriod);

#define OnCommitResponse virtual OnCommitResponse

#include "../../../../../components/sync/engine/model_type_worker.h"

#undef OnCommitResponse
#undef PRESEARCH_MODEL_TYPE_WORKER_H_

#endif  // PRESEARCH_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_
