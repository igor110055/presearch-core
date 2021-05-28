/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.content.Context;
import android.content.SharedPreferences;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;

import java.lang.Runnable;

@JNINamespace("chrome::android")
public class PresearchSyncWorker {
    public static final String TAG = "SYNC";

    private Context mContext;
    private String mDebug = "true";

    private long mNativePresearchSyncWorker;

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativePresearchSyncWorker == 0;
        mNativePresearchSyncWorker = nativePtr;
    }

    private void Init() {
        if (mNativePresearchSyncWorker == 0) {
            PresearchSyncWorkerJni.get().init(PresearchSyncWorker.this);
        }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativePresearchSyncWorker != 0) {
            PresearchSyncWorkerJni.get().destroy(mNativePresearchSyncWorker);
            mNativePresearchSyncWorker = 0;
        }
    }

    public PresearchSyncWorker() {
        mContext = ContextUtils.getApplicationContext();
        Init();
        (new MigrationFromV1()).MigrateFromSyncV1();
    }

    private class MigrationFromV1 {
        // Deprecated
        public static final String PREF_NAME = "SyncPreferences";
        private static final String PREF_LAST_FETCH_NAME = "TimeLastFetch";
        private static final String PREF_LATEST_DEVICE_RECORD_TIMESTAMPT_NAME =
                "LatestDeviceRecordTime";
        private static final String PREF_LAST_TIME_SEND_NOT_SYNCED_NAME = "TimeLastSendNotSynced";
        public static final String PREF_DEVICE_ID = "DeviceId";
        public static final String PREF_BASE_ORDER = "BaseOrder";
        public static final String PREF_LAST_ORDER = "LastOrder";
        public static final String PREF_SEED = "Seed";
        public static final String PREF_SYNC_DEVICE_NAME = "SyncDeviceName";
        private static final String PREF_SYNC_SWITCH = "sync_switch";
        private static final String PREF_SYNC_BOOKMARKS = "presearch_sync_bookmarks";
        public static final String PREF_SYNC_TABS = "presearch_sync_tabs"; // never used
        public static final String PREF_SYNC_HISTORY = "presearch_sync_history"; // never used
        public static final String PREF_SYNC_AUTOFILL_PASSWORDS =
                "presearch_sync_autofill_passwords"; // never used
        public static final String PREF_SYNC_PAYMENT_SETTINGS =
                "presearch_sync_payment_settings"; // never used

        private boolean HaveSyncV1Prefs() {
            SharedPreferences sharedPref = mContext.getSharedPreferences(PREF_NAME, 0);

            String deviceId = sharedPref.getString(PREF_DEVICE_ID, null);
            if (null == deviceId) {
                return false;
            }
            return true;
        }

        private void DeleteSyncV1Prefs() {
            SharedPreferences sharedPref = mContext.getSharedPreferences(PREF_NAME, 0);
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.clear().apply();
        }

        private void DeleteSyncV1LevelDb() {
            PresearchSyncWorkerJni.get().destroyV1LevelDb();
        }

        public void MigrateFromSyncV1() {
            // Do all migration work in file IO thread because we may need to
            // read shared preferences and delete level db
            PostTask.postTask(TaskTraits.BEST_EFFORT_MAY_BLOCK, () -> {
                if (HaveSyncV1Prefs()) {
                    Log.i(TAG, "Found sync v1 data, doing migration");
                    DeleteSyncV1Prefs();
                    DeleteSyncV1LevelDb();
                    // Mark sync v1 was enabled to trigger informers
                    ThreadUtils.runOnUiThreadBlocking(new Runnable() {
                        @Override
                        public void run() {
                            PresearchSyncWorkerJni.get().markSyncV1WasEnabledAndMigrated();
                            PresearchSyncInformers.show();
                        }
                    });
                }
            });
        }
    };

    public String GetCodephrase() {
        return PresearchSyncWorkerJni.get().getSyncCodeWords(mNativePresearchSyncWorker);
    }

    public void SaveCodephrase(String codephrase) {
        PresearchSyncWorkerJni.get().saveCodeWords(mNativePresearchSyncWorker, codephrase);
    }

    public String GetSeedHexFromWords(String codephrase) {
        return PresearchSyncWorkerJni.get().getSeedHexFromWords(codephrase);
    }

    public String GetWordsFromSeedHex(String seedHex) {
        return PresearchSyncWorkerJni.get().getWordsFromSeedHex(seedHex);
    }

    public void RequestSync() {
        PresearchSyncWorkerJni.get().requestSync(mNativePresearchSyncWorker);
    }

    public boolean IsFirstSetupComplete() {
        return PresearchSyncWorkerJni.get().isFirstSetupComplete(mNativePresearchSyncWorker);
    }

    public void FinalizeSyncSetup() {
        PresearchSyncWorkerJni.get().finalizeSyncSetup(mNativePresearchSyncWorker);
    }

    public void ResetSync() {
        PresearchSyncWorkerJni.get().resetSync(mNativePresearchSyncWorker);
    }

    public boolean getSyncV1WasEnabled() {
        return PresearchSyncWorkerJni.get().getSyncV1WasEnabled(mNativePresearchSyncWorker);
    }

    public boolean getSyncV2MigrateNoticeDismissed() {
        return PresearchSyncWorkerJni.get().getSyncV2MigrateNoticeDismissed(mNativePresearchSyncWorker);
    }

    public void setSyncV2MigrateNoticeDismissed(boolean isDismissed) {
        PresearchSyncWorkerJni.get().setSyncV2MigrateNoticeDismissed(
                mNativePresearchSyncWorker, isDismissed);
    }

    @NativeMethods
    interface Natives {
        void init(PresearchSyncWorker caller);
        void destroy(long nativePresearchSyncWorker);

        void destroyV1LevelDb();
        void markSyncV1WasEnabledAndMigrated();

        String getSyncCodeWords(long nativePresearchSyncWorker);
        void requestSync(long nativePresearchSyncWorker);

        String getSeedHexFromWords(String passphrase);
        String getWordsFromSeedHex(String seedHex);
        void saveCodeWords(long nativePresearchSyncWorker, String passphrase);

        void finalizeSyncSetup(long nativePresearchSyncWorker);

        boolean isFirstSetupComplete(long nativePresearchSyncWorker);

        void resetSync(long nativePresearchSyncWorker);

        boolean getSyncV1WasEnabled(long nativePresearchSyncWorker);
        boolean getSyncV2MigrateNoticeDismissed(long nativePresearchSyncWorker);
        void setSyncV2MigrateNoticeDismissed(long nativePresearchSyncWorker, boolean isDismissed);
    }
}
