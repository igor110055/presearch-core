/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchHomepageManagerClassAdapter extends PresearchClassVisitor {
    static String sHomepageManagerClassName = "org/chromium/chrome/browser/homepage/HomepageManager";
    static String sPresearchHomepageManagerClassName = "org/chromium/chrome/browser/homepage/PresearchHomepageManager";

    public PresearchHomepageManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sHomepageManagerClassName, "shouldCloseAppWithZeroTabs", sPresearchHomepageManagerClassName);
    }
}
