/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchBottomControlsCoordinatorClassAdapter extends PresearchClassVisitor {
    static String sBottomControlsCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/BottomControlsCoordinator";
    static String sPresearchBottomControlsCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/PresearchBottomControlsCoordinator";

    public PresearchBottomControlsCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchBottomControlsCoordinatorClassName, "mMediator");
        makeProtectedField(sBottomControlsCoordinatorClassName, "mMediator");
    }
}
