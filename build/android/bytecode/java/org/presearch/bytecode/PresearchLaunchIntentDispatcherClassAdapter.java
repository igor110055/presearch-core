/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchLaunchIntentDispatcherClassAdapter extends PresearchClassVisitor {
    static String sLaunchIntentDispatcherClassName = "org/chromium/chrome/browser/LaunchIntentDispatcher";
    static String sPresearchLaunchIntentDispatcherClassName = "org/chromium/chrome/browser/PresearchLaunchIntentDispatcher";

    public PresearchLaunchIntentDispatcherClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sLaunchIntentDispatcherClassName, "isCustomTabIntent", sPresearchLaunchIntentDispatcherClassName);
    }
}
