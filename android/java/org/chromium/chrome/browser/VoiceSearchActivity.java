/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import org.chromium.chrome.browser.IntentHandler;

import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.searchwidget.SearchWidgetProvider;

public class TextSearchActivity extends Activity {
  static final String ACTION_START_VOICE_QUERY = "org.chromium.chrome.browser.searchwidget.START_VOICE_QUERY";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Intent intent = new Intent();
        // intent.addFlags(Intent.FLAG_INCLUDE_STOPPED_PACKAGES);
        // intent.setAction(ACTION_START_TEXT_QUERY);
        // SearchWidgetProvider.startSearchActivity(intent, false);
        // finish();

        Intent intent = new Intent(ACTION_START_VOICE_QUERY);
        intent.setClass(context, SearchWidgetProvider.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        IntentHandler.addTrustedIntentExtras(intent);
        return intent;
    }
}
