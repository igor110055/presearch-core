/**
 * Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications.channels;

import android.app.NotificationManager;

import org.chromium.chrome.R;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions.PredefinedChannel;

import java.util.Map;
import java.util.Set;

public class PresearchChannelDefinitions {
    public class ChannelId {
        public static final String PRESEARCH_ADS = "com.presearch.ads";
        public static final String PRESEARCH_ADS_BACKGROUND = "com.presearch.ads.background";
        public static final String PRESEARCH_BROWSER = "com.presearch";
    }

    public class ChannelGroupId {
        public static final String PRESEARCH_ADS = "com.presearch.ads";
    }

    static protected void addPresearchChannels(
        Map<String, PredefinedChannel> map, Set<String> startup) {
        map.put(ChannelId.PRESEARCH_ADS,
                PredefinedChannel.create(ChannelId.PRESEARCH_ADS,
                                         R.string.notification_category_presearch_ads,
                                         NotificationManager.IMPORTANCE_HIGH, ChannelGroupId.PRESEARCH_ADS));
        startup.add(ChannelId.PRESEARCH_ADS);

        map.put(ChannelId.PRESEARCH_ADS_BACKGROUND,
                PredefinedChannel.create(ChannelId.PRESEARCH_ADS_BACKGROUND,
                                         R.string.notification_category_presearch_ads_background,
                                         NotificationManager.IMPORTANCE_LOW, ChannelGroupId.PRESEARCH_ADS));
        startup.add(ChannelId.PRESEARCH_ADS_BACKGROUND);
    }

    static protected void addPresearchChannelGroups(
        Map<String, ChannelDefinitions.PredefinedChannelGroup> map) {
        map.put(ChannelGroupId.PRESEARCH_ADS,
                new ChannelDefinitions.PredefinedChannelGroup(
                    ChannelGroupId.PRESEARCH_ADS, R.string.notification_category_group_presearch_ads));
    }
}
