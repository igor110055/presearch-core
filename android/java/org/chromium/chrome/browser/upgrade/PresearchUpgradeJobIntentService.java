/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.upgrade;

import org.chromium.chrome.browser.base.SplitCompatJobIntentService;
import org.chromium.chrome.browser.base.SplitCompatUtils;

public class PresearchUpgradeJobIntentService extends SplitCompatJobIntentService {
    public PresearchUpgradeJobIntentService() {
        super(SplitCompatUtils.getIdentifierName(
                "org.chromium.chrome.browser.upgrade.PresearchUpgradeJobIntentServiceImpl"));
    }
}
