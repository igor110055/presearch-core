/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "base/android/jni_android.h"
#include "presearch/browser/presearch_browser_process.h"
#include "presearch/browser/presearch_stats/presearch_stats_updater.h"
#include "presearch/build/android/jni_headers/PresearchActivity_jni.h"

namespace chrome {
namespace android {

static void JNI_PresearchActivity_RestartStatsUpdater(JNIEnv* env) {
  g_presearch_browser_process->presearch_stats_updater()->Stop();
  g_presearch_browser_process->presearch_stats_updater()->Start();
}

}  // namespace android
}  // namespace chrome
