/** Copyright (c) 2019 The Presearch Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser.notifications;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.browser.notifications.channels.PresearchChannelDefinitions;

/**
 * This class provides the Presearch Ads related methods for the native library
 * (presearch/components/presearch_ads/browser/notification_helper_android)
 */
public abstract class PresearchAds {
    @CalledByNative
    public static String getPresearchAdsChannelId() {
        return PresearchChannelDefinitions.ChannelId.PRESEARCH_ADS;
    }

    @CalledByNative
    public static String getPresearchAdsBackgroundChannelId() {
        return PresearchChannelDefinitions.ChannelId.PRESEARCH_ADS_BACKGROUND;
    }
}
