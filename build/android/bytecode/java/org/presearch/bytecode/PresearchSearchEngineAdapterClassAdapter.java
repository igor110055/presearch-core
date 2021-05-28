/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchSearchEngineAdapterClassAdapter extends PresearchClassVisitor {
    static String sSearchEngineAdapterClassName =
        "org/chromium/chrome/browser/search_engines/settings/SearchEngineAdapter";

    static String sPresearchSearchEngineAdapterBaseClassName =
        "org/chromium/chrome/browser/search_engines/settings/PresearchBaseSearchEngineAdapter";

    static String sMethodGetPermissionsLinkMessage =
        "getPermissionsLinkMessage";

    static String sMethodGetSearchEngineSourceType =
        "getSearchEngineSourceType";

    static String sMethodSortAndFilterUnnecessaryTemplateUrl = "sortAndFilterUnnecessaryTemplateUrl";

    public PresearchSearchEngineAdapterClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sSearchEngineAdapterClassName,
                        sPresearchSearchEngineAdapterBaseClassName);
        changeMethodOwner(sSearchEngineAdapterClassName, sMethodGetSearchEngineSourceType,
                          sPresearchSearchEngineAdapterBaseClassName);
        changeMethodOwner(sSearchEngineAdapterClassName, sMethodGetPermissionsLinkMessage,
                          sPresearchSearchEngineAdapterBaseClassName);
        changeMethodOwner(sSearchEngineAdapterClassName, sMethodSortAndFilterUnnecessaryTemplateUrl,
                          sPresearchSearchEngineAdapterBaseClassName);
    }
}
