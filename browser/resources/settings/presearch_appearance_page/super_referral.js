// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {Polymer, html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {assert} from 'chrome://resources/js/assert.m.js';
import {I18nBehavior} from 'chrome://resources/js/i18n_behavior.m.js';
import {loadTimeData} from "../i18n_setup.js"
import {AppearanceBrowserProxy, AppearanceBrowserProxyImpl} from '../appearance_page/appearance_browser_proxy.js';

/**
 * ID for autogenerated themes. Should match
 * |ThemeService::kAutogeneratedThemeID|.
 */
const AUTOGENERATED_THEME_ID = 'autogenerated_theme_id';

/**
 * 'settings-presearch-appearance-theme' is the settings page area containing
 * presearch's themes settings.
 */
Polymer({
  is: 'settings-presearch-appearance-super-referral',

  _template: html`{__html_template__}`,

  behaviors: [I18nBehavior],

  properties: {
    /** @private */
    themeSublabel_: String,

    /** @private */
    themeUrl_: String,

    /** @private */
    useSystemTheme_: {
      type: Boolean,
      value: false,  // Can only be true on Linux, but value exists everywhere.
    },

    // Sync with ThemesOption enum in ViewCounterService
    /** @private */
    themeValues_: {
      readOnly: true,
      type: Object,
      value: {
        DEFAULT: 0,
        SUPER_REFERRAL: 1,
      },
    },
  },

  /** @private {?settings.PresearchAppearanceBrowserProxy} */
  appearanceBrowserProxy_: null,

  observers: [
    'themeChanged_(prefs.extensions.theme.id.value, useSystemTheme_)',

    // <if expr="is_linux and not chromeos">
    // NOTE: this pref only exists on Linux.
    'useSystemThemePrefChanged_(prefs.extensions.theme.use_system.value)',
    // </if>
  ],

  /** @override */
  created: function() {
    this.appearanceBrowserProxy_ =
        AppearanceBrowserProxyImpl.getInstance();
  },

  getSuperReferralThemeName_() {
    return loadTimeData.getString('superReferralThemeName')
  },
  /**
   * Open URL for either current theme or the theme gallery.
   * @private
   */
  openThemeUrl_() {
    window.open(this.themeUrl_ || loadTimeData.getString('themesGalleryUrl'));
  },

  /** @private */
  onUseDefaultTap_() {
    this.appearanceBrowserProxy_.useDefaultTheme();
  },

  // <if expr="is_linux and not chromeos">
  /**
   * @param {boolean} useSystemTheme
   * @private
   */
  useSystemThemePrefChanged_(useSystemTheme) {
    this.useSystemTheme_ = useSystemTheme;
  },

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @return {boolean} Whether to show the "USE CLASSIC" button.
   * @private
   */
  showUseClassic_(themeId, useSystemTheme) {
    return !!themeId || useSystemTheme;
  },

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @return {boolean} Whether to show the "USE GTK+" button.
   * @private
   */
  showUseSystem_(themeId, useSystemTheme) {
    return (!!themeId || !useSystemTheme) &&
        !this.appearanceBrowserProxy_.isSupervised();
  },

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @return {boolean} Whether to show the secondary area where "USE CLASSIC"
   *     and "USE GTK+" buttons live.
   * @private
   */
  showThemesSecondary_(themeId, useSystemTheme) {
    return this.showUseClassic_(themeId, useSystemTheme) ||
        this.showUseSystem_(themeId, useSystemTheme);
  },

  /** @private */
  onUseSystemTap_() {
    this.appearanceBrowserProxy_.useSystemTheme();
  },
  // </if>

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @private
   */
  themeChanged_(themeId, useSystemTheme) {
    if (this.prefs == undefined || useSystemTheme == undefined) {
      return;
    }

    if (themeId.length > 0 && themeId != AUTOGENERATED_THEME_ID) {
      assert(!useSystemTheme);

      this.appearanceBrowserProxy_.getThemeInfo(themeId).then(info => {
        this.themeSublabel_ = info.name;
      });

      this.themeUrl_ = 'https://chrome.google.com/webstore/detail/' + themeId;
      return;
    }

    this.themeUrl_ = '';

    if (themeId == AUTOGENERATED_THEME_ID) {
      this.themeSublabel_ = this.i18n('chromeColors');
      return;
    }

    let i18nId;
    // <if expr="is_linux and not chromeos">
    i18nId = useSystemTheme ? 'systemTheme' : 'classicTheme';
    // </if>
    // <if expr="not is_linux or chromeos">
    i18nId = 'chooseFromWebStore';
    // </if>
    this.themeSublabel_ = this.i18n(i18nId);
  },

  /**
   * Open URL for either current theme or the theme gallery.
   * @private
   */
  openThemeUrl_() {
    window.open(this.themeUrl_ || loadTimeData.getString('themesGalleryUrl'));
  },

  /** @private */
  onUseDefaultTap_() {
    this.appearanceBrowserProxy_.useDefaultTheme();
  },

  // <if expr="is_linux and not chromeos">
  /**
   * @param {boolean} useSystemTheme
   * @private
   */
  useSystemThemePrefChanged_(useSystemTheme) {
    this.useSystemTheme_ = useSystemTheme;
  },

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @return {boolean} Whether to show the "USE CLASSIC" button.
   * @private
   */
  showUseClassic_(themeId, useSystemTheme) {
    return !!themeId || useSystemTheme;
  },

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @return {boolean} Whether to show the "USE GTK+" button.
   * @private
   */
  showUseSystem_(themeId, useSystemTheme) {
    return (!!themeId || !useSystemTheme) &&
        !this.appearanceBrowserProxy_.isSupervised();
  },

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @return {boolean} Whether to show the secondary area where "USE CLASSIC"
   *     and "USE GTK+" buttons live.
   * @private
   */
  showThemesSecondary_(themeId, useSystemTheme) {
    return this.showUseClassic_(themeId, useSystemTheme) ||
        this.showUseSystem_(themeId, useSystemTheme);
  },

  /** @private */
  onUseSystemTap_() {
    this.appearanceBrowserProxy_.useSystemTheme();
  },
  // </if>

  /**
   * @param {string} themeId
   * @param {boolean} useSystemTheme
   * @private
   */
  themeChanged_(themeId, useSystemTheme) {
    if (this.prefs == undefined || useSystemTheme == undefined) {
      return;
    }

    if (themeId.length > 0 && themeId != AUTOGENERATED_THEME_ID) {
      assert(!useSystemTheme);

      this.appearanceBrowserProxy_.getThemeInfo(themeId).then(info => {
        this.themeSublabel_ = info.name;
      });

      this.themeUrl_ = 'https://chrome.google.com/webstore/detail/' + themeId;
      return;
    }

    this.themeUrl_ = '';

    if (themeId == AUTOGENERATED_THEME_ID) {
      this.themeSublabel_ = this.i18n('chromeColors');
      return;
    }

    let i18nId;
    // <if expr="is_linux and not chromeos">
    i18nId = useSystemTheme ? 'systemTheme' : 'classicTheme';
    // </if>
    // <if expr="not is_linux or chromeos">
    i18nId = 'chooseFromWebStore';
    // </if>
    this.themeSublabel_ = this.i18n(i18nId);
  }
});

