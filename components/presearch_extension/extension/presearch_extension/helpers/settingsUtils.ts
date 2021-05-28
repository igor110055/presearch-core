
/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/. */

import { SettingsOptions, SettingsKey } from '../types/other/settingsTypes'

export const settingsOptions: SettingsOptions = {
  'presearch.shields.stats_badge_visible': 'statsBadgeVisible',
  'presearch.shields.advanced_view_enabled': 'showAdvancedView'
}

export const settingsKeyList: Array<SettingsKey> = [
  'presearch.shields.stats_badge_visible',
  'presearch.shields.advanced_view_enabled'
]
