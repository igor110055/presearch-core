/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {addSingletonGetter} from 'chrome://resources/js/cr.m.js';

/** @interface */
export class PresearchWalletBrowserProxy {
  /**
   * @param {boolean} value name.
   */
   setPresearchWalletEnabled (value) {}
   getWeb3ProviderList () {}
   isNativeWalletEnabled() {}
}

/**
 * @implements {settings.PresearchWalletBrowserProxy}
 */
export class PresearchWalletBrowserProxyImpl {
  /** @override */
  setPresearchWalletEnabled (value) {
    chrome.send('setPresearchWalletEnabled', [value])
  }

  /** @override */
  getWeb3ProviderList () {
    return new Promise(resolve => chrome.presearchWallet.getWeb3ProviderList(resolve))
  }

  /** @override */
  isNativeWalletEnabled () {
    return new Promise(resolve => chrome.presearchWallet.isNativeWalletEnabled(resolve))
  }
}

addSingletonGetter(PresearchWalletBrowserProxyImpl)
