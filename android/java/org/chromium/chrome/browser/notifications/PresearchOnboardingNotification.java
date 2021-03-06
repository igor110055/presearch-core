/**
 * Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications;

import android.app.Notification;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.dialogs.PresearchAdsNotificationDialog;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationPublisher;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.components.browser_ui.notifications.NotificationManagerProxyImpl;
import org.chromium.components.browser_ui.notifications.NotificationMetadata;
import org.chromium.components.browser_ui.notifications.NotificationWrapper;
import org.chromium.components.browser_ui.notifications.PendingIntentProvider;

import java.util.Locale;

public class PresearchOnboardingNotification extends BroadcastReceiver {
    public Context mContext;
    private Intent mIntent;

    private static final int PRESEARCH_ONBOARDING_NOTIFICATION_ID = -2;
    public static String PRESEARCH_ONBOARDING_NOTIFICATION_TAG = "presearch_onboarding_notification_tag";
    private static String PRESEARCH_ONBOARDING_ORIGIN_EN = "https://presearch.org/my-first-ad/";
    private static String PRESEARCH_ONBOARDING_ORIGIN_DE = "https://presearch.org/de/my-first-ad/";
    private static String PRESEARCH_ONBOARDING_ORIGIN_FR = "https://presearch.org/fr/my-first-ad/";
    public static final String DEEP_LINK = "deep_link";
    public static final String USE_CUSTOM_NOTIFICATION = "use_custom_notification";

    private static final String COUNTRY_CODE_DE = "de_DE";
    private static final String COUNTRY_CODE_FR = "fr_FR";

    public static void showOnboardingDialog() {}

    public static void showOnboardingNotification() {}

    public static PendingIntentProvider getDeepLinkIntent(Context context) {
        Intent intent = new Intent(context, PresearchOnboardingNotification.class);
        intent.setAction(DEEP_LINK);
        return new PendingIntentProvider(
                PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT),
                0, 0);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        PresearchActivity presearchActivity = PresearchActivity.getPresearchActivity();
        if (action != null && action.equals(DEEP_LINK)) {
            if (presearchActivity != null) {
                presearchActivity.openRewardsPanel();
                Intent launchIntent = new Intent(Intent.ACTION_MAIN);
                launchIntent.setPackage(context.getPackageName());
                launchIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(launchIntent);
            } else {
                intent.putExtra(RetentionNotificationUtil.NOTIFICATION_TYPE, RetentionNotificationUtil.DAY_10);
                RetentionNotificationPublisher.backgroundNotificationAction(context, intent);
            }
        } else {
            if (presearchActivity != null) {
                presearchActivity.hideRewardsOnboardingIcon();
            }
        }
    }

    private static String getNotificationUrl() {
        Locale locale = Locale.getDefault();
        switch (locale.toString()) {
        case COUNTRY_CODE_DE:
            return PRESEARCH_ONBOARDING_ORIGIN_DE;
        case COUNTRY_CODE_FR:
            return PRESEARCH_ONBOARDING_ORIGIN_FR;
        default:
            return PRESEARCH_ONBOARDING_ORIGIN_EN;
        }
    }

    public static void cancelOnboardingNotification(Context context) {
        NotificationManagerProxyImpl notificationManager =
            new NotificationManagerProxyImpl(context);
        notificationManager.cancel(
            PRESEARCH_ONBOARDING_NOTIFICATION_TAG, PRESEARCH_ONBOARDING_NOTIFICATION_ID);
    }
}
