/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchBottomControlsMediatorClassAdapter extends PresearchClassVisitor {
    static String sBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/BottomControlsMediator";
    static String sPresearchBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/PresearchBottomControlsMediator";

    public PresearchBottomControlsMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBottomControlsMediatorClassName, sPresearchBottomControlsMediatorClassName);

        deleteField(sPresearchBottomControlsMediatorClassName, "mBottomControlsHeight");
        makeProtectedField(sBottomControlsMediatorClassName, "mBottomControlsHeight");

        deleteField(sPresearchBottomControlsMediatorClassName, "mModel");
        makeProtectedField(sBottomControlsMediatorClassName, "mModel");

        deleteField(sPresearchBottomControlsMediatorClassName, "mBrowserControlsSizer");
        makeProtectedField(sBottomControlsMediatorClassName, "mBrowserControlsSizer");
    }
}
