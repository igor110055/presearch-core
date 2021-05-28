/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import org.chromium.chrome.browser.feedback.HelpAndFeedbackLauncher;
import org.chromium.chrome.browser.help.PresearchHelpAndFeedbackLauncher;
import org.chromium.chrome.browser.partnerbookmarks.PartnerBookmark;
import org.chromium.chrome.browser.partnercustomizations.PresearchPartnerBrowserCustomizations;
import org.chromium.chrome.browser.partnercustomizations.PartnerBrowserCustomizations;

public class PresearchAppHooks extends AppHooksImpl {
    @Override
    public HelpAndFeedbackLauncher createHelpAndFeedbackLauncher() {
        return new PresearchHelpAndFeedbackLauncher();
    }

    @Override
    public PartnerBookmark.BookmarkIterator getPartnerBookmarkIterator() {
        return null;
    }

    @Override
    public PartnerBrowserCustomizations.Provider getCustomizationProvider() {
        return new PresearchPartnerBrowserCustomizations.ProviderPackage();
    }
}
