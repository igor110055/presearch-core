/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
 
(function() {
'use strict';

/**
 * 'settings-presearch-default-extensions-page' is the settings page containing
 * presearch's default extensions.
 */
Polymer({
  is: 'settings-presearch-wallet-page',

  behaviors: [
    WebUIListenerBehavior,
  ],

  properties: {
    isNativeWalletEnabled_: Boolean
  },

  /** @private {?settings.PresearchWalletBrowserProxy} */
  browserProxy_: null,

  /** @override */
  created: function() {
    this.browserProxy_ = settings.PresearchWalletBrowserProxyImpl.getInstance();
  },

  /** @override */
  ready: function() {
    this.onPresearchWalletEnabledChange_ = this.onPresearchWalletEnabledChange_.bind(this)
    this.browserProxy_.getWeb3ProviderList().then(list => {
      this.presearchWeb3Providers_ = JSON.parse(list)
    });
    this.browserProxy_.isNativeWalletEnabled().then(val => {
      this.isNativeWalletEnabled_ = val;
    });
  },

  onPresearchWalletEnabledChange_: function() {
    this.browserProxy_.setPresearchWalletEnabled(this.$.presearchWalletEnabled.checked);
  },
});
})();
