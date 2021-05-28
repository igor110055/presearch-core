/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchTileViewClassAdapter extends PresearchClassVisitor {
    static String sSuggestionsTileViewClassName =
            "org/chromium/chrome/browser/suggestions/tile/SuggestionsTileView";

    static String sPresearchSuggestionsTileViewBaseClassName =
            "org/chromium/chrome/browser/suggestions/tile/PresearchTileView";

    public PresearchTileViewClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sSuggestionsTileViewClassName, sPresearchSuggestionsTileViewBaseClassName);
    }
}
