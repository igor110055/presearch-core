/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchTabbedActivityClassAdapter extends PresearchClassVisitor {
    static String sChromeTabbedActivityClassName =
            "org/chromium/chrome/browser/ChromeTabbedActivity";
    static String sPresearchActivityClassName = "org/chromium/chrome/browser/app/PresearchActivity";
    static String sTabbedRootUiCoordinatorClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedRootUiCoordinator";
    static String sTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedAppMenuPropertiesDelegate";
    static String sPresearchTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/appmenu/PresearchTabbedAppMenuPropertiesDelegate";
    static String sChromeTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/ChromeTabCreator";
    static String sPresearchTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/PresearchTabCreator";
    static String sAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/AppMenuPropertiesDelegateImpl";
    static String sPresearchAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/PresearchAppMenuPropertiesDelegateImpl";
    static String sCustomTabAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/customtabs/CustomTabAppMenuPropertiesDelegate";

    public PresearchTabbedActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sChromeTabbedActivityClassName, sPresearchActivityClassName);

        changeSuperName(sTabbedAppMenuPropertiesDelegateClassName,
                sPresearchAppMenuPropertiesDelegateImplClassName);

        changeSuperName(sCustomTabAppMenuPropertiesDelegateClassName,
                sPresearchAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(sTabbedAppMenuPropertiesDelegateClassName,
                sPresearchTabbedAppMenuPropertiesDelegateClassName);

        redirectConstructor(sAppMenuPropertiesDelegateImplClassName,
                sPresearchAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(sChromeTabCreatorClassName, sPresearchTabCreatorClassName);

        makePublicMethod(sChromeTabbedActivityClassName, "hideOverview");
    }
}
