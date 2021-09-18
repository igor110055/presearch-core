/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.top;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.SpannableStringBuilder;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.text.style.ImageSpan;
import android.util.AttributeSet;
import android.util.Pair;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.widget.AppCompatImageView;
import androidx.core.content.ContextCompat;
import androidx.core.content.res.ResourcesCompat;

import org.chromium.base.ApiCompatibilityUtils;
import org.chromium.base.PresearchReflectionUtil;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.MathUtils;
import org.chromium.base.ThreadUtils;
import org.chromium.base.supplier.BooleanSupplier;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.PresearchAdsNativeHelper;
import org.chromium.chrome.browser.PresearchFeatureList;
import org.chromium.chrome.browser.PresearchRelaunchUtils;
import org.chromium.chrome.browser.PresearchRewardsHelper;
import org.chromium.chrome.browser.PresearchRewardsNativeWorker;
import org.chromium.chrome.browser.PresearchRewardsObserver;
import org.chromium.chrome.browser.PresearchRewardsPanelPopup;
import org.chromium.chrome.browser.app.PresearchActivity;
import org.chromium.chrome.browser.presearch_stats.PresearchStatsUtil;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltip;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltipUtils;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.customtabs.features.toolbar.CustomTabToolbar;
import org.chromium.chrome.browser.dialogs.PresearchAdsSignupDialog;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.lifecycle.ConfigurationChangedObserver;
import org.chromium.chrome.browser.local_database.PresearchStatsTable;
import org.chromium.chrome.browser.local_database.DatabaseHelper;
import org.chromium.chrome.browser.local_database.SavedBandwidthTable;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.chrome.browser.ntp.PresearchNewTabPageLayout;
import org.chromium.chrome.browser.ntp.NewTabPage;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.SearchActivity;
import org.chromium.chrome.browser.preferences.PresearchPref;
import org.chromium.chrome.browser.preferences.PresearchPrefServiceBridge;
import org.chromium.chrome.browser.preferences.website.PresearchShieldsContentSettings;
import org.chromium.chrome.browser.preferences.website.PresearchShieldsContentSettingsObserver;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.settings.AppearancePreferences;
import org.chromium.chrome.browser.settings.PresearchSearchEngineUtils;
import org.chromium.chrome.browser.shields.PresearchShieldsHandler;
import org.chromium.chrome.browser.shields.PresearchShieldsMenuObserver;
import org.chromium.chrome.browser.shields.PresearchShieldsUtils;
import org.chromium.chrome.browser.shields.ShieldsTooltipEnum;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabImpl;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabModelObserver;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabObserver;
import org.chromium.chrome.browser.theme.ThemeUtils;
import org.chromium.chrome.browser.toolbar.HomeButton;
import org.chromium.chrome.browser.toolbar.ToolbarColors;
import org.chromium.chrome.browser.toolbar.ToolbarDataProvider;
import org.chromium.chrome.browser.toolbar.ToolbarTabController;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarVariationManager;
import org.chromium.chrome.browser.toolbar.menu_button.PresearchMenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.menu_button.MenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.top.ToolbarLayout;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.components.browser_ui.styles.ChromeColors;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.url_formatter.UrlFormatter;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.content_public.browser.NavigationHandle;
import org.chromium.ui.UiUtils;
import org.chromium.ui.interpolators.BakedBezierInterpolator;
import org.chromium.ui.widget.Toast;
import org.chromium.url.GURL;

import java.net.URL;
import java.util.Calendar;
import java.util.Date;
import java.util.EnumSet;
import java.util.List;
import java.util.Locale;

