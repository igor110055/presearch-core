/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchTopToolbarCoordinatorClassAdapter extends PresearchClassVisitor {
    static String sTopToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/top/TopToolbarCoordinator";
    static String sPresearchTopToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/top/PresearchTopToolbarCoordinator";

    public PresearchTopToolbarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchTopToolbarCoordinatorClassName, "mTabSwitcherModeCoordinatorPhone");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mTabSwitcherModeCoordinatorPhone");

        deleteField(sPresearchTopToolbarCoordinatorClassName, "mOptionalButtonController");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mOptionalButtonController");
    }
}
