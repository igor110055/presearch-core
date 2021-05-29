/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.chrome.browser.settings.MainSettings.PREF_UI_THEME;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.PresearchFeatureList;
import org.chromium.chrome.browser.PresearchRelaunchUtils;
import org.chromium.chrome.browser.PresearchRewardsNativeWorker;
import org.chromium.chrome.browser.PresearchRewardsObserver;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.night_mode.NightModeUtils;
import org.chromium.chrome.browser.preferences.PresearchPreferenceKeys;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.settings.PresearchPreferenceFragment;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.ui.base.DeviceFormFactor;

public class AppearancePreferences extends PresearchPreferenceFragment
        implements Preference.OnPreferenceChangeListener, PresearchRewardsObserver {
    public static final String PREF_HIDE_PRESEARCH_REWARDS_ICON = "hide_presearch_rewards_icon";
    public static final String PREF_PRESEARCH_NIGHT_MODE_ENABLED = "presearch_night_mode_enabled_key";
    public static final String PREF_PRESEARCH_ENABLE_TAB_GROUPS = "presearch_enable_tab_groups";

    private PresearchRewardsNativeWorker mPresearchRewardsNativeWorker;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.prefs_appearance);
        SettingsUtils.addPreferencesFromResource(this, R.xml.appearance_preferences);
        boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                ContextUtils.getApplicationContext());
        if (isTablet) {
            removePreferenceIfPresent(PresearchPreferenceKeys.PRESEARCH_BOTTOM_TOOLBAR_ENABLED_KEY);
            removePreferenceIfPresent(PREF_PRESEARCH_ENABLE_TAB_GROUPS);
        }

        if (!NightModeUtils.isNightModeSupported()) {
            removePreferenceIfPresent(PREF_UI_THEME);
        }

        if (!ChromeFeatureList.isEnabled(PresearchFeatureList.PRESEARCH_REWARDS)) {
            removePreferenceIfPresent(PREF_HIDE_PRESEARCH_REWARDS_ICON);
        }
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {}

    private void removePreferenceIfPresent(String key) {
        Preference preference = getPreferenceScreen().findPreference(key);
        if (preference != null) getPreferenceScreen().removePreference(preference);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        // ChromeSwitchPreference hidePresearchRewardsIconPref =
        //         (ChromeSwitchPreference) findPreference(PREF_HIDE_PRESEARCH_REWARDS_ICON);
        // if (hidePresearchRewardsIconPref != null) {
        //     SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        //     hidePresearchRewardsIconPref.setChecked(
        //             sharedPreferences.getBoolean(PREF_HIDE_PRESEARCH_REWARDS_ICON, false));
        //     hidePresearchRewardsIconPref.setOnPreferenceChangeListener(this);
        // }

        Preference nightModeEnabled = findPreference(PREF_PRESEARCH_NIGHT_MODE_ENABLED);
        nightModeEnabled.setOnPreferenceChangeListener(this);
        if (nightModeEnabled instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) nightModeEnabled)
                    .setChecked(ChromeFeatureList.isEnabled(
                            PresearchFeatureList.FORCE_WEB_CONTENTS_DARK_MODE));
        }

        Preference enableBottomToolbar =
                findPreference(PresearchPreferenceKeys.PRESEARCH_BOTTOM_TOOLBAR_ENABLED_KEY);
        if (enableBottomToolbar == null) return;

        enableBottomToolbar.setOnPreferenceChangeListener(this);
        if (enableBottomToolbar instanceof ChromeSwitchPreference) {
            boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                    ContextUtils.getApplicationContext());
            ((ChromeSwitchPreference) enableBottomToolbar)
                    .setChecked(!isTablet && BottomToolbarConfiguration.isBottomToolbarEnabled());
        }

        Preference enableTabGroups = findPreference(PREF_PRESEARCH_ENABLE_TAB_GROUPS);
        enableTabGroups.setOnPreferenceChangeListener(this);
        if (enableTabGroups instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) enableTabGroups)
                    .setChecked(ChromeFeatureList.isEnabled(ChromeFeatureList.TAB_GROUPS_ANDROID));
        }
    }

    @Override
    public void onStart() {
        mPresearchRewardsNativeWorker = PresearchRewardsNativeWorker.getInstance();
        if (mPresearchRewardsNativeWorker != null) {
            mPresearchRewardsNativeWorker.AddObserver(this);
        }
        super.onStart();
    }

    @Override
    public void onStop() {
        if (mPresearchRewardsNativeWorker != null) {
            mPresearchRewardsNativeWorker.RemoveObserver(this);
        }
        super.onStop();
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        if (PresearchPreferenceKeys.PRESEARCH_BOTTOM_TOOLBAR_ENABLED_KEY.equals(key)) {
            SharedPreferences prefs = ContextUtils.getAppSharedPreferences();
            Boolean originalStatus = BottomToolbarConfiguration.isBottomToolbarEnabled();
            prefs.edit()
                    .putBoolean(
                            PresearchPreferenceKeys.PRESEARCH_BOTTOM_TOOLBAR_ENABLED_KEY, !originalStatus)
                    .apply();
            PresearchRelaunchUtils.askForRelaunch(getActivity());
        } else if (PREF_HIDE_PRESEARCH_REWARDS_ICON.equals(key)) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(PREF_HIDE_PRESEARCH_REWARDS_ICON, !(boolean) newValue);
            sharedPreferencesEditor.apply();
            PresearchRelaunchUtils.askForRelaunch(getActivity());
        } else if (PREF_PRESEARCH_NIGHT_MODE_ENABLED.equals(key)) {
            PresearchFeatureList.enableFeature(
                    PresearchFeatureList.ENABLE_FORCE_DARK, (boolean) newValue, true);
            PresearchRelaunchUtils.askForRelaunch(getActivity());
        } else if (PREF_PRESEARCH_ENABLE_TAB_GROUPS.equals(key)) {
            PresearchFeatureList.enableFeature(
                    PresearchFeatureList.ENABLE_TAB_GROUPS, (boolean) newValue, false);
            PresearchFeatureList.enableFeature(
                    PresearchFeatureList.ENABLE_TAB_GRID, (boolean) newValue, false);
            SharedPreferencesManager.getInstance().writeBoolean(
                    PresearchPreferenceKeys.PRESEARCH_DOUBLE_RESTART, true);
            PresearchRelaunchUtils.askForRelaunch(getActivity());
        }

        return true;
    }
}
