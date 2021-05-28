/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchMainPreferenceBaseClassAdapter extends PresearchClassVisitor {
    static String sMainPreferencesClassName =
            "org/chromium/chrome/browser/settings/MainSettings";

    static String sPresearchMainPreferencesBaseClassName =
            "org/chromium/chrome/browser/settings/PresearchMainPreferencesBase";

    public PresearchMainPreferenceBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sMainPreferencesClassName,
                        sPresearchMainPreferencesBaseClassName);
    }
}
