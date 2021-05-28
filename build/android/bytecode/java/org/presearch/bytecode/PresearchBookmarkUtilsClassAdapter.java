/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchBookmarkUtilsClassAdapter extends PresearchClassVisitor {
    static String sBookmarkUtilsClassName = "org/chromium/chrome/browser/bookmarks/BookmarkUtils";
    static String sPresearchBookmarkUtilsClassName =
            "org/chromium/chrome/browser/bookmarks/PresearchBookmarkUtils";

    public PresearchBookmarkUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);
        makePublicMethod(sBookmarkUtilsClassName, "showBookmarkBottomSheet");
        changeMethodOwner(
                sPresearchBookmarkUtilsClassName, "showBookmarkBottomSheet", sBookmarkUtilsClassName);
        makePublicMethod(sBookmarkUtilsClassName, "addBookmarkAndShowSnackbar");
        changeMethodOwner(sPresearchBookmarkUtilsClassName, "addBookmarkAndShowSnackbar",
                sBookmarkUtilsClassName);
        changeMethodOwner(
                sBookmarkUtilsClassName, "addOrEditBookmark", sPresearchBookmarkUtilsClassName);
    }
}
