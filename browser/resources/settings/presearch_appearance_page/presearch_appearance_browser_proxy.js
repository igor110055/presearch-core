/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import {addSingletonGetter, sendWithPromise} from 'chrome://resources/js/cr.m.js';

/** @interface */
export class PresearchAppearanceBrowserProxy {
  /**
   * @return {!Promise<Array>}
   */
  getPresearchThemeList() {}
  /**
   * @return {!Promise<Number>}
   */
  getPresearchThemeType() {}
  /**
   * @param {Number} type
   */
  setPresearchThemeType(value) {}
}

/**
 * @implements {PresearchAppearanceBrowserProxy}
 */
export class PresearchAppearanceBrowserProxyImpl {
  /** @override */
  getPresearchThemeList() {
    return new Promise(resolve => chrome.presearchTheme.getPresearchThemeList(resolve))
  }
  getPresearchThemeType() {
    return sendWithPromise('getPresearchThemeType');
  }
  setPresearchThemeType(value) {
    chrome.send('setPresearchThemeType', [value]);
  }
}

addSingletonGetter(PresearchAppearanceBrowserProxyImpl);
