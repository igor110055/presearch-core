/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchTabSwitcherModeTTPhoneClassAdapter extends PresearchClassVisitor {
    static String sTabSwitcherModeTTPhoneClassName =
            "org/chromium/chrome/browser/toolbar/top/TabSwitcherModeTTPhone";
    static String sPresearchTabSwitcherModeTTPhoneClassName =
            "org/chromium/chrome/browser/toolbar/top/PresearchTabSwitcherModeTTPhone";

    public PresearchTabSwitcherModeTTPhoneClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchTabSwitcherModeTTPhoneClassName, "mNewTabViewButton");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mNewTabViewButton");

        deleteField(sPresearchTabSwitcherModeTTPhoneClassName, "mNewTabImageButton");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mNewTabImageButton");

        deleteField(sPresearchTabSwitcherModeTTPhoneClassName, "mToggleTabStackButton");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mToggleTabStackButton");

        deleteField(sPresearchTabSwitcherModeTTPhoneClassName, "mShouldShowNewTabVariation");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mShouldShowNewTabVariation");

        makePublicMethod(sTabSwitcherModeTTPhoneClassName, "updateNewTabButtonVisibility");
        addMethodAnnotation(sPresearchTabSwitcherModeTTPhoneClassName, "updateNewTabButtonVisibility",
                "Ljava/lang/Override;");

        makePublicMethod(sTabSwitcherModeTTPhoneClassName, "shouldShowIncognitoToggle");
        deleteMethod(sPresearchTabSwitcherModeTTPhoneClassName, "shouldShowIncognitoToggle");
    }
}
