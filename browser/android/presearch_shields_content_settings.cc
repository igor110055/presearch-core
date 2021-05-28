/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/android/presearch_shields_content_settings.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_util.h"

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "presearch/build/android/jni_headers/PresearchShieldsContentSettings_jni.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/content_settings/core/browser/content_settings_utils.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "url/gurl.h"

namespace chrome {
namespace android {


// That class is linked to a global toolbar. It's a one instance on Android
PresearchShieldsContentSettings* g_presearch_shields_content_settings = nullptr;

static void JNI_PresearchShieldsContentSettings_Init(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  g_presearch_shields_content_settings =
      new PresearchShieldsContentSettings(env, jcaller);
}

PresearchShieldsContentSettings::PresearchShieldsContentSettings(
    JNIEnv* env,
    const base::android::JavaRef<jobject>& obj)
    : jobj_(base::android::ScopedJavaGlobalRef<jobject>(obj)) {
  Java_PresearchShieldsContentSettings_setNativePtr(env, obj,
      reinterpret_cast<intptr_t>(this));
}

PresearchShieldsContentSettings::~PresearchShieldsContentSettings() {
}

void PresearchShieldsContentSettings::Destroy(JNIEnv* env) {
  g_presearch_shields_content_settings = nullptr;
  delete this;
}

void PresearchShieldsContentSettings::DispatchBlockedEventToJava(int tab_id,
        const std::string& block_type, const std::string& subresource) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchShieldsContentSettings_blockedEvent(
      env, jobj_, tab_id,
      base::android::ConvertUTF8ToJavaString(env, block_type),
      base::android::ConvertUTF8ToJavaString(env, subresource));
}

void PresearchShieldsContentSettings::DispatchSavedBandwidthToJava(
  uint64_t savings) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchShieldsContentSettings_savedBandwidth(env, jobj_, savings);
}

void PresearchShieldsContentSettings::DispatchSavedBandwidth(uint64_t savings) {
  DCHECK(g_presearch_shields_content_settings);
  if (!g_presearch_shields_content_settings) {
    return;
  }
  g_presearch_shields_content_settings->DispatchSavedBandwidthToJava(savings);
}

// static
void PresearchShieldsContentSettings::DispatchBlockedEvent(int tab_id,
  const std::string& block_type, const std::string& subresource) {
  DCHECK(g_presearch_shields_content_settings);
  if (!g_presearch_shields_content_settings) {
    return;
  }
  g_presearch_shields_content_settings->DispatchBlockedEventToJava(tab_id,
      block_type, subresource);
}

void JNI_PresearchShieldsContentSettings_SetPresearchShieldsEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::SetPresearchShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled,
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_PresearchShieldsContentSettings_GetPresearchShieldsEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return presearch_shields::GetPresearchShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_PresearchShieldsContentSettings_SetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      presearch_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_PresearchShieldsContentSettings_GetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::ControlType control_type =
      presearch_shields::GetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      presearch_shields::ControlTypeToString(control_type));
}

void JNI_PresearchShieldsContentSettings_SetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      presearch_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_PresearchShieldsContentSettings_GetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::ControlType control_type =
      presearch_shields::GetCookieControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      presearch_shields::ControlTypeToString(control_type));
}

void JNI_PresearchShieldsContentSettings_SetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      presearch_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_PresearchShieldsContentSettings_GetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::ControlType control_type =
      presearch_shields::GetFingerprintingControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      presearch_shields::ControlTypeToString(control_type));
}

void JNI_PresearchShieldsContentSettings_SetHTTPSEverywhereEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled,
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_PresearchShieldsContentSettings_GetHTTPSEverywhereEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return presearch_shields::GetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_PresearchShieldsContentSettings_SetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      presearch_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_PresearchShieldsContentSettings_GetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  presearch_shields::ControlType control_type =
      presearch_shields::GetNoScriptControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      presearch_shields::ControlTypeToString(control_type));
}

}  // namespace android
}  // namespace chrome