public abstract class PresearchToolbarLayout extends ToolbarLayout
        implements OnClickListener, View.OnLongClickListener, PresearchRewardsObserver,
                   PresearchRewardsNativeWorker.PublisherObserver {
    public static final String PREF_HIDE_PRESEARCH_REWARDS_ICON = "hide_presearch_rewards_icon";
    private static final String JAPAN_COUNTRY_CODE = "JP";

    private static final long MB_10 = 10000000;
    private static final long MINUTES_10 = 10 * 60 * 1000;

    private static final int URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP = 10;

    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private ImageButton mPresearchShieldsButton;
    private ImageButton mPresearchRewardsButton;
    private HomeButton mHomeButton;
    private FrameLayout mShieldsLayout;
    private FrameLayout mRewardsLayout;
    private PresearchShieldsHandler mPresearchShieldsHandler;
    private TabModelSelectorTabObserver mTabModelSelectorTabObserver;
    private TabModelSelectorTabModelObserver mTabModelSelectorTabModelObserver;
    private PresearchRewardsPanelPopup mRewardsPopup;
    private PresearchShieldsContentSettings mPresearchShieldsContentSettings;
    private PresearchShieldsContentSettingsObserver mPresearchShieldsContentSettingsObserver;
    private TextView mPresearchRewardsNotificationsCount;
    private ImageView mPresearchRewardsOnboardingIcon;
    private boolean mShieldsLayoutIsColorBackground;
    private int mCurrentToolbarColor;

    private boolean mIsPublisherVerified;
    private boolean mIsNotificationPosted;
    private boolean mIsInitialNotificationPosted; // initial red circle notification

    private PopupWindowTooltip mShieldsPopupWindowTooltip;
    private PopupWindowTooltip mRewardsPopupWindowTooltip;

    private boolean mIsBottomToolbarVisible;

    public PresearchToolbarLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    void destroy() {
        if (mPresearchShieldsContentSettings != null) {
            mPresearchShieldsContentSettings.removeObserver(mPresearchShieldsContentSettingsObserver);
        }
        super.destroy();
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        if (this instanceof ToolbarTablet) {
            ImageButton forwardButton = findViewById(R.id.forward_button);
            if (forwardButton != null) {
                final Drawable forwardButtonDrawable = UiUtils.getTintedDrawable(getContext(),
                        R.drawable.btn_right_tablet, R.color.default_icon_color_tint_list);
                forwardButton.setImageDrawable(forwardButtonDrawable);
            }
        }

        mShieldsLayout = (FrameLayout) findViewById(R.id.presearch_shields_button_layout);
        mRewardsLayout = (FrameLayout) findViewById(R.id.presearch_rewards_button_layout);
        mPresearchRewardsNotificationsCount = (TextView) findViewById(R.id.br_notifications_count);
        mPresearchRewardsOnboardingIcon = findViewById(R.id.br_rewards_onboarding_icon);
        mPresearchShieldsButton = (ImageButton) findViewById(R.id.presearch_shields_button);
        mPresearchRewardsButton = (ImageButton) findViewById(R.id.presearch_rewards_button);
        mHomeButton = (HomeButton) findViewById(R.id.home_button);

        if (mHomeButton != null) {
            mHomeButton.setOnLongClickListener(this);
        }

        if (mPresearchShieldsButton != null) {
            mPresearchShieldsButton.setClickable(true);
            mPresearchShieldsButton.setOnClickListener(this);
            mPresearchShieldsButton.setOnLongClickListener(this);
        }

        if (mPresearchRewardsButton != null) {
            mPresearchRewardsButton.setClickable(true);
            mPresearchRewardsButton.setOnClickListener(this);
            mPresearchRewardsButton.setOnLongClickListener(this);
        }

        mPresearchShieldsHandler = new PresearchShieldsHandler(getContext());
        mPresearchShieldsHandler.addObserver(new PresearchShieldsMenuObserver() {
            @Override
            public void onMenuTopShieldsChanged(boolean isOn, boolean isTopShield) {
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null) {
                    return;
                }
                if (isTopShield) {
                    updatePresearchShieldsButtonState(currentTab);
                }
                if (currentTab.isLoading()) {
                    currentTab.stopLoading();
                }
                currentTab.reloadIgnoringCache();
                if (null != mPresearchShieldsHandler) {
                    // Clean the Presearchry Panel
                    mPresearchShieldsHandler.updateValues(0, 0, 0, 0);
                }
            }
        });
        mPresearchShieldsContentSettingsObserver = new PresearchShieldsContentSettingsObserver() {
            @Override
            public void blockEvent(int tabId, String block_type, String subresource) {
                mPresearchShieldsHandler.addStat(tabId, block_type, subresource);
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null || currentTab.getId() != tabId) {
                    return;
                }
                mPresearchShieldsHandler.updateValues(tabId);
                if (!isIncognito() && OnboardingPrefManager.getInstance().isPresearchStatsEnabled()
                        && (block_type.equals(PresearchShieldsContentSettings.RESOURCE_IDENTIFIER_ADS)
                                || block_type.equals(PresearchShieldsContentSettings
                                                             .RESOURCE_IDENTIFIER_TRACKERS))) {
                    addStatsToDb(block_type, subresource, currentTab.getUrlString());
                }
            }

            @Override
            public void savedBandwidth(long savings) {
                if (!isIncognito() && OnboardingPrefManager.getInstance().isPresearchStatsEnabled()) {
                    addSavedBandwidthToDb(savings);
                }
            }
        };
        // Initially show shields off image. Shields button state will be updated when tab is
        // shown and loading state is changed.
        updatePresearchShieldsButtonState(null);
        if (PresearchReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)) {
            if (getMenuButtonCoordinator() != null && isMenuButtonOnBottom()) {
                getMenuButtonCoordinator().setVisibility(false);
            }
        }

        if (PresearchReflectionUtil.EqualTypes(this.getClass(), CustomTabToolbar.class)) {
            LinearLayout customActionButtons = findViewById(R.id.action_buttons);
            assert customActionButtons != null : "Something has changed in the upstream!";
            if (customActionButtons != null && mPresearchShieldsButton != null) {
                ViewGroup.MarginLayoutParams presearchShieldsButtonLayout =
                        (ViewGroup.MarginLayoutParams) mPresearchShieldsButton.getLayoutParams();
                ViewGroup.MarginLayoutParams actionButtonsLayout =
                        (ViewGroup.MarginLayoutParams) customActionButtons.getLayoutParams();
                actionButtonsLayout.setMarginEnd(actionButtonsLayout.getMarginEnd()
                        + presearchShieldsButtonLayout.getMarginEnd());
                customActionButtons.setLayoutParams(actionButtonsLayout);
            }
        }
    }

    @Override
    protected void onNativeLibraryReady() {
        super.onNativeLibraryReady();
        mPresearchShieldsContentSettings = PresearchShieldsContentSettings.getInstance();
        mPresearchShieldsContentSettings.addObserver(mPresearchShieldsContentSettingsObserver);

        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        mRewardsLayout.setVisibility(View.GONE);
        if (mShieldsLayout != null) {
            updateShieldsLayoutBackground(
                    !(mRewardsLayout != null && mRewardsLayout.getVisibility() == View.VISIBLE));
            mShieldsLayout.setVisibility(View.VISIBLE);
        }
    }

    @Override
    public void setTabModelSelector(TabModelSelector selector) {
        // We might miss events before calling setTabModelSelector, so we need
        // to proactively update the shields button state here, otherwise shields
        // might sometimes show as disabled while it is actually enabled.
        updatePresearchShieldsButtonState(getToolbarDataProvider().getTab());
        mTabModelSelectorTabObserver = new TabModelSelectorTabObserver(selector) {
            @Override
            public void onShown(Tab tab, @TabSelectionType int type) {
                // Update shields button state when visible tab is changed.
                updatePresearchShieldsButtonState(tab);
            }

            @Override
            public void onPageLoadStarted(Tab tab, GURL url) {
                if (getToolbarDataProvider().getTab() == tab) {
                    updatePresearchShieldsButtonState(tab);
                }
                mPresearchShieldsHandler.clearPresearchShieldsCount(tab.getId());
                dismissShieldsTooltip();
            }

            @Override
            public void onPageLoadFinished(final Tab tab, GURL url) {
                if (getToolbarDataProvider().getTab() == tab) {
                    mPresearchShieldsHandler.updateHost(url.getSpec());
                    updatePresearchShieldsButtonState(tab);

                    Profile mProfile = Profile.getLastUsedRegularProfile();
                    long trackersBlockedCount =
                            PresearchPrefServiceBridge.getInstance().getTrackersBlockedCount(mProfile);
                    long adsBlockedCount =
                            PresearchPrefServiceBridge.getInstance().getAdsBlockedCount(mProfile);
                    long dataSaved = PresearchPrefServiceBridge.getInstance().getDataSaved(mProfile);
                    long estimatedMillisecondsSaved = (trackersBlockedCount + adsBlockedCount)
                            * PresearchStatsUtil.MILLISECONDS_PER_ITEM;

                    if (!OnboardingPrefManager.getInstance().isAdsTrackersNotificationStarted()
                            && (trackersBlockedCount + adsBlockedCount) > 250
                            && PackageUtils.isFirstInstall(getContext())) {
                        RetentionNotificationUtil.scheduleNotification(
                                getContext(), RetentionNotificationUtil.PRESEARCH_STATS_ADS_TRACKERS);
                        OnboardingPrefManager.getInstance().setAdsTrackersNotificationStarted(true);
                    }

                    if (!OnboardingPrefManager.getInstance().isDataSavedNotificationStarted()
                            && dataSaved > MB_10 && PackageUtils.isFirstInstall(getContext())) {
                        RetentionNotificationUtil.scheduleNotification(
                                getContext(), RetentionNotificationUtil.PRESEARCH_STATS_DATA);
                        OnboardingPrefManager.getInstance().setDataSavedNotificationStarted(true);
                    }

                    if (!OnboardingPrefManager.getInstance().isTimeSavedNotificationStarted()
                            && estimatedMillisecondsSaved > MINUTES_10
                            && PackageUtils.isFirstInstall(getContext())) {
                        RetentionNotificationUtil.scheduleNotification(
                                getContext(), RetentionNotificationUtil.PRESEARCH_STATS_TIME);
                        OnboardingPrefManager.getInstance().setTimeSavedNotificationStarted(true);
                    }
                    if (mPresearchShieldsButton != null && mPresearchShieldsButton.isShown()
                            && mPresearchShieldsHandler != null && !mPresearchShieldsHandler.isShowing()
                            && !isRewardsTooltipShown() && !isRewardsPanelOpened()) {
                        checkForTooltip(tab);
                    }
                }
            }

            @Override
            public void onDidFinishNavigation(Tab tab, NavigationHandle navigation) {
            }

            @Override
            public void onDestroyed(Tab tab) {
                mPresearchShieldsHandler.removeStat(tab.getId());
            }
        };
    }

    private void checkForTooltip(Tab tab) {
        if (!PresearchShieldsUtils.isTooltipShown) {
            if (!PresearchShieldsUtils.hasShieldsTooltipShown(PresearchShieldsUtils.PREF_SHIELDS_TOOLTIP)
                    && mPresearchShieldsHandler.getTrackersBlockedCount(tab.getId())
                                    + mPresearchShieldsHandler.getAdsBlockedCount(tab.getId())
                            > 0) {
                showTooltip(ShieldsTooltipEnum.ONE_TIME_ADS_TRACKER_BLOCKED_TOOLTIP,
                        PresearchShieldsUtils.PREF_SHIELDS_TOOLTIP);
            } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                               PresearchShieldsUtils.PREF_SHIELDS_VIDEO_ADS_BLOCKED_TOOLTIP)
                    && shouldShowVideoTooltip(tab.getUrlString())) {
                showTooltip(ShieldsTooltipEnum.VIDEO_ADS_BLOCKED_TOOLTIP,
                        PresearchShieldsUtils.PREF_SHIELDS_VIDEO_ADS_BLOCKED_TOOLTIP);
            } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                               PresearchShieldsUtils.PREF_SHIELDS_ADS_TRACKER_BLOCKED_TOOLTIP)
                    && mPresearchShieldsHandler.getTrackersBlockedCount(tab.getId())
                                    + mPresearchShieldsHandler.getAdsBlockedCount(tab.getId())
                            > 10) {
                showTooltip(ShieldsTooltipEnum.ADS_TRACKER_BLOCKED_TOOLTIP,
                        PresearchShieldsUtils.PREF_SHIELDS_ADS_TRACKER_BLOCKED_TOOLTIP);
            } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                               PresearchShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP)
                    && mPresearchShieldsHandler.getHttpsUpgradeCount(tab.getId()) > 0) {
                showTooltip(ShieldsTooltipEnum.HTTPS_UPGRADE_TOOLTIP,
                        PresearchShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP);
            } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                               PresearchShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP)
                    && mPresearchShieldsHandler.getHttpsUpgradeCount(tab.getId()) > 0) {
                showTooltip(ShieldsTooltipEnum.HTTPS_UPGRADE_TOOLTIP,
                        PresearchShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP);
            } else {
                int trackersPlusAdsBlocked =
                        mPresearchShieldsHandler.getTrackersBlockedCount(tab.getId())
                        + mPresearchShieldsHandler.getAdsBlockedCount(tab.getId());
                chooseStatsShareTier(tab, trackersPlusAdsBlocked);
            }
        }
    }

    private void chooseStatsShareTier(Tab tab, int trackersPlusAdsBlocked) {
        String countryCode = Locale.getDefault().getCountry();
        // the tooltip for stats sharing is shown only for Japan
        if (!countryCode.equals(JAPAN_COUNTRY_CODE)) {
            return;
        }
        int totalBlocked = Integer.parseInt(PresearchStatsUtil.getAdsTrackersBlocked().first);

        // show after PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF (20) blocked stuff above the TIER
        // threshold
        if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER1)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER1
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER2)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER1_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER1);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER2)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER2
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER3)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER2_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER2);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER3)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER3
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER4)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER3_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER3);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER4)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER4
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER5)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER4_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER4);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER5)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER5
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER6)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER5_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER5);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER6)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER6
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER7)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER6_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER6);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER7)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER7
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER8)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER7_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER7);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER8)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER8
                                        + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF
                        && totalBlocked < PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER9)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER8_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER8);
        } else if (!PresearchShieldsUtils.hasShieldsTooltipShown(
                           PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER9)
                && (totalBlocked >= PresearchShieldsUtils.PRESEARCH_BLOCKED_TIER9
                                + PresearchShieldsUtils.PRESEARCH_BLOCKED_SHOW_DIFF)) {
            showTooltip(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER9_TOOLTIP,
                    PresearchShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER9);
        }
    }

    private boolean shouldShowVideoTooltip(String tabUrl) {
        try {
            URL url = new URL(tabUrl);
            for (String videoUrl : PresearchShieldsUtils.videoSitesList) {
                if (url.getHost().contains(videoUrl)) {
                    return true;
                }
            }
            String countryCode = Locale.getDefault().getCountry();
            if (countryCode.equals(JAPAN_COUNTRY_CODE)) {
                for (String videoUrl : PresearchShieldsUtils.videoSitesListJp) {
                    if (url.getHost().contains(videoUrl)) {
                        return true;
                    }
                }
            }
            return false;
        } catch (Exception ex) {
            // Do nothing if url is invalid.
            return false;
        }
    }

    private EnumSet<ShieldsTooltipEnum> getStatsSharingEnums() {
        return EnumSet.of(ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER1_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER2_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER3_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER4_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER5_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER6_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER7_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER8_TOOLTIP,
                ShieldsTooltipEnum.PRESEARCH_SHARE_STATS_TIER9_TOOLTIP);
    }

    private void showTooltip(ShieldsTooltipEnum shieldsTooltipEnum, String tooltipPref) {
        mShieldsPopupWindowTooltip = new PopupWindowTooltip.Builder(getContext())
                                             .anchorView(mPresearchShieldsButton)
                                             .arrowColor(getContext().getResources().getColor(
                                                     shieldsTooltipEnum.getArrowColor()))
                                             .gravity(Gravity.BOTTOM)
                                             .dismissOnOutsideTouch(true)
                                             .dismissOnInsideTouch(false)
                                             .modal(true)
                                             .contentView(R.layout.presearch_shields_tooltip_layout)
                                             .build();
        mShieldsPopupWindowTooltip.findViewById(R.id.shields_tooltip_layout)
                .setBackgroundDrawable(ContextCompat.getDrawable(
                        getContext(), shieldsTooltipEnum.getTooltipBackground()));

        if (shieldsTooltipEnum == ShieldsTooltipEnum.ONE_TIME_ADS_TRACKER_BLOCKED_TOOLTIP) {
            Button btnTooltip = mShieldsPopupWindowTooltip.findViewById(R.id.btn_tooltip);
            btnTooltip.setVisibility(View.VISIBLE);
            btnTooltip.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    dismissShieldsTooltip();
                    showShieldsMenu(mPresearchShieldsButton);
                }
            });
        } else if (getStatsSharingEnums().contains(shieldsTooltipEnum)) {
            Button btnTooltip = mShieldsPopupWindowTooltip.findViewById(R.id.btn_tooltip);

            SpannableStringBuilder shareStringBuilder = new SpannableStringBuilder();
            shareStringBuilder
                    .append(getContext().getResources().getString(
                            R.string.presearch_stats_share_button))
                    .append("  ");
            shareStringBuilder.setSpan(new ImageSpan(getContext(), R.drawable.ic_share_white),
                    shareStringBuilder.length() - 1, shareStringBuilder.length(), 0);
            btnTooltip.setText(shareStringBuilder, TextView.BufferType.SPANNABLE);

            btnTooltip.setVisibility(View.VISIBLE);

            btnTooltip.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    dismissShieldsTooltip();
                    if (PresearchStatsUtil.hasWritePermission(PresearchActivity.getPresearchActivity())) {
                        PresearchStatsUtil.shareStats(R.layout.presearch_stats_share_layout);
                    }
                }
            });
        }

        TextView tooltipTitle = mShieldsPopupWindowTooltip.findViewById(R.id.txt_tooltip_title);
        SpannableStringBuilder ssb =
                new SpannableStringBuilder(new StringBuilder("\t\t")
                                                   .append(getContext().getResources().getString(
                                                           shieldsTooltipEnum.getTitle()))
                                                   .toString());
        ssb.setSpan(new ImageSpan(getContext(), R.drawable.ic_shield_done_filled_20dp), 0, 1,
                Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        tooltipTitle.setText(ssb, TextView.BufferType.SPANNABLE);

        TextView tooltipText = mShieldsPopupWindowTooltip.findViewById(R.id.txt_tooltip_text);
        tooltipText.setText(getContext().getResources().getString(shieldsTooltipEnum.getText()));

        if (mPresearchShieldsButton != null && mPresearchShieldsButton.isShown()) {
            mShieldsPopupWindowTooltip.show();
            PresearchShieldsUtils.setShieldsTooltipShown(tooltipPref, true);
            PresearchShieldsUtils.isTooltipShown = true;
        }
    }

    public void showRewardsTooltip() {
        ShieldsTooltipEnum shieldsTooltipEnum = ShieldsTooltipEnum.BAP_DEPRECATION_TOOLTIP;
        mRewardsPopupWindowTooltip = new PopupWindowTooltip.Builder(getContext())
                                             .anchorView(mPresearchRewardsButton)
                                             .arrowColor(getContext().getResources().getColor(
                                                     shieldsTooltipEnum.getArrowColor()))
                                             .gravity(Gravity.BOTTOM)
                                             .dismissOnOutsideTouch(true)
                                             .dismissOnInsideTouch(false)
                                             .modal(true)
                                             .contentView(R.layout.presearch_shields_tooltip_layout)
                                             .build();
        mRewardsPopupWindowTooltip.findViewById(R.id.shields_tooltip_layout)
                .setBackgroundDrawable(ContextCompat.getDrawable(
                        getContext(), shieldsTooltipEnum.getTooltipBackground()));

        Button btnTooltip = mRewardsPopupWindowTooltip.findViewById(R.id.btn_tooltip);
        btnTooltip.setText(getContext().getResources().getString(R.string.menu_learn_more));
        btnTooltip.setVisibility(View.VISIBLE);
        btnTooltip.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dismissRewardsTooltip();
                if (PresearchActivity.getPresearchActivity() != null)
                    PresearchActivity.getPresearchActivity().showDeprecateBAPDialog();
            }
        });

        TextView tooltipTitle = mRewardsPopupWindowTooltip.findViewById(R.id.txt_tooltip_title);
        SpannableStringBuilder ssb =
                new SpannableStringBuilder(new StringBuilder("\t\t")
                                                   .append(getContext().getResources().getString(
                                                           shieldsTooltipEnum.getTitle()))
                                                   .toString());
        ssb.setSpan(new ImageSpan(
                            getContext(), R.drawable.ic_warning_triangle, ImageSpan.ALIGN_BASELINE),
                0, 1, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        tooltipTitle.setText(ssb, TextView.BufferType.SPANNABLE);

        TextView tooltipText = mRewardsPopupWindowTooltip.findViewById(R.id.txt_tooltip_text);
        tooltipText.setText(getContext().getResources().getString(shieldsTooltipEnum.getText()));

        mRewardsPopupWindowTooltip.show();
    }

    public void dismissShieldsTooltip() {
        if (mShieldsPopupWindowTooltip != null && mShieldsPopupWindowTooltip.isShowing()) {
            mShieldsPopupWindowTooltip.dismiss();
            mShieldsPopupWindowTooltip = null;
        }
    }

    public void reopenShieldsPanel() {
        if (mPresearchShieldsHandler != null && mPresearchShieldsHandler.isShowing()) {
            mPresearchShieldsHandler.hidePresearchShieldsMenu();
            showShieldsMenu(mPresearchShieldsButton);
        }
    }

    public void dismissRewardsTooltip() {
        if (mRewardsPopupWindowTooltip != null && mRewardsPopupWindowTooltip.isShowing()) {
            mRewardsPopupWindowTooltip.dismiss();
            mRewardsPopupWindowTooltip = null;
        }
    }

    public boolean isRewardsTooltipShown() {
        if (mRewardsPopupWindowTooltip != null) {
            return mRewardsPopupWindowTooltip.isShowing();
        }
        return false;
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        dismissRewardsTooltip();
        dismissShieldsTooltip();
        reopenShieldsPanel();
    }

    private void showPresearchRewardsOnboardingModal() {
        Context context = getContext();
        final Dialog dialog = new Dialog(context);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCancelable(false);
        dialog.setContentView(R.layout.presearch_rewards_onboarding_modal);
        dialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);

        View presearchRewardsOnboardingModalView =
                dialog.findViewById(R.id.presearch_rewards_onboarding_modal_layout);
        presearchRewardsOnboardingModalView.setBackgroundColor(
                context.getResources().getColor(android.R.color.white));
        presearchRewardsOnboardingModalView.setVisibility(View.VISIBLE);

        String tosText =
                String.format(context.getResources().getString(R.string.presearch_rewards_tos_text),
                        context.getResources().getString(R.string.terms_of_service),
                        context.getResources().getString(R.string.privacy_policy));
        int termsOfServiceIndex =
                tosText.indexOf(context.getResources().getString(R.string.terms_of_service));
        Spanned tosTextSpanned = PresearchRewardsHelper.spannedFromHtmlString(tosText);
        SpannableString tosTextSS = new SpannableString(tosTextSpanned.toString());

        ClickableSpan tosClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(context, PresearchActivity.PRESEARCH_TERMS_PAGE);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        tosTextSS.setSpan(tosClickableSpan, termsOfServiceIndex,
                termsOfServiceIndex
                        + context.getResources().getString(R.string.terms_of_service).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tosTextSS.setSpan(new ForegroundColorSpan(context.getResources().getColor(
                                  R.color.presearch_rewards_modal_theme_color)),
                termsOfServiceIndex,
                termsOfServiceIndex
                        + context.getResources().getString(R.string.terms_of_service).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        ClickableSpan privacyProtectionClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(context, PresearchActivity.PRESEARCH_PRIVACY_POLICY);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        int privacyPolicyIndex =
                tosText.indexOf(context.getResources().getString(R.string.privacy_policy));
        tosTextSS.setSpan(privacyProtectionClickableSpan, privacyPolicyIndex,
                privacyPolicyIndex
                        + context.getResources().getString(R.string.privacy_policy).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tosTextSS.setSpan(new ForegroundColorSpan(context.getResources().getColor(
                                  R.color.presearch_rewards_modal_theme_color)),
                privacyPolicyIndex,
                privacyPolicyIndex
                        + context.getResources().getString(R.string.privacy_policy).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        TextView tosAndPpText = presearchRewardsOnboardingModalView.findViewById(
                R.id.presearch_rewards_onboarding_modal_tos_pp_text);
        tosAndPpText.setMovementMethod(LinkMovementMethod.getInstance());
        tosAndPpText.setText(tosTextSS);

        TextView takeQuickTourButton =
                presearchRewardsOnboardingModalView.findViewById(R.id.take_quick_tour_button);
        takeQuickTourButton.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                PresearchRewardsHelper.setShowPresearchRewardsOnboardingOnce(true);
                openRewardsPanel();
                dialog.dismiss();
            }
        }));
        Button btnPresearchRewards =
                presearchRewardsOnboardingModalView.findViewById(R.id.btn_presearch_rewards);
        btnPresearchRewards.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                PresearchAdsNativeHelper.nativeSetAdsEnabled(Profile.getLastUsedRegularProfile());
                PresearchRewardsNativeWorker.getInstance().SetAutoContributeEnabled(true);
                dialog.dismiss();
            }
        }));
        AppCompatImageView modalCloseButton = presearchRewardsOnboardingModalView.findViewById(
                R.id.presearch_rewards_onboarding_modal_close);
        modalCloseButton.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }
        }));

        dialog.show();
    }

    private void addSavedBandwidthToDb(long savings) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    SavedBandwidthTable savedBandwidthTable = new SavedBandwidthTable(
                            savings, PresearchStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long rowId = mDatabaseHelper.insertSavedBandwidth(savedBandwidthTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void addStatsToDb(String statType, String statSite, String url) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    URL urlObject = new URL(url);
                    URL siteObject = new URL(statSite);
                    PresearchStatsTable presearchStatsTable = new PresearchStatsTable(url, urlObject.getHost(),
                            statType, statSite, siteObject.getHost(),
                            PresearchStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long rowId = mDatabaseHelper.insertStats(presearchStatsTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public void hideRewardsOnboardingIcon() {
        if (mPresearchRewardsOnboardingIcon != null) {
            mPresearchRewardsOnboardingIcon.setVisibility(View.GONE);
        }
        if (mPresearchRewardsNotificationsCount != null) {
            mPresearchRewardsNotificationsCount.setVisibility(View.GONE);
        }
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putBoolean(PresearchRewardsPanelPopup.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
        editor.apply();
    }

    @Override
    public void onClick(View v) {
        onClickImpl(v);
    }

    public void onClickImpl(View v) {
        if (mPresearchShieldsHandler == null) {
            assert false;
            return;
        }
        if (mPresearchShieldsButton == v && mPresearchShieldsButton != null) {
            showShieldsMenu(mPresearchShieldsButton);
        } else if (mPresearchRewardsButton == v && mPresearchRewardsButton != null) {
            // Context context = getContext();
            // if (checkForRewardsOnboarding()) {
            //   OnboardingPrefManager.getInstance().showOnboarding(context);
            //   hideRewardsOnboardingIcon();
            // } else {
            //   if (null != mRewardsPopup) {
            //     return;
            //   }
            //   mRewardsPopup = new PresearchRewardsPanelPopup(v);
            //   mRewardsPopup.showLikePopDownMenu();
            // }
            if (null != mRewardsPopup) {
                return;
            }
            hideRewardsOnboardingIcon();
            OnboardingPrefManager.getInstance().setOnboardingShown(true);
            mRewardsPopup = new PresearchRewardsPanelPopup(v);
            mRewardsPopup.showLikePopDownMenu();
            if (mPresearchRewardsNotificationsCount.isShown()) {
                SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
                SharedPreferences.Editor editor = sharedPref.edit();
                editor.putBoolean(
                        PresearchRewardsPanelPopup.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
                editor.apply();
                mPresearchRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsInitialNotificationPosted = false;
            }
        }
    }

    private boolean checkForRewardsOnboarding() {
        return PackageUtils.isFirstInstall(getContext())
                && !PresearchAdsNativeHelper.nativeIsPresearchAdsEnabled(
                        Profile.getLastUsedRegularProfile())
                && ChromeFeatureList.isEnabled(PresearchFeatureList.PRESEARCH_REWARDS)
                && !OnboardingPrefManager.getInstance().isOnboardingShown();
    }

    private void showShieldsMenu(View mPresearchShieldsButton) {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null) {
            return;
        }
        try {
            URL url = new URL(currentTab.getUrlString());
            // Don't show shields popup if protocol is not valid for shields.
            if (!isValidProtocolForShields(url.getProtocol())) {
                return;
            }
            mPresearchShieldsHandler.show(mPresearchShieldsButton, currentTab);
        } catch (Exception e) {
            // Do nothing if url is invalid.
            // Just return w/o showing shields popup.
            return;
        }
    }

    @Override
    public boolean onLongClick(View v) {
        // Use null as the default description since Toast.showAnchoredToast
        // will return false if it is null.
        String description = null;
        Context context = getContext();
        Resources resources = context.getResources();

        if (v == mPresearchShieldsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_presearch_shields);
        } else if (v == mPresearchRewardsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_presearch_rewards);
        } else if (v == mHomeButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_home);
        }

        return Toast.showAnchoredToast(context, v, description);
    }

    @Override
    public void onUrlFocusChange(boolean hasFocus) {
        Context context = getContext();
        if (hasFocus && PackageUtils.isFirstInstall(context)
                && PresearchActivity.getPresearchActivity() != null
                && PresearchActivity.getPresearchActivity().getActivityTab() != null
                && UrlUtilities.isNTPUrl(
                        PresearchActivity.getPresearchActivity().getActivityTab().getUrlString())
                && !OnboardingPrefManager.getInstance().hasSearchEngineOnboardingShown()
                && !PresearchSearchEngineUtils.getDSEShortName(true).equals(
                        OnboardingPrefManager.DUCKDUCKGO)) {
            Intent searchActivityIntent = new Intent(context, SearchActivity.class);
            context.startActivity(searchActivityIntent);
        }
        super.onUrlFocusChange(hasFocus);
    }

    public void populateUrlAnimatorSet(boolean showExpandedState,
            int urlFocusToolbarButtonsDuration, int urlClearFocusTabStackDelayMs,
            List<Animator> animators) {
        if (mPresearchShieldsButton != null) {
            Animator animator;
            if (showExpandedState) {
                float density = getContext().getResources().getDisplayMetrics().density;
                boolean isRtl = getLayoutDirection() == LAYOUT_DIRECTION_RTL;
                float toolbarButtonTranslationX =
                        MathUtils.flipSignIf(URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP, isRtl)
                        * density;
                animator = ObjectAnimator.ofFloat(
                        mPresearchShieldsButton, TRANSLATION_X, toolbarButtonTranslationX);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(BakedBezierInterpolator.FADE_OUT_CURVE);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(mPresearchShieldsButton, ALPHA, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(BakedBezierInterpolator.FADE_OUT_CURVE);
                animators.add(animator);
            } else {
                animator = ObjectAnimator.ofFloat(mPresearchShieldsButton, TRANSLATION_X, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(BakedBezierInterpolator.TRANSFORM_CURVE);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(mPresearchShieldsButton, ALPHA, 1);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(BakedBezierInterpolator.TRANSFORM_CURVE);
                animators.add(animator);
            }
        }
    }

    private void updateModernLocationBarColor(int color) {
        updateModernLocationBarColorImpl(color);
    }

    public void updateModernLocationBarColorImpl(int color) {
        if (mShieldsLayout != null && mShieldsLayoutIsColorBackground) {
            mShieldsLayout.setBackgroundColor(
                    ChromeColors.getDefaultThemeColor(getContext().getResources(), isIncognito()));
        }
        mCurrentToolbarColor = color;
        if (mShieldsLayout != null) {
            mShieldsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
        if (mRewardsLayout != null) {
            mRewardsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
    }

    public int getBoundsAfterAccountingForRightButtons(ViewGroup toolbarButtonsContainer) {
        if (toolbarButtonsContainer == null || mShieldsLayout == null) {
            assert false;
            return 0;
        }
        ViewGroup.MarginLayoutParams params =
                (ViewGroup.MarginLayoutParams) toolbarButtonsContainer.getLayoutParams();

        int rewardsLen = (mRewardsLayout == null || mRewardsLayout.getVisibility() == View.GONE)
                ? 0
                : mRewardsLayout.getWidth();
        return toolbarButtonsContainer.getMeasuredWidth() - mShieldsLayout.getWidth() - rewardsLen
                + params.getMarginEnd();
    }

    /**
     * If |tab| is null, set disabled image to shields button and |urlString| is
     * ignored.
     * If |urlString| is null, url is fetched from |tab|.
     */
    private void updatePresearchShieldsButtonState(Tab tab) {
        if (mPresearchShieldsButton == null) {
            assert false;
            return;
        }

        if (tab == null) {
            mPresearchShieldsButton.setImageResource(R.drawable.btn_presearch_off);
            return;
        }
        mPresearchShieldsButton.setImageResource(
                isShieldsOnForTab(tab) ? R.drawable.btn_presearch : R.drawable.btn_presearch_off);

        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();

        if (mRewardsLayout == null) return;
        if (isIncognito()) {
            updateShieldsLayoutBackground(true);
        } else if (isNativeLibraryReady()
                && ChromeFeatureList.isEnabled(PresearchFeatureList.PRESEARCH_REWARDS)
                && !PresearchPrefServiceBridge.getInstance().getSafetynetCheckFailed()
                && !sharedPreferences.getBoolean(
                        AppearancePreferences.PREF_HIDE_PRESEARCH_REWARDS_ICON, false)) {
            updateShieldsLayoutBackground(false);
        }
    }

    private boolean isShieldsOnForTab(Tab tab) {
        if (tab == null) {
            assert false;
            return false;
        }
        return PresearchShieldsContentSettings.getShields(
                Profile.fromWebContents(((TabImpl) tab).getWebContents()), tab.getUrlString(),
                PresearchShieldsContentSettings.RESOURCE_IDENTIFIER_PRESEARCH_SHIELDS);
    }

    private boolean isValidProtocolForShields(String protocol) {
        if (protocol.equals("http") || protocol.equals("https")) {
            return true;
        }

        return false;
    }

    public void dismissRewardsPanel() {
        if (mRewardsPopup != null) {
            mRewardsPopup.dismiss();
            mRewardsPopup = null;
        }
    }

    public void onRewardsPanelDismiss() {
        mRewardsPopup = null;
    }

    public void openRewardsPanel() {
        onClick(mPresearchRewardsButton);
    }

    public boolean isRewardsPanelOpened() {
        if (mRewardsPopup != null) {
            return mRewardsPopup.isShowing();
        }
        return false;
    }

    public boolean isShieldsTooltipShown() {
        if (mShieldsPopupWindowTooltip != null) {
            return mShieldsPopupWindowTooltip.isShowing();
        }
        return false;
    }

    @Override
    public void OnNotificationAdded(String id, int type, long timestamp, String[] args) {
    }

    private boolean mayShowPresearchAdsOnboardingDialog() {
        Context context = getContext();

        if (PresearchAdsSignupDialog.shouldShowNewUserDialog(context)) {
            PresearchAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (PresearchAdsSignupDialog.shouldShowNewUserDialogIfRewardsIsSwitchedOff(context)) {
            PresearchAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (PresearchAdsSignupDialog.shouldShowExistingUserDialog(context)) {
            PresearchAdsSignupDialog.showExistingUserDialog(getContext());
            return true;
        }

        return false;
    }

    @Override
    public void OnNotificationsCount(int count) {
    }

    private void updateNotificationBadgeForNewInstall() {
    }

    @Override
    public void onThemeColorChanged(int color, boolean shouldAnimate) {
        final int textBoxColor = ThemeUtils.getTextBoxColorForToolbarBackgroundInNonNativePage(
                getContext().getResources(), color, isIncognito());
        updateModernLocationBarColor(textBoxColor);
    }

    /**
     * PresearchRewardsNativeWorker.PublisherObserver:
     *   Update a 'verified publisher' checkmark on url bar BAT icon only if
     *   no notifications are posted.
     */
    @Override
    public void onFrontTabPublisherChanged(boolean verified) {
        mIsPublisherVerified = verified;
        updateVerifiedPublisherMark();
    }

    private void updateVerifiedPublisherMark() {
        if (mPresearchRewardsNotificationsCount == null) {
            // Most likely we are on a custom page
            return;
        }
    }

    public void onBottomToolbarVisibilityChanged(boolean isVisible) {
        mIsBottomToolbarVisible = isVisible;
        if (PresearchReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)
                && getMenuButtonCoordinator() != null) {
            getMenuButtonCoordinator().setVisibility(!isVisible);
            ToggleTabStackButton toggleTabStackButton = findViewById(R.id.tab_switcher_button);
            if (toggleTabStackButton != null) {
                toggleTabStackButton.setVisibility(isTabSwitcherOnBottom() ? GONE : VISIBLE);
            }
        }
    }

    private void updateShieldsLayoutBackground(boolean rounded) {
        if (!(this instanceof ToolbarTablet) || (mShieldsLayout == null)) return;

        if (rounded) {
            mShieldsLayout.setBackgroundDrawable(
                    ApiCompatibilityUtils.getDrawable(getContext().getResources(),
                            R.drawable.modern_toolbar_background_grey_end_segment));
            mShieldsLayoutIsColorBackground = false;
        } else {
            mShieldsLayout.setBackgroundColor(
                    ChromeColors.getDefaultThemeColor(getContext().getResources(), isIncognito()));
            mShieldsLayoutIsColorBackground = true;
        }
        updateModernLocationBarColor(mCurrentToolbarColor);
    }

    private boolean isTabSwitcherOnBottom() {
        return mIsBottomToolbarVisible && BottomToolbarVariationManager.isTabSwitcherOnBottom();
    }

    private boolean isMenuButtonOnBottom() {
        return mIsBottomToolbarVisible && BottomToolbarVariationManager.isMenuButtonOnBottom();
    }

    @Override
    protected void initialize(ToolbarDataProvider toolbarDataProvider,
            ToolbarTabController tabController, MenuButtonCoordinator menuButtonCoordinator,
            BooleanSupplier isInVrSupplier) {
        super.initialize(toolbarDataProvider, tabController, menuButtonCoordinator, isInVrSupplier);
        PresearchMenuButtonCoordinator.setMenuFromBottom(isMenuButtonOnBottom());
    }

    public void updateMenuButtonState() {
        PresearchMenuButtonCoordinator.setMenuFromBottom(mIsBottomToolbarVisible);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (PresearchReflectionUtil.EqualTypes(this.getClass(), CustomTabToolbar.class)
                || PresearchReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)) {
            updateMenuButtonState();
            Tab tab = getToolbarDataProvider() != null ? getToolbarDataProvider().getTab() : null;
            if (tab != null && ((TabImpl) tab).getWebContents() != null) {
                updatePresearchShieldsButtonState(tab);
            }
        }
        super.onDraw(canvas);
    }
}