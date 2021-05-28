/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchPasswordSettingsBaseClassAdapter extends PresearchClassVisitor {
    static String sPasswordSettingsClassName =
            "org/chromium/chrome/browser/password_manager/settings/PasswordSettings";
    static String sPresearchPasswordSettingsBaseClassName =
            "org/chromium/chrome/browser/password_manager/settings/PresearchPasswordSettingsBase";

    public PresearchPasswordSettingsBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sPasswordSettingsClassName, sPresearchPasswordSettingsBaseClassName);

        changeMethodOwner(sPasswordSettingsClassName, "createCheckPasswords",
                sPresearchPasswordSettingsBaseClassName);
        deleteMethod(sPasswordSettingsClassName, "createCheckPasswords");
    }
}
