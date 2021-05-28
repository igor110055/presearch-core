/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.presearch.bytecode;

import org.objectweb.asm.ClassVisitor;

public class PresearchClassAdapter {
    public static ClassVisitor createAdapter(ClassVisitor chain) {
        chain = new PresearchActivityClassAdapter(chain);
        chain = new PresearchAndroidSyncSettingsClassAdapter(chain);
        chain = new PresearchAppMenuClassAdapter(chain);
        chain = new PresearchBookmarkUtilsClassAdapter(chain);
        chain = new PresearchBottomControlsCoordinatorClassAdapter(chain);
        chain = new PresearchBottomControlsMediatorClassAdapter(chain);
        chain = new PresearchCommandLineInitUtilClassAdapter(chain);
        chain = new PresearchEditUrlSuggestionProcessorClassAdapter(chain);
        chain = new PresearchFeedSurfaceCoordinatorClassAdapter(chain);
        chain = new PresearchHomepageManagerClassAdapter(chain);
        chain = new PresearchIncognitoToggleTabLayoutClassAdapter(chain);
        chain = new PresearchLaunchIntentDispatcherClassAdapter(chain);
        chain = new PresearchMainPreferenceBaseClassAdapter(chain);
        chain = new PresearchManageSyncSettingsClassAdapter(chain);
        chain = new PresearchMimeUtilsClassAdapter(chain);
        chain = new PresearchNewTabPageClassAdapter(chain);
        chain = new PresearchNewTabPageLayoutClassAdapter(chain);
        chain = new PresearchPasswordSettingsBaseClassAdapter(chain);
        chain = new PresearchPermissionDialogDelegateClassAdapter(chain);
        chain = new PresearchPermissionDialogModelClassAdapter(chain);
        chain = new PresearchQueryTileSectionClassAdapter(chain);
        chain = new PresearchSearchEngineAdapterClassAdapter(chain);
        chain = new PresearchSettingsLauncherImplClassAdapter(chain);
        chain = new PresearchSingleCategorySettingsClassAdapter(chain);
        chain = new PresearchSiteSettingsDelegateClassAdapter(chain);
        chain = new PresearchTabbedActivityClassAdapter(chain);
        chain = new PresearchTabGroupUiCoordinatorClassAdapter(chain);
        chain = new PresearchTabSwitcherModeTTCoordinatorPhoneClassAdapter(chain);
        chain = new PresearchTabSwitcherModeTTPhoneClassAdapter(chain);
        chain = new PresearchTileViewClassAdapter(chain);
        chain = new PresearchToolbarLayoutClassAdapter(chain);
        chain = new PresearchToolbarManagerClassAdapter(chain);
        chain = new PresearchTopToolbarCoordinatorClassAdapter(chain);
        return chain;
    }
}
