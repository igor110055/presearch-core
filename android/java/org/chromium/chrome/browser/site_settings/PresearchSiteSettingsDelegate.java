/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.site_settings;

import android.content.Context;
import android.content.Intent;

import org.chromium.chrome.browser.site_settings.ChromeSiteSettingsDelegate;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.components.embedder_support.browser_context.BrowserContextHandle;


public class PresearchSiteSettingsDelegate extends ChromeSiteSettingsDelegate {
	public PresearchSiteSettingsDelegate(Context context, BrowserContextHandle browserContextHandle) {
		super(context, browserContextHandle);
	}

    @Override
    public void closeButton() {
        Intent intent = new Intent(PresearchActivity.getChromeTabbedActivity(), ChromeTabbedActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
        PresearchActivity.getChromeTabbedActivity().startActivity(intent);
    }
}
