/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.os.Bundle;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import androidx.core.app.ActivityOptionsCompat;

import org.chromium.base.ThreadUtils;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.IntentHandler;
import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;

import org.org.chromium.chrome.browser.VoiceSearchActivity;

import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.searchwidget.SearchWidgetProvider;

public class TextSearchActivity extends Activity {
  static final String ACTION_START_TEXT_QUERY = "org.chromium.chrome.browser.searchwidget.START_TEXT_QUERY";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Context context = ContextUtils.getApplicationContext();
        Intent intent = new Intent();
        intent.setAction(ACTION_START_TEXT_QUERY);

        Thread thread = new Thread() {
            @Override
            public void run() {
                VoiceSearchActivity.startSearchActivity(context, intent, false);
            }
        };
        thread.start();
    }
}
