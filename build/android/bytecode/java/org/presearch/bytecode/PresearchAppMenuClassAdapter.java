/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchAppMenuClassAdapter extends PresearchClassVisitor {
    static String sAppMenuClassName = "org/chromium/chrome/browser/ui/appmenu/AppMenu";

    static String sPresearchAppMenuClassName = "org/chromium/chrome/browser/ui/appmenu/PresearchAppMenu";

    public PresearchAppMenuClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sAppMenuClassName, "getPopupPosition", sPresearchAppMenuClassName);
    }
}
