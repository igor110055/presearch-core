/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchCommandLineInitUtilClassAdapter extends PresearchClassVisitor {
    static String sCommandLineClassName =
        "org/chromium/base/CommandLineInitUtil";

    static String sPresearchCommandLineClassName =
        "org/chromium/base/PresearchCommandLineInitUtil";

    static String sMethodReadFileAsUtf8 = "initCommandLine";

    public PresearchCommandLineInitUtilClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sCommandLineClassName, sMethodReadFileAsUtf8,
                          sPresearchCommandLineClassName);
    }
}
