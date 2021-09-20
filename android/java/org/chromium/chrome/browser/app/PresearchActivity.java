/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import org.json.JSONException;

import org.chromium.base.ApplicationStatus;
import org.chromium.base.PresearchReflectionUtil;
import org.chromium.base.CommandLine;
import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.ApplicationLifetime;
import org.chromium.chrome.browser.PresearchConfig;
import org.chromium.chrome.browser.PresearchFeatureList;
import org.chromium.chrome.browser.PresearchHelper;
import org.chromium.chrome.browser.PresearchRelaunchUtils;
import org.chromium.chrome.browser.PresearchRewardsHelper;
import org.chromium.chrome.browser.PresearchRewardsObserver;
import org.chromium.chrome.browser.PresearchSyncReflectionUtils;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.CrossPromotionalModalDialogFragment;
import org.chromium.chrome.browser.DeprecateBAPModalDialogFragment;
import org.chromium.chrome.browser.LaunchIntentDispatcher;
import org.chromium.chrome.browser.SetDefaultBrowserActivity;
import org.chromium.chrome.browser.bookmarks.BookmarkBridge;
import org.chromium.chrome.browser.bookmarks.BookmarkModel;
import org.chromium.chrome.browser.presearch_stats.PresearchStatsUtil;
import org.chromium.chrome.browser.dependency_injection.ChromeActivityComponent;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.flags.ChromeSwitches;
import org.chromium.chrome.browser.informers.PresearchAndroidSyncDisabledInformer;
import org.chromium.chrome.browser.notifications.PresearchSetDefaultBrowserNotificationService;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.chrome.browser.ntp.NewTabPage;
import org.chromium.chrome.browser.onboarding.OnboardingActivity;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.P3aOnboardingActivity;
import org.chromium.chrome.browser.onboarding.v2.HighlightDialogFragment;
import org.chromium.chrome.browser.preferences.PresearchPrefServiceBridge;
import org.chromium.chrome.browser.preferences.PresearchPreferenceKeys;
import org.chromium.chrome.browser.preferences.Pref;
import org.chromium.chrome.browser.preferences.ChromePreferenceKeys;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.rate.RateDialogFragment;
import org.chromium.chrome.browser.rate.RateUtils;
import org.chromium.chrome.browser.settings.PresearchRewardsPreferences;
import org.chromium.chrome.browser.settings.PresearchSearchEngineUtils;
import org.chromium.chrome.browser.share.ShareDelegate;
import org.chromium.chrome.browser.share.ShareDelegateImpl.ShareOrigin;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabImpl;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabModel;
import org.chromium.chrome.browser.tabmodel.TabModelUtils;
import org.chromium.chrome.browser.toolbar.top.PresearchToolbarLayout;
import org.chromium.chrome.browser.preferences.PresearchPref;
import org.chromium.chrome.browser.util.PresearchDbUtil;
import org.chromium.chrome.browser.util.PresearchReferrer;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.chrome.browser.widget.crypto.binance.BinanceAccountBalance;
import org.chromium.chrome.browser.widget.crypto.binance.BinanceWidgetManager;
import org.chromium.components.bookmarks.BookmarkId;
import org.chromium.components.bookmarks.BookmarkType;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.search_engines.TemplateUrl;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.widget.Toast;

import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;

/**
 * Presearch's extension for ChromeActivity
 */
