/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchToolbarLayoutClassAdapter extends PresearchClassVisitor {
    static String sCustomTabToolbarClassName =
            "org/chromium/chrome/browser/customtabs/features/toolbar/CustomTabToolbar";
    static String sToolbarPhoneClassName = "org/chromium/chrome/browser/toolbar/top/ToolbarPhone";
    static String sPresearchToolbarLayoutClassName =
            "org/chromium/chrome/browser/toolbar/top/PresearchToolbarLayout";

    public PresearchToolbarLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sCustomTabToolbarClassName, sPresearchToolbarLayoutClassName);

        changeSuperName(sToolbarPhoneClassName, sPresearchToolbarLayoutClassName);
    }
}
