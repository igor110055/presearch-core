// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {RegisterStyleOverride, RegisterPolymerTemplateModifications} from 'chrome://presearch-resources/polymer_overriding.js'
import {loadTimeData} from '../i18n_setup.js'
import '../presearch_icons.m.js'

function createMenuElement (title, href, iconName, pageVisibilitySection) {
  const menuEl = document.createElement('a')
  if (pageVisibilitySection) {
    menuEl.setAttribute('hidden', `[[!pageVisibility.${pageVisibilitySection}]]`)
  }
  menuEl.href = href
  const child = document.createElement('iron-icon')
  child.setAttribute('icon', iconName)
  menuEl.appendChild(child)
  const text = document.createTextNode(title)
  menuEl.appendChild(text)
  return menuEl
}

function getMenuElement (templateContent, href) {
  const menuEl = templateContent.querySelector(`a[href="${href}"]`)
  if (!menuEl) {
    console.error(`[Presearch Settings Overrides] Could not find menu item '${href}'`)
  }
  return menuEl
}

RegisterStyleOverride(
  'settings-menu',
  html`
    <style>
      :host {
        --presearch-settings-menu-margin-v: 30px;
        --presearch-settings-menu-padding: 30px;
        --settings-nav-item-color: #424242 !important;
        position: sticky;
        top: var(--presearch-settings-menu-margin-v);
        margin: 0 var(--presearch-settings-menu-margin) !important;
        max-height: calc(100vh - 56px - (var(--presearch-settings-menu-margin-v) * 2) - (var(--presearch-settings-menu-padding) * 2));
        min-width: 172px;
        border-radius: 6px;
        background-color: #fff;
        overflow-y: auto;
        padding: 30px !important;
      }

      @media (prefers-color-scheme: dark) {
        :host {
          --settings-nav-item-color: #F4F4F4 !important;
          border-color: transparent !important;
          background-color: #161719;
        }
      }

      a[href] {
        font-weight: 400 !important;
        margin: 0 20px 20px 0 !important;
        margin-inline-start: 0 !important;
        margin-inline-end: 0 !important;
        padding-bottom: 0 !important;
        padding-top: 0 !important;
        padding-inline-start: 0 !important;
      }

      a[href].iron-selected {
        color: var(--settings-nav-item-color) !important;
        font-weight: 600 !important;
      }

      iron-icon {
        color: var(--settings-nav-item-color);
        margin-inline-end: 16px !important;
        width: 20px;
        height: 20px;
      }

      @media (prefers-color-scheme: dark) {
        a[href].iron-selected iron-icon {
          color: var(--settings-nav-item-color) !important;
        }
      }

      a[href],
      #advancedButton {
        --cr-selectable-focus_-_outline: var(--presearch-focus-outline) !important;
      }

      #advancedButton {
        padding: 0 !important;
        margin-top: 30px !important;
        line-height: 1.25 !important;
        border: none !important;
      }

      #advancedButton > iron-icon {
        margin-inline-end: 0 !important;
      }

      #settingsHeader,
      #advancedButton {
        align-items: center !important;
        font-weight: normal !important;
        font-size: larger !important;
        color: var(--settings-nav-item-color) !important;
        margin-bottom: 20px !important;
      }

      #about-menu {
        display: flex;
        flex-direction: row;
        align-items: flex-start;
        justify-content: flex-start;
        color: #c5c5d3 !important;
        margin: 16px 0 0 0 !important;
      }
      .presearch-about-graphic {
        flex: 0;
        flex-basis: 30%;
        display: flex;
        align-items: center;
        justify-content: flex-start;
        align-self: stretch;
      }
      .presearch-about-meta {
        flex: 1;
      }
      .presearch-about-item {
        display: block;
      }
    </style>
  `
)

