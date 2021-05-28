/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Build;
import android.os.Bundle;

import androidx.annotation.Nullable;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.preferences.PresearchPrefServiceBridge;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.ui.UiUtils;

/**
 * Fragment to manage webrtc policy settings.
 */
public class PresearchWebrtcPolicyPreferencesFragment extends PresearchPreferenceFragment {
    static final String PREF_WEBRTC_POLICY = "webrtc_policy";

    @Override
    public void onCreatePreferences(@Nullable Bundle savedInstanceState, String rootKey) {
        SettingsUtils.addPreferencesFromResource(this, R.xml.presearch_webrtc_policy_preferences);
        getActivity().setTitle(R.string.settings_webrtc_policy_label);

        PresearchWebrtcPolicyPreference webrtcPolicyPreference =
                (PresearchWebrtcPolicyPreference) findPreference(PREF_WEBRTC_POLICY);
        webrtcPolicyPreference.initialize(PresearchPrefServiceBridge.getInstance().getWebrtcPolicy());

        webrtcPolicyPreference.setOnPreferenceChangeListener((preference, newValue) -> {
            PresearchPrefServiceBridge.getInstance().setWebrtcPolicy((int) newValue);
            return true;
        });
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        if (Build.VERSION.SDK_INT == Build.VERSION_CODES.O_MR1) {
            UiUtils.setNavigationBarIconColor(getActivity().getWindow().getDecorView(),
                    getResources().getBoolean(R.bool.window_light_navigation_bar));
        }

        setDivider(null);
    }
}