@JNINamespace("chrome::android")
public abstract class PresearchActivity < C extends ChromeActivityComponent >
  extends ChromeActivity implements PresearchRewardsObserver {
    public static final int SITE_BANNER_REQUEST_CODE = 33;
    public static final int VERIFY_WALLET_ACTIVITY_REQUEST_CODE = 34;
    public static final int USER_WALLET_ACTIVITY_REQUEST_CODE = 35;
    public static final String ADD_FUNDS_URL = "chrome://rewards/";
    public static final String REWARDS_SETTINGS_URL = "chrome://rewards/";
    public static final String PRESEARCH_REWARDS_SETTINGS_URL = "presearch://rewards/";
    public static final String REWARDS_AC_SETTINGS_URL = "chrome://rewards/";
    public static final String REWARDS_LEARN_MORE_URL = "https://presearch.io/";
    public static final String PRESEARCH_TERMS_PAGE = "https://presearch.org/terms";
    public static final String PRESEARCH_SE_URL = "https://engine.presearch.org";
    public static final String P3A_URL = "https://presearch.org/privacy";
    public static final String PRESEARCH_PRIVACY_POLICY = "https://presearch.org/privacy";
    private static final String PREF_CLOSE_TABS_ON_EXIT = "close_tabs_on_exit";
    public static final String OPEN_URL = "open_url";

    public static final String PRESEARCH_PRODUCTION_PACKAGE_NAME = "com.presearch";
    public static final String PRESEARCH_BETA_PACKAGE_NAME = "com.presearch_beta";
    public static final String PRESEARCH_NIGHTLY_PACKAGE_NAME = "com.presearch_nightly";

    private static final int DAYS_1 = 1;
    private static final int DAYS_4 = 4;
    private static final int DAYS_5 = 5;
    private static final int DAYS_12 = 12;

    /**
     * Settings for sending local notification reminders.
     */
    public static final String CHANNEL_ID = "com.presearch";
    public static final String ANDROID_SETUPWIZARD_PACKAGE_NAME = "com.google.android.setupwizard";
    public static final String ANDROID_PACKAGE_NAME = "android";
    public static final String PRESEARCH_BLOG_URL = "http://support.presearch.org";

    // Explicitly declare this variable to avoid build errors.
    // It will be removed in asm and parent variable will be used instead.
    protected ObservableSupplier < Profile > mTabModelProfileSupplier;

    public PresearchActivity() {
      // Disable key checker to avoid asserts on Presearch keys in debug
      SharedPreferencesManager.getInstance().disableKeyCheckerForTesting();
    }

    @Override
    public void onResumeWithNative() {
      super.onResumeWithNative();
      PresearchActivityJni.get().restartStatsUpdater();
    }

    @Override
    public boolean onMenuOrKeyboardAction(int id, boolean fromMenu) {
      final TabImpl currentTab = (TabImpl) getActivityTab();
      // Handle items replaced by Presearch.
      if (id == R.id.info_menu_id && currentTab != null) {
        ShareDelegate shareDelegate = (ShareDelegate) getShareDelegateSupplier().get();
        shareDelegate.share(currentTab, false, ShareOrigin.OVERFLOW_MENU);
        return true;
      }

      if (super.onMenuOrKeyboardAction(id, fromMenu)) {
        return true;
      }

      // Handle items added by Presearch.
      if (currentTab == null) {
        return false;
      } else if (id == R.id.exit_id) {
        ApplicationLifetime.terminate(false);
      } else if (id == R.id.set_default_browser) {
        handlePresearchSetDefaultBrowserDialog();
      } else {
        return false;
      }

      return true;
    }

    @Override
    public void initializeState() {
      super.initializeState();
      if (isNoRestoreState()) {
        CommandLine.getInstance().appendSwitch(ChromeSwitches.NO_RESTORE_STATE);
      }
      PresearchSearchEngineUtils.initializePresearchSearchEngineStates(getTabModelSelector());
    }

    @Override
    public void onResume() {
      super.onResume();

      Tab tab = getActivityTab();
      if (tab == null)
         return;

      // Set proper active DSE whenever presearch returns to foreground.
      // If active tab is private, set private DSE as an active DSE.
      PresearchSearchEngineUtils.updateActiveDSE(tab.isIncognito());
    }

    @Override
    public void onPause() {
      super.onPause();

      Tab tab = getActivityTab();
      if (tab == null)
        return;

      // Set normal DSE as an active DSE when presearch goes in background
      // because currently set DSE is used by outside of presearch(ex, presearch search widget).
      if (tab.isIncognito()) {
        PresearchSearchEngineUtils.updateActiveDSE(false);
      }
    }

    @Override
    public void performPostInflationStartup() {
      super.performPostInflationStartup();

      PresearchReferrer.getInstance().initReferrer(this);
      createNotificationChannel();
      setupPresearchSetDefaultBrowserNotification();
    }

    @Override
    protected void initializeStartupMetrics() {
      super.initializeStartupMetrics();

      // Disable FRE for arm64 builds where ChromeActivity is the one that
      // triggers FRE instead of ChromeLauncherActivity on arm32 build.
      PresearchHelper.DisableFREDRP();
    }

    @Override
    public void finishNativeInitialization() {
      super.finishNativeInitialization();

      UserPrefs.get(Profile.getLastUsedRegularProfile()).setBoolean(PresearchPref.NEW_TAB_PAGE_SHOW_BACKGROUND_IMAGE, false);
      UserPrefs.get(Profile.getLastUsedRegularProfile()).setBoolean(PresearchPref.NEW_TAB_PAGE_SHOW_SPONSORED_IMAGES_BACKGROUND_IMAGE, false);
      SharedPreferencesManager.getInstance().writeBoolean(
                ChromePreferenceKeys.SETTINGS_DEVELOPER_ENABLED, false);

      if (SharedPreferencesManager.getInstance().readBoolean(
          PresearchPreferenceKeys.PRESEARCH_DOUBLE_RESTART, false)) {
        SharedPreferencesManager.getInstance().writeBoolean(
          PresearchPreferenceKeys.PRESEARCH_DOUBLE_RESTART, false);
        PresearchRelaunchUtils.restart();
        return;
      }

      int appOpenCount = SharedPreferencesManager.getInstance().readInt(PresearchPreferenceKeys.PRESEARCH_APP_OPEN_COUNT);
      SharedPreferencesManager.getInstance().writeInt(PresearchPreferenceKeys.PRESEARCH_APP_OPEN_COUNT, appOpenCount + 1);

      if (PackageUtils.isFirstInstall(this)) {
        setDSEToPresearch();
      }

      //set bg ads to off for existing and new installations
      setBgPresearchAdsDefaultOff();

      checkForNotificationData();

      if (!RateUtils.getInstance(this).getPrefRateEnabled()) {
            RateUtils.getInstance(this).setPrefRateEnabled(true);
            RateUtils.getInstance(this).setNextRateDateAndCount();
        }

      if (RateUtils.getInstance(this).shouldShowRateDialog())
            showBraveRateDialog();

      if (!PackageUtils.isFirstInstall(this) &&
        !OnboardingPrefManager.getInstance().isP3AEnabledForExistingUsers()) {
          PresearchPrefServiceBridge.getInstance().setP3AEnabled(false);
          OnboardingPrefManager.getInstance().setP3AEnabledForExistingUsers(false);
      }

      if (PresearchConfig.P3A_ENABLED &&
        !OnboardingPrefManager.getInstance().isP3aOnboardingShown()) {
          Intent p3aOnboardingIntent = new Intent(this, P3aOnboardingActivity.class);
          p3aOnboardingIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
          startActivity(p3aOnboardingIntent);
      }

      if (!OnboardingPrefManager.getInstance().isOneTimeNotificationStarted() && PackageUtils.isFirstInstall(this)) {
        RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DEFAULT_BROWSER_1);
        RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DEFAULT_BROWSER_2);
        RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DEFAULT_BROWSER_3);
        OnboardingPrefManager.getInstance().setOneTimeNotificationStarted(true);
      }

      if (SharedPreferencesManager.getInstance().readInt(PresearchPreferenceKeys.PRESEARCH_APP_OPEN_COUNT) ==
        1) {
        Calendar calender = Calendar.getInstance();
        calender.setTime(new Date());
        calender.add(Calendar.DATE, DAYS_5);
        OnboardingPrefManager.getInstance().setNextSetDefaultBrowserModalDate(
          calender.getTimeInMillis());
      }
      checkSetDefaultBrowserModal();
    }

    @Override
    public void OnRewardsParameters(int errorCode) {}

    @Override
    public void OnStartProcess() {
      mPresearchRewardsNativeWorker.GetRewardsParameters();
    }

    private void checkSetDefaultBrowserModal() {
      boolean shouldShowDefaultBrowserModal =
        (OnboardingPrefManager.getInstance().getNextSetDefaultBrowserModalDate() > 0 &&
          System.currentTimeMillis() >
          OnboardingPrefManager.getInstance()
          .getNextSetDefaultBrowserModalDate());
      boolean shouldShowDefaultBrowserModalAfterP3A =
        OnboardingPrefManager.getInstance().shouldShowDefaultBrowserModalAfterP3A();
      if (!PresearchSetDefaultBrowserNotificationService.isPresearchSetAsDefaultBrowser(this) &&
        (shouldShowDefaultBrowserModalAfterP3A || shouldShowDefaultBrowserModal)) {
        Intent setDefaultBrowserIntent = new Intent(this, SetDefaultBrowserActivity.class);
        setDefaultBrowserIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        startActivity(setDefaultBrowserIntent);
        if (shouldShowDefaultBrowserModal) {
          OnboardingPrefManager.getInstance().setNextSetDefaultBrowserModalDate(0);
        }
        if (shouldShowDefaultBrowserModalAfterP3A) {
          OnboardingPrefManager.getInstance().setShowDefaultBrowserModalAfterP3A(false);
        }
      }
    }

    private void setDSEToPresearch() {
      // Set the default search engine to engine.presearch.org. Added by Mamy
      TemplateUrl presearchTemplateUrl =
        PresearchSearchEngineUtils.getTemplateUrlByShortName(OnboardingPrefManager.QWANT);
      if (presearchTemplateUrl != null) {
        PresearchSearchEngineUtils.setDSEPrefs(presearchTemplateUrl, false);
        PresearchSearchEngineUtils.setDSEPrefs(presearchTemplateUrl, true);
      }
    }

    private void checkForNotificationData() {
      Intent notifIntent = getIntent();
      if (notifIntent != null && notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE) != null) {
        Log.e("NTP", notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE));
        String notificationType = notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE);
      }
    }

    public void checkForPresearchStats() {
        if (getActivityTab() != null && getActivityTab().getUrlString() != null &&
          !UrlUtilities.isNTPUrl(getActivityTab().getUrlString())) {
          OnboardingPrefManager.getInstance().setFromNotification(true);
          if (getTabCreator(false) != null) {
            getTabCreator(false).launchUrl(
              UrlConstants.NTP_URL, TabLaunchType.FROM_CHROME_UI);
          }
        }
    }

    public void hideRewardsOnboardingIcon() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        layout.hideRewardsOnboardingIcon();
      }
    }

    public void showRewardsTooltip() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        layout.showRewardsTooltip();
      }
    }

    private void createNotificationChannel() {
      Context context = ContextUtils.getApplicationContext();
      // Create the NotificationChannel, but only on API 26+ because
      // the NotificationChannel class is new and not in the support library
      if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
        CharSequence name = "Presearch Browser";
        String description = "Notification channel for Presearch Browser";
        int importance = NotificationManager.IMPORTANCE_DEFAULT;
        NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
        channel.setDescription(description);
        // Register the channel with the system; you can't change the importance
        // or other notification behaviors after this
        NotificationManager notificationManager = getSystemService(NotificationManager.class);
        notificationManager.createNotificationChannel(channel);
      }
    }

    private void setupPresearchSetDefaultBrowserNotification() {
      // Post task to IO thread because isPresearchSetAsDefaultBrowser may cause
      // sqlite file IO operation underneath
      PostTask.postTask(TaskTraits.BEST_EFFORT_MAY_BLOCK, () -> {
        Context context = ContextUtils.getApplicationContext();
        if (PresearchSetDefaultBrowserNotificationService.isPresearchSetAsDefaultBrowser(this)) {
          // Don't ask again
          return;
        }
        Intent intent = new Intent(context, PresearchSetDefaultBrowserNotificationService.class);
        context.sendBroadcast(intent);
      });
    }

    private boolean isNoRestoreState() {
      return ContextUtils.getAppSharedPreferences().getBoolean(PREF_CLOSE_TABS_ON_EXIT, false);
    }

    public void handlePresearchSetDefaultBrowserDialog() {
      /* (Albert Wang): Default app settings didn't get added until API 24
       * https://developer.android.com/reference/android/provider/Settings#ACTION_MANAGE_DEFAULT_APPS_SETTINGS
       */
      Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(UrlConstants.HTTP_URL_PREFIX));
      boolean supportsDefault = Build.VERSION.SDK_INT >= Build.VERSION_CODES.N;
      ResolveInfo resolveInfo = getPackageManager().resolveActivity( browserIntent, supportsDefault ? PackageManager.MATCH_DEFAULT_ONLY : 0);
      Context context = ContextUtils.getApplicationContext();
      if (PresearchSetDefaultBrowserNotificationService.isPresearchSetAsDefaultBrowser(this)) {
        Toast toast = Toast.makeText(context, R.string.presearch_already_set_as_default_browser, Toast.LENGTH_LONG);
        toast.show();
        return;
      }
      if (supportsDefault) {
        if (resolveInfo.activityInfo.packageName.equals(ANDROID_SETUPWIZARD_PACKAGE_NAME) ||
          resolveInfo.activityInfo.packageName.equals(ANDROID_PACKAGE_NAME)) {
          LayoutInflater inflater = getLayoutInflater();
          View layout = inflater.inflate(R.layout.presearch_set_default_browser_dialog, (ViewGroup) findViewById(R.id.presearch_set_default_browser_toast_container));

          Toast toast = new Toast(context, layout);
          toast.setDuration(Toast.LENGTH_LONG);
          toast.setGravity(Gravity.TOP, 0, 40);
          toast.show();
          Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(PRESEARCH_BLOG_URL));
          intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
          context.startActivity(intent);
        } else {
          Intent intent = new Intent(Settings.ACTION_MANAGE_DEFAULT_APPS_SETTINGS);
          intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
          context.startActivity(intent);
        }
      } else {
        if (resolveInfo.activityInfo.packageName.equals(ANDROID_SETUPWIZARD_PACKAGE_NAME) ||
          resolveInfo.activityInfo.packageName.equals(ANDROID_PACKAGE_NAME)) {
          // (Albert Wang): From what I've experimented on 6.0,
          // default browser popup is in the middle of the screen for
          // these versions. So we shouldn't show the toast.
          Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(PRESEARCH_BLOG_URL));
          intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
          context.startActivity(intent);
        } else {
          Toast toast = Toast.makeText(context, R.string.presearch_default_browser_go_to_settings, Toast.LENGTH_LONG);
          toast.show();
          return;
        }
      }
    }

    public void OnRewardsPanelDismiss() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        layout.onRewardsPanelDismiss();
      }
    }

    public void dismissRewardsPanel() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        layout.dismissRewardsPanel();
      }
    }

    public void dismissShieldsTooltip() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        layout.dismissShieldsTooltip();
      }
    }

    public void openRewardsPanel() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        layout.openRewardsPanel();
      }
    }

    public boolean isRewardsPanelOpened() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        return layout.isRewardsPanelOpened();
      }
      return false;
    }

    public boolean isShieldsTooltipShown() {
      PresearchToolbarLayout layout = (PresearchToolbarLayout) findViewById(R.id.toolbar);
      assert layout != null;
      if (layout != null) {
        return layout.isShieldsTooltipShown();
      }
      return false;
    }

    public Tab selectExistingTab(String url) {
      Tab tab = getActivityTab();
      if (tab != null && tab.getUrlString().equals(url)) {
        return tab;
      }

      TabModel tabModel = getCurrentTabModel();
      int tabIndex = TabModelUtils.getTabIndexByUrl(tabModel, url);

      // Find if tab exists
      if (tabIndex != TabModel.INVALID_TAB_INDEX) {
        tab = tabModel.getTabAt(tabIndex);
        // Set active tab
        tabModel.setIndex(tabIndex, TabSelectionType.FROM_USER);
        return tab;
      } else {
        return null;
      }
    }

    public Tab openNewOrSelectExistingTab(String url) {
      TabModel tabModel = getCurrentTabModel();
      int tabRewardsIndex = TabModelUtils.getTabIndexByUrl(tabModel, url);

      Tab tab = selectExistingTab(url);

      if (tab != null) {
        return tab;
      } else { // Open a new tab
        return getTabCreator(false).launchUrl(url, TabLaunchType.FROM_CHROME_UI);
      }
    }

    private void showPresearchRateDialog() {
      RateDialogFragment mRateDialogFragment = new RateDialogFragment();
      mRateDialogFragment.setCancelable(false);
      mRateDialogFragment.show(getSupportFragmentManager(), "RateDialogFragment");
    }

    public void showDeprecateBAPDialog() {
      DeprecateBAPModalDialogFragment mDeprecateBAPModalDialogFragment =
        new DeprecateBAPModalDialogFragment();
      mDeprecateBAPModalDialogFragment.setCancelable(false);
      mDeprecateBAPModalDialogFragment.show(getSupportFragmentManager(), "DeprecateBAPModalDialogFragment");
    }

    static public ChromeTabbedActivity getChromeTabbedActivity() {
      for (Activity ref: ApplicationStatus.getRunningActivities()) {
        if (!(ref instanceof ChromeTabbedActivity)) continue;
        return (ChromeTabbedActivity) ref;
      }
      return null;
    }

    static public PresearchActivity getPresearchActivity() {
      for (Activity ref: ApplicationStatus.getRunningActivities()) {
        if (!(ref instanceof PresearchActivity)) continue;
        return (PresearchActivity) ref;
      }
      return null;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
      if (resultCode == RESULT_OK){
        dismissRewardsPanel();
        String open_url = data.getStringExtra(PresearchActivity.OPEN_URL);
        if (!TextUtils.isEmpty(open_url)) {
          openNewOrSelectExistingTab(open_url);
        }
      }
      super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onRequestPermissionsResult(
      int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
      if (requestCode == PresearchStatsUtil.SHARE_STATS_WRITE_EXTERNAL_STORAGE_PERM &&
        grantResults.length != 0 &&
        grantResults[0] == PackageManager.PERMISSION_GRANTED) {
        PresearchStatsUtil.shareStats(R.layout.presearch_stats_share_layout);
      }
      super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    /**
     * Disable background ads on Android. Issue #8641.
     */
    private void setBgPresearchAdsDefaultOff() {
      SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
      boolean exists = sharedPreferences.contains(PresearchRewardsPreferences.PREF_ADS_SWITCH_DEFAULT_HAS_BEEN_SET);
      if (!exists) {
        SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PresearchRewardsPreferences.PREF_ADS_SWITCH, false);
        sharedPreferencesEditor.putBoolean(PresearchRewardsPreferences.PREF_ADS_SWITCH_DEFAULT_HAS_BEEN_SET, true);
        sharedPreferencesEditor.apply();
      }
    }

    @Override
    public void performPreInflationStartup() {
      PresearchDbUtil dbUtil = PresearchDbUtil.getInstance();
      if (dbUtil.dbOperationRequested()) {
        AlertDialog dialog = new AlertDialog.Builder(this)
          .setMessage(dbUtil.performDbExportOnStart() ? "Exporting database, please wait..." :
            "Importing database, please wait...")
          .setCancelable(false)
          .create();
        dialog.setCanceledOnTouchOutside(false);
        if (dbUtil.performDbExportOnStart()) {
          dbUtil.setPerformDbExportOnStart(false);
          dbUtil.ExportRewardsDb(dialog);
        } else if (dbUtil.performDbImportOnStart() && !dbUtil.dbImportFile().isEmpty()) {
          dbUtil.setPerformDbImportOnStart(false);
          dbUtil.ImportRewardsDb(dialog, dbUtil.dbImportFile());
        }
        dbUtil.cleanUpDbOperationRequest();
      }
      super.performPreInflationStartup();
    }

    @Override
    protected @LaunchIntentDispatcher.Action int maybeDispatchLaunchIntent(
      Intent intent, Bundle savedInstanceState) {
      boolean notificationUpdate = IntentUtils.safeGetBooleanExtra(
        intent, PresearchPreferenceKeys.PRESEARCH_UPDATE_EXTRA_PARAM, false);
      if (notificationUpdate) {
        SetUpdatePreferences();
      }

      return super.maybeDispatchLaunchIntent(intent, savedInstanceState);
    }

    private void SetUpdatePreferences() {
      Calendar currentTime = Calendar.getInstance();
      long milliSeconds = currentTime.getTimeInMillis();

      SharedPreferences sharedPref =    getApplicationContext().getSharedPreferences(PresearchPreferenceKeys.PRESEARCH_NOTIFICATION_PREF_NAME, 0);
      SharedPreferences.Editor editor = sharedPref.edit();

      editor.putLong(PresearchPreferenceKeys.PRESEARCH_MILLISECONDS_NAME, milliSeconds);
      editor.apply();
    }

    @NativeMethods
    interface Natives {
      void restartStatsUpdater();
    }
  }
