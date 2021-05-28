/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;
import androidx.preference.Preference;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.PresearchRelaunchUtils;
import org.chromium.chrome.browser.preferences.PresearchPrefServiceBridge;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.chrome.browser.settings.PresearchPreferenceFragment;

public class BackgroundVideoPlaybackPreference
        extends PresearchPreferenceFragment implements Preference.OnPreferenceChangeListener {
    private static final String BACKGROUND_VIDEO_PLAYBACK_KEY = "background_video_playback";

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.prefs_background_video_playback);
        SettingsUtils.addPreferencesFromResource(this, R.xml.background_video_playback_preference);

        ChromeSwitchPreference playbackPref =
                (ChromeSwitchPreference) findPreference(BACKGROUND_VIDEO_PLAYBACK_KEY);
        playbackPref.setChecked(PresearchPrefServiceBridge.getInstance().getBackgroundVideoPlaybackEnabled());
        playbackPref.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        PresearchPrefServiceBridge.getInstance().setBackgroundVideoPlaybackEnabled((boolean) newValue);
        PresearchRelaunchUtils.askForRelaunch(this.getActivity());
        return true;
    }

    public static int getPreferenceSummary() {
        return PresearchPrefServiceBridge.getInstance().getBackgroundVideoPlaybackEnabled() ? R.string.text_enabled : R.string.text_disabled;
    }
}
