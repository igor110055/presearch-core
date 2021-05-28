/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.download;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.preferences.PresearchPreferenceKeys;

public class PresearchMimeUtils {
    public static boolean canAutoOpenMimeType(String mimeType) {
        return ContextUtils.getAppSharedPreferences().getBoolean(
                PresearchPreferenceKeys.PRESEARCH_DOWNLOADS_AUTOMATICALLY_OPEN_WHEN_POSSIBLE, true);
    }
}
