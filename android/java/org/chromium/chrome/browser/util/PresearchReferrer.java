/**
 * Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.util;

import android.content.Context;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.RemoteException;
import com.android.installreferrer.api.InstallReferrerClient;
import com.android.installreferrer.api.InstallReferrerClient.InstallReferrerResponse;
import com.android.installreferrer.api.InstallReferrerStateListener;
import com.android.installreferrer.api.ReferrerDetails;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;

public class PresearchReferrer implements InstallReferrerStateListener {
    private static final String TAG = "PresearchReferrer";
    private static final String APP_CHROME_DIR = "app_chrome";
    private static final String PROMO_CODE_FILE_NAME = "promoCode";
    private static final String PRESEARCH_REFERRER_RECEIVED = "presearch_referrer_received";

    private String promoCodeFilePath;
    private InstallReferrerClient referrerClient;

    private static PresearchReferrer sInstance;

    private PresearchReferrer() {}

    public static PresearchReferrer getInstance() {
        if (sInstance != null) return sInstance;
        sInstance = new PresearchReferrer();
        return sInstance;
    }

    private class InitReferrerRunnable implements Runnable {
        private Context mContext;
        private PresearchReferrer mPresearchReferrer;
        public InitReferrerRunnable(Context context, PresearchReferrer presearchReferrer) {
          mContext = context;
          mPresearchReferrer = presearchReferrer;
        }

        @Override
        public void run() {
            promoCodeFilePath = mContext.getApplicationInfo().dataDir +
                    File.separator + APP_CHROME_DIR + File.separator + PROMO_CODE_FILE_NAME;
            SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
            if (!sharedPref.getBoolean(PRESEARCH_REFERRER_RECEIVED, false) &&
                PackageUtils.isFirstInstall(mContext)) {
                referrerClient = InstallReferrerClient.newBuilder(mContext).build();
                // This seems to be known issue, for now just wrapping it into try/catch block
                // https://issuetracker.google.com/issues/72926755
                try {
                    referrerClient.startConnection(mPresearchReferrer);
                } catch (SecurityException e) {
                    Log.e(TAG, "Unable to start connection for referrer client: " + e);
                }
            }
        }
    }

    public void initReferrer(Context context) {
        // On some devices InstallReferrerClient.startConnection causes file IO,
        // so run it in IO task
        PostTask.postTask(TaskTraits.BEST_EFFORT_MAY_BLOCK,
            new InitReferrerRunnable(context, this));
    }

    private class SaveReferrerRunnable implements Runnable {
      private String mUrpc;
      public SaveReferrerRunnable(String urpc) {
         mUrpc = urpc;
      }

      @Override
      public void run() {
        FileOutputStream outputStreamWriter = null;
        try {
            File promoCodeFile = new File(promoCodeFilePath);
            outputStreamWriter = new FileOutputStream(promoCodeFile);
            outputStreamWriter.write(mUrpc.getBytes());
        } catch (IOException e) {
            Log.e(TAG, "Could not write to file (" + promoCodeFilePath + "): " + e.getMessage());
        } finally {
            try {
              if (outputStreamWriter != null) outputStreamWriter.close();
            } catch (IOException exception) {}
        }
      }
    }

    @Override
    public void onInstallReferrerSetupFinished(int responseCode) {
        switch (responseCode) {
            case InstallReferrerResponse.OK:
                try {
                    ReferrerDetails response = referrerClient.getInstallReferrer();
                    String referrer = response.getInstallReferrer();
                    Uri uri = Uri.parse("http://www.stub.co/?"+referrer);
                    // Get and save user referal program code
                    String urpc = uri.getQueryParameter("urpc");
                    if (urpc != null && !urpc.isEmpty()) {
                        PostTask.postTask(TaskTraits.BEST_EFFORT_MAY_BLOCK,
                            new SaveReferrerRunnable(urpc));
                    }
                    referrerClient.endConnection();
                    // Set flag to not repeat this procedure
                    SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
                    SharedPreferences.Editor editor = sharedPref.edit();
                    editor.putBoolean(PRESEARCH_REFERRER_RECEIVED, true);
                    editor.apply();
                } catch (RemoteException e) {
                    Log.e(TAG, "Could not get referral: " + e.getMessage());
                }
                break;
            case InstallReferrerResponse.FEATURE_NOT_SUPPORTED:
                Log.e(TAG, "API not available on the current Play Store app");
                break;
            case InstallReferrerResponse.SERVICE_UNAVAILABLE:
                Log.e(TAG, "Connection couldn't be established");
                break;
        }
    }

    @Override
    public void onInstallReferrerServiceDisconnected() {
        Log.e(TAG, "Install referrer service was disconnected");
    }
}
