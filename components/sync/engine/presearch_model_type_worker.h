/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_SYNC_ENGINE_PRESEARCH_MODEL_TYPE_WORKER_H_
#define PRESEARCH_COMPONENTS_SYNC_ENGINE_PRESEARCH_MODEL_TYPE_WORKER_H_

#include <memory>

#include "base/feature_list.h"
#include "components/sync/base/model_type.h"
#include "components/sync/base/passphrase_enums.h"
#include "components/sync/engine/commit_and_get_updates_types.h"
#include "components/sync/engine/model_type_worker.h"

namespace syncer {

class CancelationSignal;
class Cryptographer;
class NudgeHandler;
class ModelTypeProcessor;

namespace features {

extern const base::Feature kPresearchSyncResetProgressMarker;

}  // namespace features

FORWARD_DECLARE_TEST(PresearchModelTypeWorkerTest, ResetProgressMarker);
FORWARD_DECLARE_TEST(PresearchModelTypeWorkerTest, ResetProgressMarkerMaxPeriod);
FORWARD_DECLARE_TEST(PresearchModelTypeWorkerTest,
                     ResetProgressMarkerDisabledFeature);

class PresearchModelTypeWorker : public ModelTypeWorker {
 public:
  PresearchModelTypeWorker(ModelType type,
                       const sync_pb::ModelTypeState& initial_state,
                       bool trigger_initial_sync,
                       std::unique_ptr<Cryptographer> cryptographer,
                       PassphraseType passphrase_type,
                       NudgeHandler* nudge_handler,
                       std::unique_ptr<ModelTypeProcessor> model_type_processor,
                       CancelationSignal* cancelation_signal);
  ~PresearchModelTypeWorker() override;
  PresearchModelTypeWorker(const PresearchModelTypeWorker&) = delete;
  PresearchModelTypeWorker& operator=(const PresearchModelTypeWorker&) = delete;

 private:
  FRIEND_TEST_ALL_PREFIXES(PresearchModelTypeWorkerTest, ResetProgressMarker);
  FRIEND_TEST_ALL_PREFIXES(PresearchModelTypeWorkerTest,
                           ResetProgressMarkerMaxPeriod);
  FRIEND_TEST_ALL_PREFIXES(PresearchModelTypeWorkerTest,
                           ResetProgressMarkerDisabledFeature);

  void OnCommitResponse(
      const CommitResponseDataList& committed_response_list,
      const FailedCommitResponseDataList& error_response_list) override;

  bool IsResetProgressMarkerRequired(
      const FailedCommitResponseDataList& error_response_list);
  void ResetProgressMarker();

  size_t failed_commit_times_ = 0;
  base::Time last_reset_marker_time_;
  static size_t GetFailuresToResetMarkerForTests();
  static base::TimeDelta MinimalTimeBetweenResetForTests();
};

}  // namespace syncer

#endif  // PRESEARCH_COMPONENTS_SYNC_ENGINE_PRESEARCH_MODEL_TYPE_WORKER_H_
