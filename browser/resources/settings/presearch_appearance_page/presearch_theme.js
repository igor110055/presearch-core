// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {Polymer, html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {WebUIListenerBehavior} from 'chrome://resources/js/web_ui_listener_behavior.m.js';
import {routes} from '../route.js';
import {Router} from '../router.m.js';
import 'chrome://resources/cr_elements/md_select_css.m.js';
import '../settings_shared_css.m.js';
import '../settings_vars_css.m.js';
import {loadTimeData} from "../i18n_setup.js"
import {PresearchAppearanceBrowserProxy,  PresearchAppearanceBrowserProxyImpl} from './presearch_appearance_browser_proxy.js';

/**
 * 'settings-presearch-appearance-theme' is the settings page area containing
 * presearch's appearance related settings that located at the top of appearance
 * area.
 */
Polymer({
  is: 'settings-presearch-appearance-theme',

  _template: html`{__html_template__}`,

  behaviors: [
    WebUIListenerBehavior,
  ],

  properties: {
    presearchThemeList_: Array,
    presearchThemeType_: Number,
  },

  /** @private {?settings.PresearchAppearanceBrowserProxy} */
  browserProxy_: null,

  observers: [
    'updateSelected_(presearchThemeType_, presearchThemeList_)',
  ],

  /** @override */
  created: function() {
    this.browserProxy_ = PresearchAppearanceBrowserProxyImpl.getInstance();
  },

  /** @override */
  ready: function() {
    this.addWebUIListener('presearch-theme-type-changed', (type) => {
      this.presearchThemeType_ = type;
    })
    this.browserProxy_.getPresearchThemeList().then(list => {
      this.presearchThemeList_ = JSON.parse(list);
    })
    this.browserProxy_.getPresearchThemeType().then(type => {
      this.presearchThemeType_ = type;
    })
  },

  onPresearchThemeTypeChange_: function() {
    this.browserProxy_.setPresearchThemeType(Number(this.$.presearchThemeType.value));
  },

  presearchThemeTypeEqual_: function(theme1, theme2) {
    return theme1 === theme2;
  },

  onThemeTap_: function() {
    Router.getInstance().navigateTo(routes.THEMES);
  },

  // Wait for the dom-repeat to populate the <select> before setting
  // <select>#value so the correct option gets selected.
  updateSelected_: function() {
    this.async(() => {
      this.$.presearchThemeType.value = this.presearchThemeType_;
    });
  },

  useThemesSubPage_: function() {
    return loadTimeData.getString('superReferralThemeName') !== "";
  }
});
