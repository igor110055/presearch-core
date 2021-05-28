/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchToolbarManagerClassAdapter extends PresearchClassVisitor {
    static String sToolbarManagerClassName = "org/chromium/chrome/browser/toolbar/ToolbarManager";
    static String sPresearchToolbarManagerClassName =
            "org/chromium/chrome/browser/toolbar/PresearchToolbarManager";

    public PresearchToolbarManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sToolbarManagerClassName, sPresearchToolbarManagerClassName);

        deleteField(sPresearchToolbarManagerClassName, "mBottomControlsCoordinatorSupplier");
        makeProtectedField(sToolbarManagerClassName, "mBottomControlsCoordinatorSupplier");

        deleteField(sPresearchToolbarManagerClassName, "mCallbackController");
        makeProtectedField(sToolbarManagerClassName, "mCallbackController");

        deleteField(sPresearchToolbarManagerClassName, "mBrowserControlsSizer");
        makeProtectedField(sToolbarManagerClassName, "mBrowserControlsSizer");

        deleteField(sPresearchToolbarManagerClassName, "mFullscreenManager");
        makeProtectedField(sToolbarManagerClassName, "mFullscreenManager");

        deleteField(sPresearchToolbarManagerClassName, "mActivityTabProvider");
        makeProtectedField(sToolbarManagerClassName, "mActivityTabProvider");

        deleteField(sPresearchToolbarManagerClassName, "mAppThemeColorProvider");
        makeProtectedField(sToolbarManagerClassName, "mAppThemeColorProvider");

        deleteField(sPresearchToolbarManagerClassName, "mShareDelegateSupplier");
        makeProtectedField(sToolbarManagerClassName, "mShareDelegateSupplier");

        deleteField(sPresearchToolbarManagerClassName, "mScrimCoordinator");
        makeProtectedField(sToolbarManagerClassName, "mScrimCoordinator");

        deleteField(sPresearchToolbarManagerClassName, "mShowStartSurfaceSupplier");
        makeProtectedField(sToolbarManagerClassName, "mShowStartSurfaceSupplier");

        deleteField(sPresearchToolbarManagerClassName, "mMenuButtonCoordinator");
        makeProtectedField(sToolbarManagerClassName, "mMenuButtonCoordinator");

        deleteField(sPresearchToolbarManagerClassName, "mToolbarTabController");
        makeProtectedField(sToolbarManagerClassName, "mToolbarTabController");

        deleteField(sPresearchToolbarManagerClassName, "mLocationBar");
        makeProtectedField(sToolbarManagerClassName, "mLocationBar");

        deleteField(sPresearchToolbarManagerClassName, "mActionModeController");
        makeProtectedField(sToolbarManagerClassName, "mActionModeController");

        deleteField(sPresearchToolbarManagerClassName, "mLocationBarModel");
        makeProtectedField(sToolbarManagerClassName, "mLocationBarModel");

        deleteField(sPresearchToolbarManagerClassName, "mToolbar");
        makeProtectedField(sToolbarManagerClassName, "mToolbar");

        deleteField(sPresearchToolbarManagerClassName, "mBookmarkBridgeSupplier");
        makeProtectedField(sToolbarManagerClassName, "mBookmarkBridgeSupplier");

        deleteField(sPresearchToolbarManagerClassName, "mLayoutManager");
        makeProtectedField(sToolbarManagerClassName, "mLayoutManager");

        deleteField(sPresearchToolbarManagerClassName, "mOverlayPanelVisibilitySupplier");
        makeProtectedField(sToolbarManagerClassName, "mOverlayPanelVisibilitySupplier");

        deleteField(sPresearchToolbarManagerClassName, "mTabModelSelector");
        makeProtectedField(sToolbarManagerClassName, "mTabModelSelector");

        deleteField(sPresearchToolbarManagerClassName, "mIncognitoStateProvider");
        makeProtectedField(sToolbarManagerClassName, "mIncognitoStateProvider");

        deleteField(sPresearchToolbarManagerClassName, "mTabCountProvider");
        makeProtectedField(sToolbarManagerClassName, "mTabCountProvider");

        deleteField(sPresearchToolbarManagerClassName, "mTabGroupUi");
        makeProtectedField(sToolbarManagerClassName, "mTabGroupUi");

        makePublicMethod(sToolbarManagerClassName, "onOrientationChange");
        addMethodAnnotation(
                sPresearchToolbarManagerClassName, "onOrientationChange", "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateButtonStatus");
        addMethodAnnotation(
                sPresearchToolbarManagerClassName, "updateButtonStatus", "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateBookmarkButtonStatus");
        addMethodAnnotation(sPresearchToolbarManagerClassName, "updateBookmarkButtonStatus",
                "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateReloadState");
        deleteMethod(sPresearchToolbarManagerClassName, "updateReloadState");
    }
}
