/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchPermissionDialogModelClassAdapter extends PresearchClassVisitor {
    static String sPermissionDialogModelClassName =
            "org/chromium/components/permissions/PermissionDialogModel";

    static String sPresearchPermissionDialogModelClassName =
            "org/chromium/components/permissions/PresearchPermissionDialogModel";

    public PresearchPermissionDialogModelClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(
                sPermissionDialogModelClassName, "getModel", sPresearchPermissionDialogModelClassName);
    }
}
