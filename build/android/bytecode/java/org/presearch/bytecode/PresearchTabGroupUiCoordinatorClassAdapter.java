/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchTabGroupUiCoordinatorClassAdapter extends PresearchClassVisitor {
    static String sTabGroupUiCoordinatorClassName =
            "org/chromium/chrome/browser/tasks/tab_management/TabGroupUiCoordinator";
    static String sPresearchTabGroupUiCoordinatorClassName =
            "org/chromium/chrome/browser/tasks/tab_management/PresearchTabGroupUiCoordinator";

    public PresearchTabGroupUiCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sTabGroupUiCoordinatorClassName, sPresearchTabGroupUiCoordinatorClassName);

        deleteField(sPresearchTabGroupUiCoordinatorClassName, "mToolbarView");
        makeProtectedField(sTabGroupUiCoordinatorClassName, "mToolbarView");
    }
}
