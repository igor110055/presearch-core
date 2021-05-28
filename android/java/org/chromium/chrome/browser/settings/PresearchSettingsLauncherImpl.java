/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import org.chromium.base.IntentUtils;
import org.chromium.chrome.browser.safe_browsing.settings.PresearchStandardProtectionSettingsFragment;
import org.chromium.chrome.browser.safe_browsing.settings.StandardProtectionSettingsFragment;

public class PresearchSettingsLauncherImpl extends SettingsLauncherImpl {
    public PresearchSettingsLauncherImpl() {
        super();
    }

    @Override
    public void launchSettingsActivity(Context context,
            @Nullable Class<? extends Fragment> fragment, @Nullable Bundle fragmentArgs) {
        if (fragment != null) {
            // Substitute with our version of class
            if (fragment.equals(StandardProtectionSettingsFragment.class)) {
                fragment = PresearchStandardProtectionSettingsFragment.class;
            }
        }
        super.launchSettingsActivity(context, fragment, fragmentArgs);
    }
}
