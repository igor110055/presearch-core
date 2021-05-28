/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchMimeUtilsClassAdapter extends PresearchClassVisitor {
    static String sMimeUtilsClassName = "org/chromium/chrome/browser/download/MimeUtils";

    static String sPresearchMimeUtilsClassName = "org/chromium/chrome/browser/download/PresearchMimeUtils";

    public PresearchMimeUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sMimeUtilsClassName, "canAutoOpenMimeType", sPresearchMimeUtilsClassName);
    }
}
