/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.tabmodel;

import android.os.Build;

import org.chromium.base.PresearchReflectionUtil;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.app.ChromeActivity;
import org.chromium.chrome.browser.init.StartupTabPreloader;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.ntp_background_images.util.SponsoredImageUtil;
import org.chromium.chrome.browser.preferences.PresearchPref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabDelegateFactory;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.base.WindowAndroid;

public class PresearchTabCreator extends ChromeTabCreator {

    public PresearchTabCreator(ChromeActivity activity, WindowAndroid nativeWindow,
            StartupTabPreloader startupTabPreloader,
            Supplier<TabDelegateFactory> tabDelegateFactory, boolean incognito,
            OverviewNTPCreator overviewNTPCreator, AsyncTabParamsManager asyncTabParamsManager) {
        super(activity, nativeWindow, startupTabPreloader, tabDelegateFactory, incognito, overviewNTPCreator, asyncTabParamsManager);
    }

    @Override
    public Tab launchUrl(String url, @TabLaunchType int type) {
        if (url.equals(UrlConstants.NTP_URL) && type == TabLaunchType.FROM_CHROME_UI) {
            registerPageView();
            ChromeTabbedActivity chromeTabbedActivity = PresearchActivity.getChromeTabbedActivity();
            if(chromeTabbedActivity != null && Build.VERSION.SDK_INT <= Build.VERSION_CODES.M) {
                TabModel tabModel = chromeTabbedActivity.getCurrentTabModel();
                if (tabModel.getCount() >= SponsoredImageUtil.MAX_TABS && UserPrefs.get(Profile.getLastUsedRegularProfile()).getBoolean(PresearchPref.NEW_TAB_PAGE_SHOW_BACKGROUND_IMAGE)) {
                    Tab tab = PresearchActivity.class.cast(chromeTabbedActivity)
                                      .selectExistingTab(UrlConstants.NTP_URL);
                    if (tab != null) {
                        PresearchReflectionUtil.InvokeMethod(
                                ChromeTabbedActivity.class, chromeTabbedActivity, "hideOverview");
                        return tab;
                    }
                }
            }
        }
        return super.launchUrl(url, type);
    }

    private void registerPageView() {
        NTPBackgroundImagesBridge.getInstance(Profile.getLastUsedRegularProfile()).registerPageView();
    }
}
