/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;
import androidx.preference.Preference;

import org.chromium.chrome.R;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.chrome.browser.settings.PresearchPreferenceFragment;
import org.chromium.chrome.browser.partnercustomizations.ClosePresearchManager;

public class ClosingAllTabsClosesPresearchPreference
        extends PresearchPreferenceFragment implements Preference.OnPreferenceChangeListener {
    private static final String CLOSING_ALL_TABS_CLOSES_PRESEARCH_KEY = "closing_all_tabs_closes_presearch";

    public static int getPreferenceSummary() {
        return ClosePresearchManager.getClosingAllTabsClosesPresearchEnabled() ? R.string.text_on : R.string.text_off;
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.prefs_closing_all_tabs_closes_presearch);
        SettingsUtils.addPreferencesFromResource(this, R.xml.closing_all_tabs_closes_presearch_preference);

        ChromeSwitchPreference pref =
                (ChromeSwitchPreference) findPreference(CLOSING_ALL_TABS_CLOSES_PRESEARCH_KEY);
        pref.setChecked(ClosePresearchManager.getClosingAllTabsClosesPresearchEnabled());
        pref.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        ClosePresearchManager.setClosingAllTabsClosesPresearchEnabled((boolean) newValue);
        return true;
    }
}
