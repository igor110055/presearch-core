/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchFeedSurfaceCoordinatorClassAdapter extends PresearchClassVisitor {
    static String sFeedSurfaceCoordinatorClassName =
            "org/chromium/chrome/browser/feed/FeedSurfaceCoordinator";
    static String sPresearchFeedSurfaceCoordinatorClassName =
            "org/chromium/chrome/browser/feed/PresearchFeedSurfaceCoordinator";

    public PresearchFeedSurfaceCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchFeedSurfaceCoordinatorClassName, "mActivity");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mActivity");

        deleteField(sPresearchFeedSurfaceCoordinatorClassName, "mScrollViewForPolicy");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mScrollViewForPolicy");

        deleteField(sPresearchFeedSurfaceCoordinatorClassName, "mNtpHeader");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mNtpHeader");

        deleteField(sPresearchFeedSurfaceCoordinatorClassName, "mRootView");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mRootView");
    }
}