RegisterPolymerTemplateModifications({
  'settings-menu': (templateContent) => {
    // Add title
    const titleEl = document.createElement('h1')
    titleEl.id = 'settingsHeader'
    titleEl.textContent = loadTimeData.getString('settings')
    const topMenuEl = templateContent.querySelector('#topMenu')
    if (!topMenuEl) {
      console.error('[Presearch Settings Overrides] Could not find topMenu element to add title after')
    } else {
      topMenuEl.insertAdjacentElement('afterbegin', titleEl)
    }
    // Advanced text
    const advancedToggle = templateContent.querySelector('#advancedButton span')
    if (!advancedToggle) {
      console.error('[Presearch Settings Overrides] Could not find advancedButton to modify text')
    }
    advancedToggle.textContent = loadTimeData.getString('presearchAdditionalSettingsTitle')
    // Add 'Get Started' item
    const peopleEl = getMenuElement(templateContent, '/people')
    const getStartedEl = createMenuElement(
      loadTimeData.getString('presearchGetStartedTitle'),
      '/getStarted',
      'presearch_settings:get-started',
      'getStarted'
    )
    peopleEl.insertAdjacentElement('afterend', getStartedEl)
    // Move Appearance item
    const appearanceBrowserEl = getMenuElement(templateContent, '/appearance')
    getStartedEl.insertAdjacentElement('afterend', appearanceBrowserEl)

    // Add New Tab item
    const newTabEl = createMenuElement(
      loadTimeData.getString('presearchNewTab'),
      '/newTab',
      'presearch_settings:new-tab',
      'newTab'
    )
    appearanceBrowserEl.insertAdjacentElement('afterend', newTabEl)
    // Add Sync and Help Tips item
    const helpTipsEl = createMenuElement(
      loadTimeData.getString('presearchHelpTips'),
      '/presearchHelpTips',
      'presearch_settings:help',
      'presearchHelpTips',
    )
    const syncEl = createMenuElement(
      loadTimeData.getString('presearchSync'),
      '/presearchSync',
      'presearch_settings:sync',
      'presearchSync',
    )
    newTabEl.insertAdjacentElement('afterend', syncEl)
    syncEl.insertAdjacentElement('afterend', helpTipsEl)
    // Add Shields item
    const shieldsEl = createMenuElement(
      loadTimeData.getString('presearchShieldsTitle'),
      '/shields',
      'presearch_settings:shields',
      'shields',
    )
    helpTipsEl.insertAdjacentElement('afterend', shieldsEl)
    // Add Embed Blocking item
    const embedEl = createMenuElement(
      loadTimeData.getString('socialBlocking'),
      '/socialBlocking',
      'presearch_settings:social-permissions',
      'socialBlocking',
    )
    shieldsEl.insertAdjacentElement('afterend', embedEl)
    // Move search item
    const searchEl = getMenuElement(templateContent, '/search')
    embedEl.insertAdjacentElement('afterend', searchEl)
    // Add Extensions item
    const extensionEl = createMenuElement(
      loadTimeData.getString('presearchDefaultExtensions'),
      '/extensions',
      'presearch_settings:extensions',
      'extensions',
    )
    searchEl.insertAdjacentElement('afterend', extensionEl)

    const walletEl = createMenuElement(
      loadTimeData.getString('presearchWallet'),
      '/wallet',
      'presearch_settings:wallet',
      'wallet',
    )
    extensionEl.insertAdjacentElement('afterend', walletEl)

    const ipfsEl = createMenuElement(
      loadTimeData.getString('presearchIPFS'),
      '/ipfs',
      'presearch_settings:ipfs',
      'ipfs',
    )
    walletEl.insertAdjacentElement('afterend', ipfsEl)

    // Move autofill to advanced
    const autofillEl = getMenuElement(templateContent, '/autofill')
    const languagesEl = getMenuElement(templateContent, '/languages')
    languagesEl.insertAdjacentElement('beforebegin', autofillEl)
    // Move privacy to advanced
    const privacyEl = getMenuElement(templateContent, '/privacy')
    autofillEl.insertAdjacentElement('beforebegin', privacyEl)
    // Move helptips to advanced
    const downloadsEl = getMenuElement(templateContent, '/downloads')
    downloadsEl.insertAdjacentElement('afterend', helpTipsEl)
    // Allow Accessibility to be removed :-(
    const a11yEl = getMenuElement(templateContent, '/accessibility')
    a11yEl.setAttribute('hidden', '[[!pageVisibility.a11y]')
    // Remove extensions link
    const extensionsLinkEl = templateContent.querySelector('#extensionsLink')
    if (!extensionsLinkEl) {
      console.error('[Presearch Settings Overrides] Could not find extensionsLinkEl to remove')
    }
    extensionsLinkEl.remove()
    // Add version number to 'about' link
    const aboutEl = templateContent.querySelector('#about-menu')
    if (!aboutEl) {
      console.error('[Presearch Settings Overrides] Could not find about-menu element')
      return
    }
    const parent = aboutEl.parentNode
    parent.removeChild(aboutEl)

    const newAboutEl = document.createElement('a')
    newAboutEl.setAttribute('href', '/help')
    newAboutEl.setAttribute('id', aboutEl.id)

    const graphicsEl = document.createElement('div')
    graphicsEl.setAttribute('class', 'presearch-about-graphic')

    const icon = document.createElement('iron-icon')
    icon.setAttribute('icon', 'presearch_settings:full-color-presearch-lion')

    const metaEl = document.createElement('div')
    metaEl.setAttribute('class', 'presearch-about-meta')

    const menuLink = document.createElement('span')
    menuLink.setAttribute('class', 'presearch-about-item presearch-about-menu-link-text')
    menuLink.textContent = aboutEl.textContent

    const versionEl = document.createElement('span')
    versionEl.setAttribute('class', 'presearch-about-item presearch-about-menu-version')
    versionEl.textContent = `v ${loadTimeData.getString('presearchProductVersion')}`

    parent.appendChild(newAboutEl)
    newAboutEl.appendChild(graphicsEl)
    graphicsEl.appendChild(icon)
    newAboutEl.appendChild(metaEl)
    metaEl.appendChild(menuLink)
    metaEl.appendChild(versionEl)
  }
})
