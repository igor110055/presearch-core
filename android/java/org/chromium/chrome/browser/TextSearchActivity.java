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
import org.chromium.chrome.browser.IntentHandler;
import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;

import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.searchwidget.SearchWidgetProvider;

public class TextSearchActivity extends Activity {
  static final String ACTION_START_TEXT_QUERY = "org.chromium.chrome.browser.searchwidget.START_TEXT_QUERY";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Context context = ContextUtils.getApplicationContext();

        Intent textIntent = new Intent(ACTION_START_TEXT_QUERY);
        textIntent.setClass(context, SearchWidgetProvider.class);
        textIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        IntentHandler.addTrustedIntentExtras(textIntent);

        PendingIntent.getBroadcast(context, 0, textIntent,
                        PendingIntent.FLAG_UPDATE_CURRENT
                                | IntentUtils.getPendingIntentMutabilityFlag(false));
        finish();
    }
}
