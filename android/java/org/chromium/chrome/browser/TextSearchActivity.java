/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

 package org.chromium.chrome.browser;

 import android.app.Activity;
 import android.content.Context;
 import android.content.Intent;

 public class TextSearchActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        static final String ACTION_START_TEXT_QUERY =
        "org.chromium.chrome.browser.searchwidget.START_TEXT_QUERY";

        Intent intent = new Intent();
        intent.addFlags(Intent.FLAG_INCLUDE_STOPPED_PACKAGES);
        intent.setAction(ACTION_START_TEXT_QUERY);
        sendBroadcast(intent);
        finish();
    }
}
