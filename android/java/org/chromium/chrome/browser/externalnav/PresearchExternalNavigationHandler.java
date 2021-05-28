/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.externalnav;

import org.chromium.components.external_intents.ExternalNavigationDelegate;
import org.chromium.components.external_intents.ExternalNavigationHandler;
import org.chromium.components.external_intents.ExternalNavigationParams;
import org.chromium.components.external_intents.ExternalNavigationHandler.OverrideUrlLoadingResult;
import org.chromium.chrome.browser.PresearchUphold;

public class PresearchExternalNavigationHandler extends ExternalNavigationHandler {
    private PresearchUphold mPresearchUphold;

    public PresearchExternalNavigationHandler(ExternalNavigationDelegate delegate) {
        super(delegate);
    }

    @Override
    public OverrideUrlLoadingResult shouldOverrideUrlLoading(ExternalNavigationParams params) {
        if (isUpholdOverride(params)) {
            CompleteUpholdVerification(params);
            return OverrideUrlLoadingResult.forClobberingTab();
        }
        return super.shouldOverrideUrlLoading(params);
    }

    private boolean isUpholdOverride(ExternalNavigationParams params) {
        if (!params.getUrl().startsWith(PresearchUphold.UPHOLD_REDIRECT_URL)) return false;
        return true;
    }

    private void CompleteUpholdVerification(ExternalNavigationParams params) {
        mPresearchUphold = new PresearchUphold();
        mPresearchUphold.CompleteUpholdVerification(params, this);
    }

    public OverrideUrlLoadingResult clobberCurrentTabWithFallbackUrl(
            String browserFallbackUrl, ExternalNavigationParams params) {
        // Below is an actual code that was used prior to deletion of
        // clobberCurrentTabWithFallbackUrl introduced here
        // https://chromium.googlesource.com/chromium/src/+/37b5b744bc83f630d3121b46868818bb4e848c2a
        if (!params.isMainFrame()) {
            return OverrideUrlLoadingResult.forNoOverride();
        }

        if (params.getRedirectHandler() != null) {
            params.getRedirectHandler().setShouldNotOverrideUrlLoadingOnCurrentRedirectChain();
        }
        return clobberCurrentTab(browserFallbackUrl, params.getReferrerUrl());
    }
}