/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import androidx.preference.DialogPreference;
import android.util.AttributeSet;

import org.chromium.chrome.R;

/**
 * The preference used to reset Presearch Rewards.
 */
public class PresearchRewardsResetPreference extends DialogPreference {
    /**
     * Constructor for PresearchRewardsResetPreference.
     */
    public PresearchRewardsResetPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        setDialogLayoutResource(R.layout.presearch_rewards_reset_tab_content);
    }
}
