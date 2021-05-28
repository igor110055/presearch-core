/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchIncognitoToggleTabLayoutClassAdapter extends PresearchClassVisitor {
    static String sIncognitoToggleTabLayoutClassName =
            "org/chromium/chrome/browser/toolbar/IncognitoToggleTabLayout";
    static String sPresearchIncognitoToggleTabLayoutClassName =
            "org/chromium/chrome/browser/toolbar/PresearchIncognitoToggleTabLayout";

    public PresearchIncognitoToggleTabLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchIncognitoToggleTabLayoutClassName, "mIncognitoButtonIcon");
        makeProtectedField(sIncognitoToggleTabLayoutClassName, "mIncognitoButtonIcon");
    }
}
