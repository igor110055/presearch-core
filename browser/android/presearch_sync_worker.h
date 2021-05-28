/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_ANDROID_PRESEARCH_SYNC_WORKER_H_
#define PRESEARCH_BROWSER_ANDROID_PRESEARCH_SYNC_WORKER_H_

#include <jni.h>
#include <string>

#include "base/android/jni_weak_ref.h"
#include "base/scoped_observer.h"
#include "components/sync/driver/sync_service.h"
#include "components/sync/driver/sync_service_observer.h"

class Profile;

namespace syncer {
class PresearchProfileSyncService;
}  // namespace syncer

namespace chrome {
namespace android {

class PresearchSyncWorker : public syncer::SyncServiceObserver {
 public:
  PresearchSyncWorker(JNIEnv* env,
                  const base::android::JavaRef<jobject>& obj);
  ~PresearchSyncWorker() override;

  void Destroy(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetSyncCodeWords(JNIEnv* env);

  void SaveCodeWords(JNIEnv* env,
                     const base::android::JavaParamRef<jstring>& passphrase);

  void RequestSync(JNIEnv* env);

  void FinalizeSyncSetup(JNIEnv* env);

  bool IsFirstSetupComplete(JNIEnv* env);

  void ResetSync(JNIEnv* env);

  bool GetSyncV1WasEnabled(JNIEnv* env);

  bool GetSyncV2MigrateNoticeDismissed(JNIEnv* env);

  void SetSyncV2MigrateNoticeDismissed(
      JNIEnv* env,
      bool sync_v2_migration_notice_dismissed);

 private:
  syncer::PresearchProfileSyncService* GetSyncService() const;
  void MarkFirstSetupComplete();

  // syncer::SyncServiceObserver implementation.
  void OnStateChanged(syncer::SyncService* service) override;

  void OnResetDone();

  void SetEncryptionPassphrase(syncer::SyncService* service);
  void SetDecryptionPassphrase(syncer::SyncService* service);

  JavaObjectWeakGlobalRef weak_java_presearch_sync_worker_;
  Profile* profile_ = nullptr;

  std::string passphrase_;

  ScopedObserver<syncer::SyncService, syncer::SyncServiceObserver>
      sync_service_observer_{this};
  base::WeakPtrFactory<PresearchSyncWorker> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(PresearchSyncWorker);
};

}  // namespace android
}  // namespace chrome

#endif  // PRESEARCH_BROWSER_ANDROID_PRESEARCH_SYNC_WORKER_H_
