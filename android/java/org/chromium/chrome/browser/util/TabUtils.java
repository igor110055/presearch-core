/**
 * Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.util;

import android.app.Activity;
import android.content.Context;
import android.view.ContextThemeWrapper;
import android.view.MenuItem;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.PopupMenu;

import org.chromium.base.ApplicationStatus;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.app.ChromeActivity;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.content_public.browser.LoadUrlParams;

public class TabUtils {
    public static void showTabPopupMenu(Context context, View view) {
        PresearchActivity presearchActivity = PresearchActivity.getPresearchActivity();
        Context wrapper = new ContextThemeWrapper(context,
                GlobalNightModeStateProviderHolder.getInstance().isInNightMode()
                        ? R.style.NewTabPopupMenuDark
                        : R.style.NewTabPopupMenuLight);
        // Creating the instance of PopupMenu
        PopupMenu popup = new PopupMenu(wrapper, view);
        // Inflating the Popup using xml file
        popup.getMenuInflater().inflate(R.menu.new_tab_menu, popup.getMenu());

        if (presearchActivity != null && presearchActivity.getCurrentTabModel().isIncognito()) {
            popup.getMenu().findItem(R.id.new_tab_menu_id).setVisible(false);
        }
        // registering popup with OnMenuItemClickListener
        popup.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                int id = item.getItemId();
                if (id == R.id.new_tab_menu_id) {
                    openNewTab(presearchActivity, false);
                } else if (id == R.id.new_incognito_tab_menu_id) {
                    openNewTab(presearchActivity, true);
                }
                return true;
            }
        });
        popup.show(); // showing popup menu
    }

    public static void openNewTab() {
        PresearchActivity presearchActivity = PresearchActivity.getPresearchActivity();
        boolean isIncognito =
                presearchActivity != null ? presearchActivity.getCurrentTabModel().isIncognito() : false;
        openNewTab(presearchActivity, isIncognito);
    }

    private static void openNewTab(PresearchActivity presearchActivity, boolean isIncognito) {
        if (presearchActivity == null) return;
        presearchActivity.getTabModelSelector().getModel(isIncognito).commitAllTabClosures();
        presearchActivity.getTabCreator(isIncognito).launchNTP();
    }

    public static void openUrlInNewTab(boolean isIncognito, String url) {
        PresearchActivity presearchActivity = PresearchActivity.getPresearchActivity();
        if (presearchActivity != null) {
            presearchActivity.getTabCreator(isIncognito).launchUrl(url, TabLaunchType.FROM_CHROME_UI);
        }
    }

    public static void openUrlInSameTab(String url) {
        PresearchActivity presearchActivity = PresearchActivity.getPresearchActivity();
        if (presearchActivity != null) {
            LoadUrlParams loadUrlParams = new LoadUrlParams(url);
            presearchActivity.getActivityTab().loadUrl(loadUrlParams);
        }
    }

    public static void enableRewardsButton() {
        PresearchActivity presearchActivity = PresearchActivity.getPresearchActivity();
        if (presearchActivity == null || presearchActivity.getToolbarManager() == null) {
            return;
        }
        View toolbarView = presearchActivity.findViewById(R.id.toolbar);
        if (toolbarView == null) {
            return;
        }
        FrameLayout rewardsLayout = toolbarView.findViewById(R.id.presearch_rewards_button_layout);
        if (rewardsLayout == null) {
            return;
        }
        rewardsLayout.setVisibility(View.VISIBLE);
    }
}
