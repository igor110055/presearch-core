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

import org.chromium.chrome.browser.searchwidget.SearchActivity;

import org.chromium.base.ThreadUtils;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.IntentHandler;
import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;

import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.searchwidget.SearchWidgetProvider;

public class VoiceSearchActivity extends Activity {
  static final String ACTION_START_VOICE_QUERY = "org.chromium.chrome.browser.searchwidget.START_VOICE_QUERY";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Context context = ContextUtils.getApplicationContext();
        Intent intent = new Intent();
        intent.setAction(ACTION_START_VOICE_QUERY);

        Thread thread = new Thread() {
            @Override
            public void run() {
                startSearchActivity(context, intent, true);
            }
        };
        thread.start();
        finish();
    }

    public static void startSearchActivity(Context context, Intent intent, boolean startVoiceSearch) {

        // Launch the SearchActivity.
        Intent searchIntent = new Intent();
        searchIntent.setClass(context, SearchActivity.class);
        searchIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        searchIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_DOCUMENT);
        searchIntent.putExtra(SearchWidgetProvider.EXTRA_START_VOICE_SEARCH, startVoiceSearch);
        searchIntent.putExtra(SearchWidgetProvider.EXTRA_FROM_SEARCH_WIDGET, true);

        Bundle optionsBundle =
                ActivityOptionsCompat.makeCustomAnimation(context, R.anim.activity_open_enter, 0)
                        .toBundle();
        IntentUtils.safeStartActivity(context, searchIntent, optionsBundle);
    }
}
