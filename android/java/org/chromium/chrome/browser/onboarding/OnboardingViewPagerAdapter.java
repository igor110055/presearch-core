/**
 * Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.onboarding;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import android.content.Context;
import androidx.fragment.app.FragmentPagerAdapter;

import org.chromium.chrome.browser.onboarding.OnViewPagerAction;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;

public class OnboardingViewPagerAdapter extends FragmentPagerAdapter {
    private final OnViewPagerAction onViewPagerAction;
    private final Context context;
    private static final int ONBOARDING_WITH_3_PAGES = 3;

    public OnboardingViewPagerAdapter(Context context, FragmentManager fm,
                                      OnViewPagerAction onViewPagerAction) {
        super(fm);
        this.context = context;
        this.onViewPagerAction = onViewPagerAction;
    }

    @Override
    public Fragment getItem(int position) {
        switch (position) {
        case 0:
            PresearchRewardsOnboardingFragment presearchRewardsOnboardingFragment =
                new PresearchRewardsOnboardingFragment();
            presearchRewardsOnboardingFragment.setOnViewPagerAction(onViewPagerAction);
            return presearchRewardsOnboardingFragment;
        case 1:
            PresearchAdsOnboardingFragment presearchAdsOnboardingFragment =
                new PresearchAdsOnboardingFragment();
            presearchAdsOnboardingFragment.setOnViewPagerAction(onViewPagerAction);
            return presearchAdsOnboardingFragment;
        case 2:
            TroubleshootingOnboardingFragment troubleshootingOnboardingFragment =
                new TroubleshootingOnboardingFragment();
            troubleshootingOnboardingFragment.setOnViewPagerAction(onViewPagerAction);
            return troubleshootingOnboardingFragment;
        default:
            return null;
        }
    }

    @Override
    public int getCount() {
        return ONBOARDING_WITH_3_PAGES;
    }
}
