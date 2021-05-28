/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/common/pref_names.h"

const char kAdsBlocked[] = "presearch.stats.ads_blocked";
// We no longer update this pref, but we keep it around for now because it's
// added to kAdsBlocked when being displayed.
const char kTrackersBlocked[] = "presearch.stats.trackers_blocked";
const char kJavascriptBlocked[] = "presearch.stats.javascript_blocked";
const char kHttpsUpgrades[] = "presearch.stats.https_upgrades";
const char kFingerprintingBlocked[] = "presearch.stats.fingerprinting_blocked";
const char kLastCheckYMD[] = "presearch.stats.last_check_ymd";
const char kLastCheckWOY[] = "presearch.stats.last_check_woy";
const char kLastCheckMonth[] = "presearch.stats.last_check_month";
const char kFirstCheckMade[] = "presearch.stats.first_check_made";
// Set to true if the user met the threshold requirements and successfully
// sent a ping to the stats-updater server.
const char kThresholdCheckMade[] = "presearch.stats.threshold_check_made";
// Serialized query for to send to the stats-updater server. Needs to be saved
// in the case that the user sends the standard usage ping, stops the browser,
// meets the threshold requirements, and then starts the browser before the
// threshold ping was sent.
const char kThresholdQuery[] = "presearch.stats.threshold_query";
const char kWeekOfInstallation[] = "presearch.stats.week_of_installation";
const char kAdBlockCheckedDefaultRegion[] =
    "presearch.ad_block.checked_default_region";
const char kAdBlockCustomFilters[] = "presearch.ad_block.custom_filters";
const char kAdBlockRegionalFilters[] = "presearch.ad_block.regional_filters";
const char kWidevineOptedIn[] = "presearch.widevine_opted_in";
const char kAskWidevineInstall[] = "presearch.ask_widevine_install";
const char kUseAlternativeSearchEngineProvider[] =
    "presearch.use_alternate_private_search_engine";
const char kAlternativeSearchEngineProviderInTor[] =
    "presearch.alternate_private_search_engine_in_tor";
const char kLocationBarIsWide[] = "presearch.location_bar_is_wide";
const char kReferralDownloadID[] = "presearch.referral.download_id";
const char kReferralTimestamp[] = "presearch.referral.timestamp";
const char kReferralAttemptTimestamp[] =
    "presearch.referral.referral_attempt_timestamp";
const char kReferralAttemptCount[] = "presearch.referral.referral_attempt_count";
const char kReferralHeaders[] = "presearch.referral.headers";
const char kReferralAndroidFirstRunTimestamp[] =
    "presearch.referral_android_first_run_timestamp";
const char kHTTPSEVerywhereControlType[] = "presearch.https_everywhere_default";
const char kNoScriptControlType[] = "presearch.no_script_default";
const char kShieldsAdvancedViewEnabled[] =
    "presearch.shields.advanced_view_enabled";
const char kShieldsStatsBadgeVisible[] =
    "presearch.shields.stats_badge_visible";
const char kAdControlType[] = "presearch.ad_default";
const char kGoogleLoginControlType[] = "presearch.google_login_default";
const char kFBEmbedControlType[] = "presearch.fb_embed_default";
const char kTwitterEmbedControlType[] = "presearch.twitter_embed_default";
const char kLinkedInEmbedControlType[] = "presearch.linkedin_embed_default";
const char kWebTorrentEnabled[] = "presearch.webtorrent_enabled";
const char kHangoutsEnabled[] = "presearch.hangouts_enabled";
const char kIPFSCompanionEnabled[] = "presearch.ipfs_companion_enabled";
const char kNewTabPageShowClock[] = "presearch.new_tab_page.show_clock";
const char kNewTabPageClockFormat[] = "presearch.new_tab_page.clock_format";
const char kNewTabPageShowStats[] = "presearch.new_tab_page.show_stats";
const char kNewTabPageShowToday[] = "presearch.new_tab_page.show_presearch_today";
const char kNewTabPageShowRewards[] = "presearch.new_tab_page.show_rewards";
const char kNewTabPageShowBinance[] = "presearch.new_tab_page.show_binance";
const char kNewTabPageShowGemini[] = "presearch.new_tab_page.show_gemini";
const char kNewTabPageShowTogether[] = "presearch.new_tab_page.show_together";
const char kNewTabPageShowsOptions[] = "presearch.new_tab_page.shows_options";
const char kPresearchTodaySources[] = "presearch.today.sources";
const char kPresearchTodayIntroDismissed[] = "presearch.today.intro_dismissed";
const char kPresearchTodayWeeklySessionCount[] =
    "presearch.today.p3a_weekly_session_count";
const char kPresearchTodayWeeklyCardViewsCount[] =
    "presearch.today.p3a_weekly_card_views_count";
const char kPresearchTodayWeeklyCardVisitsCount[] =
    "presearch.today.p3a_weekly_card_visits_count";
const char kPresearchEnabledMediaRouter[] = "presearch.enable_media_router";
const char kBinanceAccessToken[] = "presearch.binance.access_token";
const char kBinanceRefreshToken[] = "presearch.binance.refresh_token";
const char kAlwaysShowBookmarkBarOnNTP[] =
    "presearch.always_show_bookmark_bar_on_ntp";
const char kAutocompleteEnabled[] = "presearch.autocomplete_enabled";
const char kTopSiteSuggestionsEnabled[] = "presearch.top_site_suggestions_enabled";
const char kPresearchSuggestedSiteSuggestionsEnabled[] =
    "presearch.presearch_suggested_site_suggestions_enabled";
const char kPresearchDarkMode[] = "presearch.dark_mode";
const char kOtherBookmarksMigrated[] = "presearch.other_bookmarks_migrated";
const char kPresearchShieldsSettingsVersion[] = "presearch.shields_settings_version";
const char kDefaultBrowserPromptEnabled[] =
    "presearch.default_browser_prompt_enabled";
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
const char kPresearchGCMChannelStatus[] = "presearch.gcm.channel_status";
#endif
const char kImportDialogExtensions[] = "import_dialog_extensions";
const char kImportDialogPayments[] = "import_dialog_payments";
const char kMRUCyclingEnabled[] = "presearch.mru_cycling_enabled";

#if defined(OS_ANDROID)
const char kDesktopModeEnabled[] = "presearch.desktop_mode_enabled";
const char kPlayYTVideoInBrowserEnabled[] =
    "presearch.play_yt_video_in_browser_enabled";
const char kBackgroundVideoPlaybackEnabled[] =
    "presearch.background_video_playback";
const char kSafetynetCheckFailed[] = "safetynetcheck.failed";
const char kSafetynetStatus[] = "safetynet.status";
#endif

const char kDefaultBrowserLaunchingCount[] =
    "presearch.default_browser.launching_count";
const char kPresearchDefaultSearchVersion[] = "presearch.search.default_version";

// deprecated
const char kPresearchThemeType[] = "presearch.theme.type";
const char kUseOverriddenPresearchThemeType[] =
    "presearch.theme.use_overridden_presearch_theme_type";
const char kNewTabPageShowTopSites[] = "presearch.new_tab_page.show_top_sites";
