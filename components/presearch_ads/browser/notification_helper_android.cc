/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_ads/browser/notification_helper_android.h"

#include <string>

#include "base/android/jni_string.h"
#include "base/system/sys_info.h"
#include "bat/ads/pref_names.h"
#include "presearch/browser/presearch_ads/android/jni_headers/PresearchAdsSignupDialog_jni.h"
#include "presearch/browser/presearch_ads/android/jni_headers/PresearchAds_jni.h"
#include "presearch/build/android/jni_headers/PresearchNotificationSettingsBridge_jni.h"
#include "presearch/common/presearch_channel_info.h"
#include "presearch/components/presearch_ads/browser/background_helper.h"
#include "chrome/android/chrome_jni_headers/NotificationSystemStatusUtil_jni.h"
#include "chrome/browser/notifications/notification_channels_provider_android.h"

namespace presearch_ads {

namespace {

const int kMinimumVersionForNotificationChannels = 8;

const int kAppNotificationStatusUndeterminable = 0;
const int kAppNotificationsStatusEnabled = 2;

}  // namespace

NotificationHelperAndroid::NotificationHelperAndroid() = default;

NotificationHelperAndroid::~NotificationHelperAndroid() = default;

bool NotificationHelperAndroid::ShouldShowNotifications() {
  if (presearch::IsNightlyOrDeveloperBuild()) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  int status = Java_NotificationSystemStatusUtil_getAppNotificationStatus(env);
  bool is_notifications_enabled =
      (status == kAppNotificationsStatusEnabled ||
       status == kAppNotificationStatusUndeterminable);

  bool is_foreground = BackgroundHelper::GetInstance()->IsForeground();
  bool is_notification_channel_enabled =
      IsPresearchAdsNotificationChannelEnabled(is_foreground);
  bool result = is_notifications_enabled && is_notification_channel_enabled;
  if (!is_foreground) {
    result = result && CanShowBackgroundNotifications();
  }
  return result;
}

bool NotificationHelperAndroid::ShowMyFirstAdNotification() {
  if (!ShouldShowNotifications()) {
    return false;
  }

  const bool use_custom_notifications = presearch::IsNightlyOrDeveloperBuild();

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchAdsSignupDialog_enqueueOnboardingNotificationNative(
      env, use_custom_notifications);

  return true;
}

bool NotificationHelperAndroid::CanShowBackgroundNotifications() const {
  if (presearch::IsNightlyOrDeveloperBuild()) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  return Java_PresearchAdsSignupDialog_showAdsInBackground(env);
}

NotificationHelperAndroid* NotificationHelperAndroid::GetInstanceImpl() {
  return base::Singleton<NotificationHelperAndroid>::get();
}

NotificationHelper* NotificationHelper::GetInstanceImpl() {
  return NotificationHelperAndroid::GetInstanceImpl();
}

///////////////////////////////////////////////////////////////////////////////

bool NotificationHelperAndroid::IsPresearchAdsNotificationChannelEnabled(
    bool foreground_channel) const {
  if (GetOperatingSystemVersion() < kMinimumVersionForNotificationChannels) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  auto j_channel_id = (foreground_channel)
                          ? Java_PresearchAds_getPresearchAdsChannelId(env)
                          : Java_PresearchAds_getPresearchAdsBackgroundChannelId(env);
  auto status = static_cast<NotificationChannelStatus>(
      Java_PresearchNotificationSettingsBridge_getChannelStatus(env, j_channel_id));

  return (status == NotificationChannelStatus::ENABLED ||
          status == NotificationChannelStatus::UNAVAILABLE);
}

int NotificationHelperAndroid::GetOperatingSystemVersion() const {
  int32_t major_version = 0;
  int32_t minor_version = 0;
  int32_t bugfix_version = 0;

  base::SysInfo::OperatingSystemVersionNumbers(&major_version, &minor_version,
                                               &bugfix_version);

  return major_version;
}

}  // namespace presearch_ads
