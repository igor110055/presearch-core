// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { initLocale } from 'brave-ui'

import 'emptykit.css'

// Fonts
import '../../ui/webui/resources/fonts/poppins.css'
import '../../ui/webui/resources/fonts/muli.css'

// Components
import App from './components/app'
import Theme from 'brave-ui/theme/brave-default'
import DarkTheme from 'brave-ui/theme/brave-dark'
import PresearchCoreThemeProvider from '../common/PresearchCoreThemeProvider'

function initialize () {
  chrome.presearchWallet.shouldPromptForSetup((prompt: boolean) => {
    if (!prompt) {
      chrome.presearchWallet.loadUI(() => {
        window.location.href = 'chrome://wallet'
      })
      return
    }

    renderWebUIView()
  })
}

function renderWebUIView () {
  new Promise(resolve => chrome.presearchTheme.getPresearchThemeType(resolve))
  .then((themeType: chrome.presearchTheme.ThemeType) => {
    window.i18nTemplate.process(window.document, window.loadTimeData)
    if (window.loadTimeData && window.loadTimeData.data_) {
      initLocale(window.loadTimeData.data_)
    }

    render(
      <PresearchCoreThemeProvider
        initialThemeType={themeType}
        dark={DarkTheme}
        light={Theme}
      >
        <App />
      </PresearchCoreThemeProvider>,
      document.getElementById('root')
    )
  })
  .catch(({ message }) => {
    console.error(`Could not mount presearch wallet: ${message}`)
  })
}

document.addEventListener('DOMContentLoaded', initialize)
