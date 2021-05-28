// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {RegisterPolymerTemplateModifications} from 'chrome://presearch-resources/polymer_overriding.js'
import {I18nBehavior} from 'chrome://resources/js/i18n_behavior.m.js'
import {Router} from '../router.m.js'
import {loadTimeData} from '../i18n_setup.js'

import '../presearch_appearance_page/super_referral.js'
import '../presearch_appearance_page/presearch_theme.js'
import '../presearch_appearance_page/sidebar.js'
import '../presearch_appearance_page/toolbar.js'

const superReferralStringId = 'superReferralThemeName'

RegisterPolymerTemplateModifications({
  'settings-appearance-page': (templateContent) => {
    const theme = templateContent.getElementById('themeRow')
    if (!theme) {
      console.error(`[Presearch Settings Overrides] Couldn't find #themeRow`)
    } else {
      theme.insertAdjacentHTML('beforebegin', `
        <settings-presearch-appearance-theme prefs="{{prefs}}"></settings-presearch-appearance-theme>
      `)
    }
    const r = Router.getInstance().routes_
    // Super-referral
    // W/o super referral, we don't need to themes link option with themes sub
    // page.
    const hasSuperReferral = (
      loadTimeData.valueExists(superReferralStringId) &&
      loadTimeData.getString(superReferralStringId) !== ''
    )
    if (hasSuperReferral) {
      // Routes
      if (!r.APPEARANCE) {
        console.error('[Presearch Settings Overrides] Routes: could not find APPEARANCE page')
        return
      } else {
        r.THEMES = r.APPEARANCE.createChild('/themes');
        // Hide chromium's theme section. It's replaced with our themes page.
        if (theme) {
          theme.remove()
        }
      }
    }
    // Toolbar prefs
    const bookmarkBarToggle = templateContent.querySelector('[pref="{{prefs.bookmark_bar.show_on_all_tabs}}"]')
    if (!bookmarkBarToggle) {
      console.error(`[Presearch Settings Overrides] Couldn't find bookmark bar toggle`)
    } else {
      bookmarkBarToggle.insertAdjacentHTML('beforebegin', `
        <settings-presearch-appearance-sidebar prefs="{{prefs}}"></settings-presearch-appearance-sidebar>
      `)
      bookmarkBarToggle.insertAdjacentHTML('afterend', `
        <settings-presearch-appearance-toolbar prefs="{{prefs}}"></settings-presearch-appearance-toolbar>
      `)
    }
    const zoomLevel = templateContent.getElementById('zoomLevel')
    if (!zoomLevel || !zoomLevel.parentNode) {
      console.error(`[Presearch Settings Overrides] Couldn't find zoomLevel`)
    } else {
      zoomLevel.parentNode.insertAdjacentHTML('afterend', `
        <settings-toggle-button
          class="hr"
          pref="{{prefs.presearch.mru_cycling_enabled}}"
          label="${I18nBehavior.i18n('mruCyclingSettingLabel')}">
        </settings-toggle-button>
      `)
    }
    // Super referral themes prefs
    const pages = templateContent.getElementById('pages')
    if (!pages) {
      console.error(`[Presearch Settings Overrides] Couldn't find appearance_page #pages`)
    } else {
      pages.insertAdjacentHTML('beforeend', `
        <template is="dom-if" route-path="/themes">
          <settings-subpage
          associated-control="[[$$('#themes-subpage-trigger')]]"
          page-title="${I18nBehavior.i18n('themes')}">
            <settings-presearch-appearance-super-referral prefs="{{prefs}}">
            </settings-presearch-appearance-super-referral>
          </settings-subpage>
        </template>
      `)
    }
  },
})
