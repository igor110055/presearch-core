/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_ADS_ANDROID_PRESEARCH_ADS_NATIVE_HELPER_H_
#define PRESEARCH_BROWSER_PRESEARCH_ADS_ANDROID_PRESEARCH_ADS_NATIVE_HELPER_H_

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/browser/presearch_ads/android/jni_headers/PresearchAdsNativeHelper_jni.h"
#include "presearch/components/presearch_ads/browser/ads_notification_handler.h"
#include "presearch/components/presearch_ads/browser/ads_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"

using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;

namespace presearch_ads {

// static

static jboolean JNI_PresearchAdsNativeHelper_IsPresearchAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static jboolean JNI_PresearchAdsNativeHelper_IsLocaleValid(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static jboolean JNI_PresearchAdsNativeHelper_IsSupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static jboolean JNI_PresearchAdsNativeHelper_IsNewlySupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static void JNI_PresearchAdsNativeHelper_SetAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static void JNI_PresearchAdsNativeHelper_AdNotificationClicked(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id);

static void JNI_PresearchAdsNativeHelper_AdNotificationDismissed(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id,
    jboolean j_by_user);

}  // namespace presearch_ads

#endif  // PRESEARCH_BROWSER_PRESEARCH_ADS_ANDROID_PRESEARCH_ADS_NATIVE_HELPER_H_
