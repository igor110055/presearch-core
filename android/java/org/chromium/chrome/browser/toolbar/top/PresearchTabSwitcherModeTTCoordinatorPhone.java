/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.top;

import android.view.ViewStub;

import org.chromium.chrome.browser.toolbar.menu_button.MenuButtonCoordinator;

class PresearchTabSwitcherModeTTCoordinatorPhone extends TabSwitcherModeTTCoordinatorPhone {
    private TabSwitcherModeTTPhone mTabSwitcherModeToolbar;

    private boolean mIsBottomToolbarVisible;
    private MenuButtonCoordinator mPresearchMenuButtonCoordinator;

    PresearchTabSwitcherModeTTCoordinatorPhone(ViewStub tabSwitcherToolbarStub,
            MenuButtonCoordinator menuButtonCoordinator, boolean isGridTabSwitcherEnabled,
            boolean isTabToGtsAnimationEnabled, boolean isStartSurfaceEnabled) {
        super(tabSwitcherToolbarStub, menuButtonCoordinator, isGridTabSwitcherEnabled,
                isTabToGtsAnimationEnabled, isStartSurfaceEnabled);
        mPresearchMenuButtonCoordinator = menuButtonCoordinator;
    }

    @Override
    public void setTabSwitcherMode(boolean inTabSwitcherMode) {
        super.setTabSwitcherMode(inTabSwitcherMode);
        if (inTabSwitcherMode && (mTabSwitcherModeToolbar instanceof PresearchTabSwitcherModeTTPhone)) {
            ((PresearchTabSwitcherModeTTPhone) mTabSwitcherModeToolbar)
                    .onBottomToolbarVisibilityChanged(mIsBottomToolbarVisible);
        }
        if (mPresearchMenuButtonCoordinator != null && mIsBottomToolbarVisible) {
            mPresearchMenuButtonCoordinator.setVisibility(!inTabSwitcherMode);
        }
    }

    void onBottomToolbarVisibilityChanged(boolean isVisible) {
        if (mIsBottomToolbarVisible == isVisible) {
            return;
        }
        mIsBottomToolbarVisible = isVisible;
        if (mTabSwitcherModeToolbar instanceof PresearchTabSwitcherModeTTPhone) {
            ((PresearchTabSwitcherModeTTPhone) mTabSwitcherModeToolbar)
                    .onBottomToolbarVisibilityChanged(isVisible);
        }
    }
}
