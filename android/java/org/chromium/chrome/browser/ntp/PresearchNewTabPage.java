/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp;

import android.app.Activity;

import androidx.annotation.Nullable;

import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.browser_controls.BrowserControlsStateProvider;
import org.chromium.chrome.browser.compositor.layouts.OverviewModeBehavior;
import org.chromium.chrome.browser.feed.PresearchFeedSurfaceCoordinator;
import org.chromium.chrome.browser.feed.shared.FeedFeatures;
import org.chromium.chrome.browser.feed.shared.FeedSurfaceProvider;
import org.chromium.chrome.browser.lifecycle.ActivityLifecycleDispatcher;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.share.ShareDelegate;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.chrome.browser.ui.native_page.NativePageHost;
import org.chromium.components.browser_ui.bottomsheet.BottomSheetController;
import org.chromium.ui.base.WindowAndroid;

public class PresearchNewTabPage extends NewTabPage {
    private NewTabPageLayout mNewTabPageLayout;
    private FeedSurfaceProvider mFeedSurfaceProvider;

    public PresearchNewTabPage(Activity activity,
            BrowserControlsStateProvider browserControlsStateProvider,
            Supplier<Tab> activityTabProvider, SnackbarManager snackbarManager,
            ActivityLifecycleDispatcher lifecycleDispatcher, TabModelSelector tabModelSelector,
            boolean isTablet, NewTabPageUma uma, boolean isInNightMode,
            NativePageHost nativePageHost, Tab tab, BottomSheetController bottomSheetController,
            ObservableSupplier<ShareDelegate> shareDelegateSupplier, WindowAndroid windowAndroid) {
        super(activity, browserControlsStateProvider, activityTabProvider, snackbarManager,
                lifecycleDispatcher, tabModelSelector, isTablet, uma, isInNightMode, nativePageHost,
                tab, bottomSheetController, shareDelegateSupplier, windowAndroid);

        assert mNewTabPageLayout instanceof PresearchNewTabPageLayout;
        if (mNewTabPageLayout instanceof PresearchNewTabPageLayout) {
            ((PresearchNewTabPageLayout) mNewTabPageLayout).setTab(tab);
        }
    }

    @Override
    protected void initializeMainView(Activity activity, WindowAndroid windowAndroid,
            SnackbarManager snackbarManager, TabModelSelector tabModelSelector, NewTabPageUma uma,
            boolean isInNightMode, BottomSheetController bottomSheetController,
            ObservableSupplier<ShareDelegate> shareDelegateSupplier) {
        super.initializeMainView(activity, windowAndroid, snackbarManager, tabModelSelector, uma,
                isInNightMode, bottomSheetController, shareDelegateSupplier);
        // Override surface provider
        Profile profile = Profile.fromWebContents(mTab.getWebContents());

        assert !FeedFeatures.isFeedEnabled();
        mFeedSurfaceProvider = new PresearchFeedSurfaceCoordinator(activity, snackbarManager,
                tabModelSelector, windowAndroid,
                new SnapScrollHelper(mNewTabPageManager, mNewTabPageLayout), mNewTabPageLayout,
                null, null, isInNightMode, this, mNewTabPageManager.getNavigationDelegate(),
                profile, false, bottomSheetController, shareDelegateSupplier, null);
    }
}
