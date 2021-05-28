// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import Theme from 'brave-ui/theme/presearch-default'
import DarkTheme from 'brave-ui/theme/presearch-dark'
import '../common/defaultTrustedTypesPolicy'
import PresearchCoreThemeProvider from '../common/PresearchCoreThemeProvider'
import { wireApiEventsToStore } from './apiEventsToStore'
import * as topSitesAPI from './api/topSites'
import { init } from './actions/new_tab_actions'

// Components
import App from './containers/app'

// Utils
import store from './store'

// Let things handle 'init'
store.dispatch(init())

function initialize () {
  console.timeStamp('loaded')
  // Get rendering going
  new Promise(resolve => chrome.presearchTheme.getPresearchThemeType(resolve))
  .then((themeType: chrome.presearchTheme.ThemeType) => {
    render(
      <Provider store={store}>
        <PresearchCoreThemeProvider
          initialThemeType={themeType}
          dark={DarkTheme}
          light={Theme}
        >
          <App />
        </PresearchCoreThemeProvider>
      </Provider>,
      document.getElementById('root'),
      () => console.timeStamp('first react render'))
  })
  .catch((error) => {
    console.error('Problem mounting presearch new tab', error)
  })
  window.i18nTemplate.process(window.document, window.loadTimeData)
}

console.timeStamp('JS start')

// Get store data going
wireApiEventsToStore()

// Perform DOM-dependent initialization when ready
document.addEventListener('DOMContentLoaded', initialize)

// Update topsite tiles when NTP gets visible.
document.addEventListener('visibilitychange', () => {
  if (document.visibilityState === 'visible') {
    topSitesAPI.updateMostVisitedInfo()
  }
})
