/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchNewTabPageLayoutClassAdapter extends PresearchClassVisitor {
    static String sNewTabPageLayoutClassName = "org/chromium/chrome/browser/ntp/NewTabPageLayout";
    static String sPresearchNewTabPageLayoutClassName =
            "org/chromium/chrome/browser/ntp/PresearchNewTabPageLayout";
    static String sNewTabPageLayoutSuperClassName = "android/widget/FrameLayout";

    public PresearchNewTabPageLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);
        makePublicMethod(sNewTabPageLayoutClassName, "insertSiteSectionView");
        addMethodAnnotation(sPresearchNewTabPageLayoutClassName, "insertSiteSectionView",
                "Ljava/lang/Override;");

        deleteField(sPresearchNewTabPageLayoutClassName, "mSiteSectionView");
        makeProtectedField(sNewTabPageLayoutClassName, "mSiteSectionView");

        deleteField(sPresearchNewTabPageLayoutClassName, "mTileGroup");
        makeProtectedField(sNewTabPageLayoutClassName, "mTileGroup");

        makePublicMethod(sNewTabPageLayoutClassName, "updateTileGridPlaceholderVisibility");
        addMethodAnnotation(sPresearchNewTabPageLayoutClassName, "updateTileGridPlaceholderVisibility",
                "Ljava/lang/Override;");

        changeSuperName(sNewTabPageLayoutClassName, sNewTabPageLayoutSuperClassName);
    }
}
