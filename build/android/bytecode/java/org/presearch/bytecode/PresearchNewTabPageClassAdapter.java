/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchNewTabPageClassAdapter extends PresearchClassVisitor {
    static String sNewTabPageClassName = "org/chromium/chrome/browser/ntp/NewTabPage";
    static String sPresearchNewTabPageClassName = "org/chromium/chrome/browser/ntp/PresearchNewTabPage";

    public PresearchNewTabPageClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchNewTabPageClassName, "mNewTabPageLayout");
        makeProtectedField(sNewTabPageClassName, "mNewTabPageLayout");

        deleteField(sPresearchNewTabPageClassName, "mFeedSurfaceProvider");
        makeProtectedField(sNewTabPageClassName, "mFeedSurfaceProvider");
    }
}
