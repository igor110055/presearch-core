/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings.themes;

import android.content.Context;
import androidx.preference.PreferenceViewHolder;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RadioGroup;

import org.chromium.chrome.R;
import org.chromium.components.browser_ui.widget.RadioButtonWithDescription;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.preferences.PresearchPref;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.chrome.browser.night_mode.settings.RadioButtonGroupThemePreference;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.PresearchRelaunchUtils;

public class PresearchRadioButtonGroupCustomHomepageThemePreference extends RadioButtonGroupThemePreference {

    private RadioButtonWithDescription presearchDefaultView;
    private RadioButtonWithDescription refView;
    private NTPBackgroundImagesBridge mNTPBackgroundImagesBridge;

    public PresearchRadioButtonGroupCustomHomepageThemePreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        Profile mProfile = Profile.getLastUsedRegularProfile();
        mNTPBackgroundImagesBridge = NTPBackgroundImagesBridge.getInstance(mProfile);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        holder.findViewById(R.id.system_default).setVisibility(View.GONE);

        RadioButtonWithDescription presearchDefaultView = (RadioButtonWithDescription)holder.findViewById(R.id.light);
        RadioButtonWithDescription refView = (RadioButtonWithDescription)holder.findViewById(R.id.dark);
        if (mNTPBackgroundImagesBridge != null && mNTPBackgroundImagesBridge.isSuperReferral()) {
            refView.setPrimaryText(mNTPBackgroundImagesBridge.getSuperReferralThemeName());
            if(UserPrefs.get(Profile.getLastUsedRegularProfile()).getInteger(PresearchPref.NEW_TAB_PAGE_SUPER_REFERRAL_THEMES_OPTION) == 1 ? true : false) {
                refView.setChecked(true);
                presearchDefaultView.setChecked(false);
            } else {
                refView.setChecked(false);
                presearchDefaultView.setChecked(true);
            }
        }
        presearchDefaultView.setPrimaryText(getContext().getResources().getString(R.string.presearch_default));
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        super.onCheckedChanged(group, checkedId);
        UserPrefs.get(Profile.getLastUsedRegularProfile()).setInteger(PresearchPref.NEW_TAB_PAGE_SUPER_REFERRAL_THEMES_OPTION, checkedId == R.id.light ? 0 : 1 );
        PresearchRelaunchUtils.askForRelaunch(getContext());
    }
}
