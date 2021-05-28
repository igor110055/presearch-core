/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import org.chromium.base.CommandLine;
import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.flags.ChromeSwitches;
import org.chromium.chrome.browser.firstrun.FirstRunStatus;

public class PresearchHelper {
  public static final String SHARED_PREF_DISPLAYED_INFOBAR_PROMO =
            "displayed_data_reduction_infobar_promo";
  // Used to indicate were the settings migrated to the new
  // presearch-core based version
  public static final String PREF_TABS_SETTINGS_MIGRATED =
            "android_tabs_settings_to_core_migrated";
  // New search engines settings
  public static final String PRIVATE_DSE_SHORTNAME = "private_dse_shortname";
  public static final String STANDARD_DSE_SHORTNAME = "standard_dse_shortname";

  public PresearchHelper() {}

  public static void DisableFREDRP() {
      CommandLine.getInstance().appendSwitch(ChromeSwitches.DISABLE_FIRST_RUN_EXPERIENCE);
      FirstRunStatus.setFirstRunFlowComplete(true);
      
      // Disables data reduction promo dialog
      ContextUtils.getAppSharedPreferences()
              .edit()
              .putBoolean(SHARED_PREF_DISPLAYED_INFOBAR_PROMO, true)
              .apply();
  }
}
