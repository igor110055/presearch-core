/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_ads/android/presearch_ads_native_helper.h"

#include <memory>
#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "presearch/browser/presearch_ads/android/jni_headers/PresearchAdsNativeHelper_jni.h"
#include "presearch/browser/presearch_rewards/rewards_service_factory.h"
#include "presearch/components/presearch_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"

using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;

namespace presearch_ads {

class AdsNotificationhandler;
class AdsService;
class AdsServiceFactory;
class AdsServiceImpl;

// static

jboolean JNI_PresearchAdsNativeHelper_IsPresearchAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile);
  if (!ads_service_) {
    NOTREACHED();
    return false;
  }

  return ads_service_->IsEnabled();
}

jboolean JNI_PresearchAdsNativeHelper_IsLocaleValid(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile);
  if (!ads_service_) {
    NOTREACHED();
    return false;
  }

  return ads_service_->IsSupportedLocale();
}

jboolean JNI_PresearchAdsNativeHelper_IsSupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile);
  if (!ads_service_) {
    NOTREACHED();
    return false;
  }

  return ads_service_->IsSupportedLocale();
}

jboolean JNI_PresearchAdsNativeHelper_IsNewlySupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile);
  if (!ads_service_) {
    NOTREACHED();
    return false;
  }

  return ads_service_->IsNewlySupportedLocale();
}

void JNI_PresearchAdsNativeHelper_SetAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  auto* rewards_service =
      presearch_rewards::RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    NOTREACHED();
    return;
  }

  rewards_service->SetAdsEnabled(true);
}

void JNI_PresearchAdsNativeHelper_AdNotificationClicked(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  presearch_ads::AdsServiceImpl* ads_service =
      presearch_ads::AdsServiceFactory::GetImplForProfile(profile);
  if (!ads_service) {
    NOTREACHED();
    return;
  }
  std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  std::unique_ptr<presearch_ads::AdsNotificationHandler> handler =
      std::make_unique<presearch_ads::AdsNotificationHandler>(
          static_cast<content::BrowserContext*>(profile));
  handler->SetAdsService(ads_service);
  handler->OnClick(profile, GURL(""), notification_id, base::nullopt,
                   base::nullopt, base::OnceClosure());
}

void JNI_PresearchAdsNativeHelper_AdNotificationDismissed(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id,
    jboolean j_by_user) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  presearch_ads::AdsServiceImpl* ads_service =
      presearch_ads::AdsServiceFactory::GetImplForProfile(profile);
  if (!ads_service) {
    NOTREACHED();
    return;
  }
  std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  std::unique_ptr<presearch_ads::AdsNotificationHandler> handler =
      std::make_unique<presearch_ads::AdsNotificationHandler>(
          static_cast<content::BrowserContext*>(profile));
  handler->SetAdsService(ads_service);
  handler->OnClose(profile, GURL(""), notification_id, j_by_user,
                   base::OnceClosure());
}

}  // namespace presearch_ads
