/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchManageSyncSettingsClassAdapter extends PresearchClassVisitor {
    static String sManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/ManageSyncSettings";
    static String sPresearchManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/PresearchManageSyncSettings";

    PresearchManageSyncSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sPresearchManageSyncSettingsClassName, "mGoogleActivityControls");
        makeProtectedField(sManageSyncSettingsClassName, "mGoogleActivityControls");

        deleteField(sPresearchManageSyncSettingsClassName, "mSyncEncryption");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEncryption");

        deleteField(sPresearchManageSyncSettingsClassName, "mManageSyncData");
        makeProtectedField(sManageSyncSettingsClassName, "mManageSyncData");

        deleteField(sPresearchManageSyncSettingsClassName, "mSyncPaymentsIntegration");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncPaymentsIntegration");
    }
}
