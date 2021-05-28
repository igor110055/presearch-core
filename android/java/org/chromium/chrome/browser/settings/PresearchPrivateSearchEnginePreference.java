/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

public class PresearchPrivateSearchEnginePreference extends PresearchSearchEnginePreference {
    public PresearchPrivateSearchEnginePreference() {
        super(true /* private */);
    }
}
