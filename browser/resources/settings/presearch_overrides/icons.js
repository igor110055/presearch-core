// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {OverrideIronIcons} from 'chrome://presearch-resources/polymer_overriding.js'

import '../icons.m.js'
import '../presearch_icons.m.js'

OverrideIronIcons('settings', 'presearch_settings', {
  palette: 'appearance',
  assignment: 'autofill',
  language: 'language',
  build: 'system',
  restore: 'reset-settings'
})
OverrideIronIcons('cr', 'presearch_settings', {
  security: 'privacy-security',
  search: 'search-engine',
  ['file-download']: 'download',
  print: 'printing'
})
OverrideIronIcons('settings20', 'presearch_settings20', {
  incognito: 'private-mode'
})
