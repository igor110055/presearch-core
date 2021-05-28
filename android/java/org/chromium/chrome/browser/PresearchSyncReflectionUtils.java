/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import java.lang.reflect.Method;

import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.chrome.browser.PresearchConfig;

// The purpose of this class is to hide PresearchSyncWorker object under `enable_presearch_sync`
// and create it without explict import
public class PresearchSyncReflectionUtils {
    private static Object sPresearchSyncWorker;
    private static boolean sInitialized;
    private static String TAG = "SYNC";

    public static Object getSyncWorker() {
        // May be invoked in non-UI thread when we do validation for camera QR in callback
        if (!sInitialized) {
            if (PresearchConfig.SYNC_ENABLED) {
                try {
                    sPresearchSyncWorker =
                        Class.forName("org.chromium.chrome.browser.PresearchSyncWorker")
                             .getConstructor()
                             .newInstance();
                } catch (Exception e) {
                    Log.e(TAG, "Cannot create PresearchSyncWorker ", e);
                }
            }
            sInitialized = true;
        }
        return sPresearchSyncWorker;
    }

    public static void showInformers() {
        if (!PresearchConfig.SYNC_ENABLED) {
            return;
        }

        try {
            Method method = Class.forName("org.chromium.chrome.browser.PresearchSyncInformers").getDeclaredMethod("show");
            method.invoke(null);
        } catch (Exception e) {
            Log.e(TAG, "Cannot show sync informers with reflection ", e);
        }
    }
}
