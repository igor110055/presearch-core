/** Copyright (c) 2019 The Presearch Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser;

import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.profiles.Profile;

@JNINamespace("presearch_ads")
public class PresearchAdsNativeHelper {
    private PresearchAdsNativeHelper() {}

    public static boolean nativeIsPresearchAdsEnabled(Profile profile) {
        return PresearchAdsNativeHelperJni.get().isPresearchAdsEnabled(profile);
    };

    public static boolean nativeIsLocaleValid(Profile profile) {
        return PresearchAdsNativeHelperJni.get().isLocaleValid(profile);
    };

    public static void nativeSetAdsEnabled(Profile profile) {
        PresearchAdsNativeHelperJni.get().setAdsEnabled(profile);
    };

    public static boolean nativeIsNewlySupportedLocale(Profile profile) {
        return PresearchAdsNativeHelperJni.get().isNewlySupportedLocale(profile);
    };

    public static boolean nativeIsSupportedLocale(Profile profile) {
        return PresearchAdsNativeHelperJni.get().isSupportedLocale(profile);
    };

    public static void nativeAdNotificationClicked(Profile profile, String j_notification_id) {
        PresearchAdsNativeHelperJni.get().adNotificationClicked(profile, j_notification_id);
    };

    public static void nativeAdNotificationDismissed(
            Profile profile, String j_notification_id, boolean j_by_user) {
        PresearchAdsNativeHelperJni.get().adNotificationDismissed(
                profile, j_notification_id, j_by_user);
    };

    @NativeMethods
    interface Natives {
        boolean isPresearchAdsEnabled(Profile profile);
        boolean isLocaleValid(Profile profile);
        void setAdsEnabled(Profile profile);
        boolean isNewlySupportedLocale(Profile profile);
        boolean isSupportedLocale(Profile profile);
        void adNotificationClicked(Profile profile, String j_notification_id);
        void adNotificationDismissed(Profile profile, String j_notification_id, boolean j_by_user);
    }
}
