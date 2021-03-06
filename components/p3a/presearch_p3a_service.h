/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_P3A_PRESEARCH_P3A_SERVICE_H_
#define PRESEARCH_COMPONENTS_P3A_PRESEARCH_P3A_SERVICE_H_

#include <memory>
#include <string>

#include "base/containers/flat_map.h"
#include "base/memory/ref_counted.h"
#include "base/metrics/histogram_base.h"
#include "base/timer/timer.h"
#include "presearch/components/presearch_prochlo/presearch_prochlo_message.h"
#include "presearch/components/p3a/presearch_p3a_log_store.h"
#include "url/gurl.h"

class PrefRegistrySimple;

namespace network {
class SharedURLLoaderFactory;
}

namespace presearch {

class PresearchP3AScheduler;
class PresearchP3AUploader;

// Core class for Presearch Privacy-Preserving Product Analytics machinery.
// Works on UI thread. Refcounted to receive histogram updating callbacks
// on any thread.
// TODO(iefremov): It should be possible to get rid of refcounted here.
class PresearchP3AService : public base::RefCountedThreadSafe<PresearchP3AService>,
                        public PresearchP3ALogStore::Delegate {
 public:
  explicit PresearchP3AService(PrefService* local_state);

  static void RegisterPrefs(PrefRegistrySimple* registry, bool first_run);

  // Should be called right after constructor to subscribe to histogram
  // updates. Can't call it in constructor because of refcounted peculiarities.
  void InitCallbacks();

  // Needs a living browser process to complete the initialization.
  void Init(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);

  // PresearchP3ALogStore::Delegate
  std::string Serialize(base::StringPiece histogram_name,
                        uint64_t value) override;

  // May be accessed from multiple threads, so this is thread-safe.
  bool IsActualMetric(base::StringPiece histogram_name) const override;

 private:
  friend class base::RefCountedThreadSafe<PresearchP3AService>;
  ~PresearchP3AService() override;

  void MaybeOverrideSettingsFromCommandLine();

  void InitPyxisMeta();

  // Updates things that change over time: week of survey, etc.
  void UpdatePyxisMeta();

  void StartScheduledUpload();

  // Invoked by callbacks registered by our service. Since these callbacks
  // can fire on any thread, this method reposts everything to UI thread.
  void OnHistogramChanged(const char* histogram_name,
                          uint64_t name_hash,
                          base::HistogramBase::Sample sample);

  void OnHistogramChangedOnUI(const char* histogram_name,
                              base::HistogramBase::Sample sample,
                              size_t bucket);

  // Updates or removes a metric from the log.
  void HandleHistogramChange(base::StringPiece histogram_name, size_t bucket);

  void OnLogUploadComplete(int response_code, int error_code, bool was_https);

  // Restart the uploading process (i.e. mark all values as unsent).
  void DoRotation();

  void UpdateRotationTimer();

  // General prefs:
  bool initialized_ = false;
  PrefService* local_state_ = nullptr;

  // The average interval between uploading different values.
  base::TimeDelta average_upload_interval_;
  bool randomize_upload_interval_ = true;
  // Interval between rotations, only used for testing from the command line.
  base::TimeDelta rotation_interval_;
  GURL upload_server_url_;

  prochlo::MessageMetainfo pyxis_meta_;

  // Components:
  std::unique_ptr<PresearchP3ALogStore> log_store_;
  std::unique_ptr<PresearchP3AUploader> uploader_;
  std::unique_ptr<PresearchP3AScheduler> upload_scheduler_;

  // Used to store histogram values that are produced between constructing
  // the service and its initialization.
  base::flat_map<base::StringPiece, size_t> histogram_values_;

  // Once fired we restart the overall uploading process.
  base::OneShotTimer rotation_timer_;

  DISALLOW_COPY_AND_ASSIGN(PresearchP3AService);
};

}  // namespace presearch

#endif  // PRESEARCH_COMPONENTS_P3A_PRESEARCH_P3A_SERVICE_H_
