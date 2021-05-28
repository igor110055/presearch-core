/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { render } from 'react-dom'
import { bindActionCreators } from 'redux'
import { Provider } from 'react-redux'

import welcomeDarkTheme from './theme/welcome-dark'
import welcomeLightTheme from './theme/welcome-light'
import PresearchCoreThemeProvider from '../common/PresearchCoreThemeProvider'

// Components
import App from './containers/app'

// Utils
import store from './store'
import * as welcomeActions from './actions/welcome_actions'

window.cr.define('presearch_welcome', function () {
  'use strict'

  function loadWelcomeData () {
    const actions = bindActionCreators(welcomeActions, store.dispatch.bind(store))
    actions.getSearchEngineProviders()
    actions.getBrowserProfiles()
  }

  function initialize () {
    loadWelcomeData()
    new Promise(resolve => chrome.presearchTheme.getPresearchThemeType(resolve))
    .then((themeType: chrome.presearchTheme.ThemeType) => {
      render(
        <Provider store={store}>
          <PresearchCoreThemeProvider
            initialThemeType={themeType}
            dark={welcomeDarkTheme}
            light={welcomeLightTheme}
          >
            <App />
          </PresearchCoreThemeProvider>
        </Provider>,
        document.getElementById('root'))
    })
    .catch((error) => {
      console.error('Problem mounting presearch welcome', error)
    })
    window.i18nTemplate.process(window.document, window.loadTimeData)
  }

  return {
    initialize
  }
})

document.addEventListener('DOMContentLoaded', window.presearch_welcome.initialize)
