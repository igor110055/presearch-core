/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchSiteSettingsDelegateClassAdapter extends PresearchClassVisitor {

    static String sChromeSiteSettingsDelegateClassName = "org/chromium/chrome/browser/site_settings/ChromeSiteSettingsDelegate";
    static String sPresearchSiteSettingsDelegateClassName = "org/chromium/chrome/browser/site_settings/PresearchSiteSettingsDelegate";

    public PresearchSiteSettingsDelegateClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sChromeSiteSettingsDelegateClassName, sPresearchSiteSettingsDelegateClassName);
    }
}
