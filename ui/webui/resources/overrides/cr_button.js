// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'

export default html`
  <style>
    /* Default/light theme button colors */
    :host(:not([style])) {
      --presearch-text-color: #3b3e4f; /* grey800 */
      --presearch-brand-color: #2d8eff; /* orange400 */
      --presearch-primary-hover: #ff977d; /* orange300 */
      --presearch-default-hover: rgba(45, 142, 255, 0.1); /* orange400 */
      --presearch-focus-outline: rgba(45, 142, 255, 0.4); /* orange400 */
      --ink-color: var(--presearch-brand-color) !important;
      --text-color: var(--presearch-text-color) !important;
      --text-color-action: white !important;
      --border-color: #c2c4cf !important; /* grey 400 */
      --active-shadow-rgb: 255,118,84 !important;
      --active-shadow-action-rgb: 255,118,84 !important;
      --bg-action: var(--presearch-brand-color) !important;
      /* TODO: Disabled colors:
      --disabled-bg-action: var(--google-grey-refresh-100);
      --disabled-bg: white;
      --disabled-border-color: var(--google-grey-refresh-100); */
      --hover-bg-action: var(--presearch-primary-hover) !important;
      --hover-bg-color: none !important;
      --hover-border-color: var(--presearch-brand-color) !important;
      --hover-shadow-action-rgb: 255, 118, 84 !important;
      --ink-color-action: white !important;
      font-family: Muli !important;
      outline: none !important;
      border-radius: 100px !important; /* absurdly large to make sure is
                                          rounded at any height */
    }
    :host(:not([style]):hover) {
      --text-color: var(--presearch-brand-color) !important;
      --border-color: var(--text-color) !important;
    }
    /* Dark theme colors */
    @media (prefers-color-scheme: dark) {
      :host(:not([style])) {
        --presearch-text-color: #ffffff;
        --border-color: #5e6175 !important; /* grey 700 */
      }
    }
    :host-context(.focus-outline-visible):host(:not([style]):focus) {
      box-shadow: 0 0 0 3px var(--presearch-focus-outline) !important;
    }
  </style>
`
