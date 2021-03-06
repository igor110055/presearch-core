// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

(function() {
  'use strict';

  /**
   * 'settings-presearch-new-tab-page' is the settings page containing
   * presearch's new tab features.
   */
  Polymer({
    is: 'settings-presearch-new-tab-page',

    /** @private {?settings.PresearchNewTabBrowserProxy} */
    browserProxy_: null,

    behaviors: [
      WebUIListenerBehavior,
    ],

    properties: {
      newTabShowOptions_: Array,
      shouldNewTabShowDashboardSettings_: Boolean,
    },

    /** @override */
    created: function() {
      this.browserProxy_ = settings.PresearchNewTabBrowserProxyImpl.getInstance();
      this.showNewTabDashboardSettings_ = false;
    },

    /** @override */
    ready: function() {
      this.openNewTabPage_ = this.openNewTabPage_.bind(this)

      this.browserProxy_.getNewTabShowsOptionsList().then(list => {
        this.newTabShowOptions_ = list;
      })
      this.browserProxy_.shouldShowNewTabDashboardSettings().then(showNewTabDashboardSettings => {
        this.showNewTabDashboardSettings_ = showNewTabDashboardSettings;
      })

      this.addWebUIListener('show-new-tab-dashboard-settings-changed', (show) => {
        this.showNewTabDashboardSettings_ = show
      })
    },

    openNewTabPage_: function () {
      window.open("chrome://newTab?openSettings=1", "_self");
    }
  });
})();
