/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.site_settings;

import android.os.Bundle;
import androidx.preference.Preference;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.preferences.PresearchPrefServiceBridge;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.chrome.browser.settings.PresearchPreferenceFragment;

public class DesktopModePreferences
        extends PresearchPreferenceFragment implements Preference.OnPreferenceChangeListener {
    public static final String DESKTOP_MODE_KEY = "desktop_mode";

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.settings_desktop_mode_title);
        SettingsUtils.addPreferencesFromResource(this, R.xml.desktop_mode_preferences);

        ChromeSwitchPreference desktopModePref =
                (ChromeSwitchPreference) findPreference(DESKTOP_MODE_KEY);
        desktopModePref.setChecked(false);
        desktopModePref.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        PresearchPrefServiceBridge.getInstance().setDesktopModeEnabled((boolean) newValue);
        return true;
    }
}
