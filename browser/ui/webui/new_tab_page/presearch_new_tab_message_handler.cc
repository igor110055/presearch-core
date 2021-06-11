// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/browser/ui/webui/new_tab_page/presearch_new_tab_message_handler.h"

#include <memory>
#include <utility>

#include "base/guid.h"
#include "base/json/json_writer.h"
#include "base/metrics/histogram_macros.h"
#include "base/values.h"
#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/browser/ntp_background_images/view_counter_service_factory.h"
#include "presearch/browser/profiles/profile_util.h"
#include "presearch/browser/search_engines/search_engine_provider_util.h"
#include "presearch/browser/ui/webui/new_tab_page/presearch_new_tab_ui.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_ads/browser/ads_service.h"
#include "presearch/components/presearch_perf_predictor/browser/buildflags.h"
#include "presearch/components/crypto_dot_com/browser/buildflags/buildflags.h"
#include "presearch/components/ntp_background_images/browser/features.h"
#include "presearch/components/ntp_background_images/browser/url_constants.h"
#include "presearch/components/ntp_background_images/browser/view_counter_service.h"
#include "presearch/components/ntp_background_images/common/pref_names.h"
#include "presearch/components/p3a/presearch_p3a_utils.h"
#include "presearch/components/weekly_storage/weekly_storage.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_features.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui_data_source.h"

using ntp_background_images::features::kPresearchNTPBrandedWallpaper;
using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::kNewTabPageShowSponsoredImagesBackgroundImage;  // NOLINT
using ntp_background_images::prefs::kBrandedWallpaperNotificationDismissed;
using ntp_background_images::ViewCounterServiceFactory;

#if BUILDFLAG(ENABLE_PRESEARCH_PERF_PREDICTOR)
#include "presearch/components/presearch_perf_predictor/common/pref_names.h"
#endif

#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
#include "presearch/components/crypto_dot_com/common/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "presearch/components/tor/tor_launcher_factory.h"
#endif

namespace {

bool IsPrivateNewTab(Profile* profile) {
  return profile->IsIncognitoProfile() || profile->IsGuestSession();
}

base::DictionaryValue GetStatsDictionary(PrefService* prefs) {
  base::DictionaryValue stats_data;
  stats_data.SetInteger(
    "adsBlockedStat",
    prefs->GetUint64(kAdsBlocked) + prefs->GetUint64(kTrackersBlocked));
  stats_data.SetInteger(
    "javascriptBlockedStat",
    prefs->GetUint64(kJavascriptBlocked));
  stats_data.SetInteger(
    "fingerprintingBlockedStat",
    prefs->GetUint64(kFingerprintingBlocked));
#if BUILDFLAG(ENABLE_PRESEARCH_PERF_PREDICTOR)
  stats_data.SetDouble(
      "bandwidthSavedStat",
      prefs->GetUint64(presearch_perf_predictor::prefs::kBandwidthSavedBytes));
#endif
  return stats_data;
}

base::DictionaryValue GetPreferencesDictionary(PrefService* prefs) {
  base::DictionaryValue pref_data;
  pref_data.SetBoolean(
      "showBackgroundImage",
      prefs->GetBoolean(kNewTabPageShowBackgroundImage));
  pref_data.SetBoolean(
      "brandedWallpaperOptIn",
      prefs->GetBoolean(kNewTabPageShowSponsoredImagesBackgroundImage));
  pref_data.SetBoolean(
      "showClock",
      prefs->GetBoolean(kNewTabPageShowClock));
  pref_data.SetString(
      "clockFormat",
      prefs->GetString(kNewTabPageClockFormat));
  pref_data.SetBoolean(
      "showStats",
      prefs->GetBoolean(kNewTabPageShowStats));
  pref_data.SetBoolean(
      "showToday",
      prefs->GetBoolean(kNewTabPageShowToday));
  pref_data.SetBoolean(
      "showRewards",
      prefs->GetBoolean(kNewTabPageShowRewards));
  pref_data.SetBoolean(
      "isBrandedWallpaperNotificationDismissed",
      prefs->GetBoolean(kBrandedWallpaperNotificationDismissed));
  pref_data.SetBoolean(
      "isPresearchTodayIntroDismissed",
      prefs->GetBoolean(kPresearchTodayIntroDismissed));
  pref_data.SetBoolean(
      "showBinance",
      prefs->GetBoolean(kNewTabPageShowBinance));
  pref_data.SetBoolean(
      "showTogether",
      prefs->GetBoolean(kNewTabPageShowTogether));
  pref_data.SetBoolean(
      "showGemini",
      prefs->GetBoolean(kNewTabPageShowGemini));
#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
  pref_data.SetBoolean(
      "showCryptoDotCom",
      prefs->GetBoolean(kCryptoDotComNewTabPageShowCryptoDotCom));
#endif
  return pref_data;
}

base::DictionaryValue GetPrivatePropertiesDictionary(PrefService* prefs) {
  base::DictionaryValue private_data;
  private_data.SetBoolean(
      "useAlternativePrivateSearchEngine",
      prefs->GetBoolean(kUseAlternativeSearchEngineProvider));
  return private_data;
}

base::DictionaryValue GetTorPropertiesDictionary(bool connected,
                                                 const std::string& progress) {
  base::DictionaryValue tor_data;
  tor_data.SetBoolean("torCircuitEstablished", connected);
  tor_data.SetString("torInitProgress", progress);
  return tor_data;
}

// TODO(petemill): Move p3a to own NTP component so it can
// be used by other platforms.

enum class NTPCustomizeUsage {
  kNeverOpened,
  kOpened,
  kOpenedAndEdited,
  kSize
};

const char kNTPCustomizeUsageStatus[] =
    "presearch.new_tab_page.customize_p3a_usage";

}  // namespace

// static
void PresearchNewTabMessageHandler::RegisterLocalStatePrefs(
    PrefRegistrySimple* local_state) {
  local_state->RegisterIntegerPref(kNTPCustomizeUsageStatus, -1);
}

void PresearchNewTabMessageHandler::RecordInitialP3AValues(
    PrefService* local_state) {
  presearch::RecordValueIfGreater<NTPCustomizeUsage>(
      NTPCustomizeUsage::kNeverOpened, "Presearch.NTP.CustomizeUsageStatus",
      kNTPCustomizeUsageStatus, local_state);
}

PresearchNewTabMessageHandler* PresearchNewTabMessageHandler::Create(
      content::WebUIDataSource* source, Profile* profile) {
  //
  // Initial Values
  // Should only contain data that is static
  //
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile);
  // For safety, default |is_ads_supported_locale_| to true. Better to have
  // false positive than falsen egative,
  // in which case we would not show "opt out" toggle.
  bool is_ads_supported_locale_ = true;
  if (!ads_service_) {
    LOG(ERROR) << "Ads service is not initialized!";
  } else {
    is_ads_supported_locale_ = ads_service_->IsSupportedLocale();
  }

  source->AddBoolean(
      "featureFlagPresearchNTPSponsoredImagesWallpaper",
      base::FeatureList::IsEnabled(kPresearchNTPBrandedWallpaper) &&
      is_ads_supported_locale_);
  // Private Tab info
  if (IsPrivateNewTab(profile)) {
    // source->AddBoolean(
    //   "isTor", profile->IsTor());
    source->AddBoolean(
      "isTor", false);
    // source->AddBoolean(
    //   "isQwant", presearch::IsRegionForQwant(profile));
    source->AddBoolean(
      "isQwant", true);
  }
  return new PresearchNewTabMessageHandler(profile);
}

PresearchNewTabMessageHandler::PresearchNewTabMessageHandler(Profile* profile)
    : profile_(profile) {
#if BUILDFLAG(ENABLE_TOR)
  tor_launcher_factory_ = TorLauncherFactory::GetInstance();
#endif
}

PresearchNewTabMessageHandler::~PresearchNewTabMessageHandler() {
#if BUILDFLAG(ENABLE_TOR)
  if (tor_launcher_factory_)
    tor_launcher_factory_->RemoveObserver(this);
#endif
}

void PresearchNewTabMessageHandler::RegisterMessages() {
  // TODO(petemill): This MessageHandler can be split up to
  // individual MessageHandlers for each individual topic area,
  // should other WebUI pages wish to consume the APIs:
  // - Stats
  // - Preferences
  // - PrivatePage properties
  web_ui()->RegisterMessageCallback(
      "getNewTabPagePreferences",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleGetPreferences,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabPageStats",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleGetStats,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabPagePrivateProperties",
      base::BindRepeating(
          &PresearchNewTabMessageHandler::HandleGetPrivateProperties,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabPageTorProperties",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleGetTorProperties,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "toggleAlternativePrivateSearchEngine",
      base::BindRepeating(&PresearchNewTabMessageHandler::
                              HandleToggleAlternativeSearchEngineProvider,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "saveNewTabPagePref",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleSaveNewTabPagePref,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "registerNewTabPageView",
      base::BindRepeating(
          &PresearchNewTabMessageHandler::HandleRegisterNewTabPageView,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "brandedWallpaperLogoClicked",
      base::BindRepeating(
          &PresearchNewTabMessageHandler::HandleBrandedWallpaperLogoClicked,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getBrandedWallpaperData",
      base::BindRepeating(
          &PresearchNewTabMessageHandler::HandleGetBrandedWallpaperData,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "customizeClicked",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleCustomizeClicked,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "todayInteractionBegin",
      base::BindRepeating(
          &PresearchNewTabMessageHandler::HandleTodayInteractionBegin,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "todayOnCardVisit",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleTodayOnCardVisit,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "todayOnCardViews",
      base::BindRepeating(&PresearchNewTabMessageHandler::HandleTodayOnCardViews,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "todayOnPromotedCardView",
      base::BindRepeating(
          &PresearchNewTabMessageHandler::HandleTodayOnPromotedCardView,
          base::Unretained(this)));
}

void PresearchNewTabMessageHandler::OnJavascriptAllowed() {
  // Observe relevant preferences
  PrefService* prefs = profile_->GetPrefs();
  pref_change_registrar_.Init(prefs);
  // Stats
  pref_change_registrar_.Add(kAdsBlocked,
    base::Bind(&PresearchNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kTrackersBlocked,
    base::Bind(&PresearchNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kJavascriptBlocked,
    base::Bind(&PresearchNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kHttpsUpgrades,
    base::Bind(&PresearchNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kFingerprintingBlocked,
    base::Bind(&PresearchNewTabMessageHandler::OnStatsChanged,
    base::Unretained(this)));

  if (IsPrivateNewTab(profile_)) {
    // Private New Tab Page preferences
    pref_change_registrar_.Add(kUseAlternativeSearchEngineProvider,
      base::Bind(&PresearchNewTabMessageHandler::OnPrivatePropertiesChanged,
      base::Unretained(this)));
    pref_change_registrar_.Add(kAlternativeSearchEngineProviderInTor,
      base::Bind(&PresearchNewTabMessageHandler::OnPrivatePropertiesChanged,
      base::Unretained(this)));
  }
  // New Tab Page preferences
  pref_change_registrar_.Add(kNewTabPageShowBackgroundImage,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowSponsoredImagesBackgroundImage,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowClock,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageClockFormat,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowStats,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowToday,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowRewards,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kBrandedWallpaperNotificationDismissed,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowBinance,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowTogether,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
  pref_change_registrar_.Add(kNewTabPageShowGemini,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
  pref_change_registrar_.Add(kCryptoDotComNewTabPageShowCryptoDotCom,
    base::Bind(&PresearchNewTabMessageHandler::OnPreferencesChanged,
    base::Unretained(this)));
#endif

#if BUILDFLAG(ENABLE_TOR)
  if (tor_launcher_factory_)
    tor_launcher_factory_->AddObserver(this);
#endif
}

void PresearchNewTabMessageHandler::OnJavascriptDisallowed() {
  pref_change_registrar_.RemoveAll();
#if BUILDFLAG(ENABLE_TOR)
  if (tor_launcher_factory_)
    tor_launcher_factory_->RemoveObserver(this);
#endif
}

void PresearchNewTabMessageHandler::HandleGetPreferences(
        const base::ListValue* args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPreferencesDictionary(prefs);
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void PresearchNewTabMessageHandler::HandleGetStats(const base::ListValue* args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetStatsDictionary(prefs);
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void PresearchNewTabMessageHandler::HandleGetPrivateProperties(
        const base::ListValue* args) {
  AllowJavascript();
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPrivatePropertiesDictionary(prefs);
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void PresearchNewTabMessageHandler::HandleGetTorProperties(
        const base::ListValue* args) {
  AllowJavascript();
#if BUILDFLAG(ENABLE_TOR)
  auto data = GetTorPropertiesDictionary(
      tor_launcher_factory_ ? tor_launcher_factory_->IsTorConnected() : false,
      "");
#else
  auto data = GetTorPropertiesDictionary(false, "");
#endif
  ResolveJavascriptCallback(args->GetList()[0], data);
}

void PresearchNewTabMessageHandler::HandleToggleAlternativeSearchEngineProvider(
    const base::ListValue* args) {
  presearch::ToggleUseAlternativeSearchEngineProvider(profile_);
}

void PresearchNewTabMessageHandler::HandleSaveNewTabPagePref(
    const base::ListValue* args) {
  if (args->GetSize() != 2) {
    LOG(ERROR) << "Invalid input";
    return;
  }
  presearch::RecordValueIfGreater<NTPCustomizeUsage>(
      NTPCustomizeUsage::kOpenedAndEdited, "Presearch.NTP.CustomizeUsageStatus",
      kNTPCustomizeUsageStatus, g_browser_process->local_state());
  PrefService* prefs = profile_->GetPrefs();
  // Collect args
  std::string settingsKeyInput = args->GetList()[0].GetString();
  auto settingsValue = args->GetList()[1].Clone();
  std::string settingsKey;

  // Handle string settings
  if (settingsValue.is_string()) {
    const auto settingsValueString = settingsValue.GetString();
    if (settingsKeyInput == "clockFormat") {
      settingsKey = kNewTabPageClockFormat;
    } else {
      LOG(ERROR) << "Invalid setting key";
      return;
    }
    prefs->SetString(settingsKey, settingsValueString);
    return;
  }

  // Handle bool settings
  if (!settingsValue.is_bool()) {
    LOG(ERROR) << "Invalid value type";
    return;
  }
  const auto settingsValueBool = settingsValue.GetBool();
  if (settingsKeyInput == "showBackgroundImage") {
    settingsKey = kNewTabPageShowBackgroundImage;
  } else if (settingsKeyInput == "brandedWallpaperOptIn") {
    // TODO(simonhong): I think above |brandedWallpaperOptIn| should be changed
    // to |sponsoredImagesWallpaperOptIn|.
    settingsKey = kNewTabPageShowSponsoredImagesBackgroundImage;
  } else if (settingsKeyInput == "showClock") {
    settingsKey = kNewTabPageShowClock;
  } else if (settingsKeyInput == "showStats") {
    settingsKey = kNewTabPageShowStats;
  } else if (settingsKeyInput == "showToday") {
    settingsKey = kNewTabPageShowToday;
  } else if (settingsKeyInput == "isPresearchTodayIntroDismissed") {
    settingsKey = kPresearchTodayIntroDismissed;
  } else if (settingsKeyInput == "showRewards") {
    settingsKey = kNewTabPageShowRewards;
  } else if (settingsKeyInput == "isBrandedWallpaperNotificationDismissed") {
    settingsKey = kBrandedWallpaperNotificationDismissed;
  } else if (settingsKeyInput == "showBinance") {
    settingsKey = kNewTabPageShowBinance;
  } else if (settingsKeyInput == "showTogether") {
    settingsKey = kNewTabPageShowTogether;
  } else if (settingsKeyInput == "showGemini") {
    settingsKey = kNewTabPageShowGemini;
#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
  } else if (settingsKeyInput == "showCryptoDotCom") {
    settingsKey = kCryptoDotComNewTabPageShowCryptoDotCom;
#endif
  } else {
    LOG(ERROR) << "Invalid setting key";
    return;
  }
  prefs->SetBoolean(settingsKey, settingsValueBool);

  // P3A can only be recorded after profile is updated
  if (settingsKeyInput == "showBackgroundImage" ||
      settingsKeyInput == "brandedWallpaperOptIn") {
    presearch::RecordSponsoredImagesEnabledP3A(profile_);
  }
}

void PresearchNewTabMessageHandler::HandleRegisterNewTabPageView(
    const base::ListValue* args) {
  AllowJavascript();

  // Decrement original value only if there's actual branded content
  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile_))
    service->RegisterPageView();
}

void PresearchNewTabMessageHandler::HandleBrandedWallpaperLogoClicked(
    const base::ListValue* args) {
  AllowJavascript();
  if (args->GetSize() != 1) {
    LOG(ERROR) << "Invalid input";
    return;
  }

  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile_)) {
    auto* creative_instance_id = args->GetList()[0].FindStringKey(
        ntp_background_images::kCreativeInstanceIDKey);
    auto* destination_url = args->GetList()[0].FindStringPath(
        ntp_background_images::kLogoDestinationURLPath);
    auto* wallpaper_id = args->GetList()[0].FindStringPath(
        ntp_background_images::kWallpaperIDKey);

    DCHECK(creative_instance_id);
    DCHECK(destination_url);
    DCHECK(wallpaper_id);

    service->BrandedWallpaperLogoClicked(
        creative_instance_id ? *creative_instance_id : "",
        destination_url ? *destination_url : "",
        wallpaper_id ? *wallpaper_id : "");
  }
}

void PresearchNewTabMessageHandler::HandleGetBrandedWallpaperData(
    const base::ListValue* args) {
  AllowJavascript();

  auto* service = ViewCounterServiceFactory::GetForProfile(profile_);
  auto data = service ? service->GetCurrentWallpaperForDisplay()
                      : base::Value();
  if (!data.is_none()) {
    DCHECK(data.is_dict());

    const std::string wallpaper_id = base::GenerateGUID();
    data.SetStringKey(ntp_background_images::kWallpaperIDKey, wallpaper_id);
    service->BrandedWallpaperWillBeDisplayed(wallpaper_id);
  }

  ResolveJavascriptCallback(args->GetList()[0], std::move(data));
}

void PresearchNewTabMessageHandler::HandleCustomizeClicked(
    const base::ListValue* args) {
  AllowJavascript();
  presearch::RecordValueIfGreater<NTPCustomizeUsage>(
      NTPCustomizeUsage::kOpened, "Presearch.NTP.CustomizeUsageStatus",
      kNTPCustomizeUsageStatus, g_browser_process->local_state());
}

void PresearchNewTabMessageHandler::HandleTodayInteractionBegin(
    const base::ListValue* args) {
  AllowJavascript();
  // Track if user has ever scrolled to Presearch Today.
  UMA_HISTOGRAM_EXACT_LINEAR("Presearch.Today.HasEverInteracted", 1, 1);
  // Track how many times in the past week
  // user has scrolled to Presearch Today.
  WeeklyStorage session_count_storage(
      profile_->GetPrefs(), kPresearchTodayWeeklySessionCount);
  session_count_storage.AddDelta(1);
  uint64_t total_session_count = session_count_storage.GetWeeklySum();
  constexpr int kSessionCountBuckets[] = {0, 1, 3, 7, 12, 18, 25, 1000};
  const int* it_count =
      std::lower_bound(kSessionCountBuckets, std::end(kSessionCountBuckets),
                      total_session_count);
  int answer = it_count - kSessionCountBuckets;
  UMA_HISTOGRAM_EXACT_LINEAR("Presearch.Today.WeeklySessionCount", answer,
                             base::size(kSessionCountBuckets) + 1);
}

void PresearchNewTabMessageHandler::HandleTodayOnCardVisit(
    const base::ListValue* args) {
  // Argument should be how many cards visited in this session.
  // We need the front-end to give us this since this class
  // will be destroyed and re-created when the user navigates "back",
  // but the front-end will have access to history state in order to
  // keep a count for the session.
  int cards_visited_total = args->GetList()[0].GetInt();
  // Track how many Presearch Today cards have been viewed per session
  // (each NTP / NTP Message Handler is treated as 1 session).
  WeeklyStorage storage(
      profile_->GetPrefs(), kPresearchTodayWeeklyCardVisitsCount);
  storage.ReplaceTodaysValueIfGreater(cards_visited_total);
  // Send the session with the highest count of cards viewed.
  uint64_t total = storage.GetHighestValueInWeek();
  constexpr int kBuckets[] = {0, 1, 3, 6, 10, 15, 100};
  const int* it_count =
      std::lower_bound(kBuckets, std::end(kBuckets),
                      total);
  int answer = it_count - kBuckets;
  UMA_HISTOGRAM_EXACT_LINEAR("Presearch.Today.WeeklyMaxCardVisitsCount", answer,
                             base::size(kBuckets) + 1);
  // Record ad click if a promoted card was read.
  if (args->GetSize() < 4) {
    return;
  }
  std::string item_id = args->GetList()[1].GetString();
  std::string creative_instance_id = args->GetList()[2].GetString();
  bool is_promoted = args->GetList()[3].GetBool();
  if (is_promoted && !item_id.empty() && !creative_instance_id.empty()) {
    auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile_);
    ads_service_->OnPromotedContentAdEvent(
        item_id, creative_instance_id,
        ads::mojom::PresearchAdsPromotedContentAdEventType::kClicked);
  }
}

void PresearchNewTabMessageHandler::HandleTodayOnCardViews(
    const base::ListValue* args) {
  // Argument should be how many cards viewed in this session.
  int cards_viewed_total = args->GetList()[0].GetInt();
  // Track how many Presearch Today cards have been viewed per session
  // (each NTP / NTP Message Handler is treated as 1 session).
  WeeklyStorage storage(
      profile_->GetPrefs(), kPresearchTodayWeeklyCardViewsCount);
  storage.ReplaceTodaysValueIfGreater(cards_viewed_total);
  // Send the session with the highest count of cards viewed.
  uint64_t total = storage.GetHighestValueInWeek();
  constexpr int kBuckets[] = {0, 1, 4, 12, 20, 40, 80, 1000};
  const int* it_count =
      std::lower_bound(kBuckets, std::end(kBuckets),
                      total);
  int answer = it_count - kBuckets;
  UMA_HISTOGRAM_EXACT_LINEAR("Presearch.Today.WeeklyMaxCardViewsCount", answer,
                             base::size(kBuckets) + 1);
}

void PresearchNewTabMessageHandler::HandleTodayOnPromotedCardView(
    const base::ListValue* args) {
  // Argument should be how many cards viewed in this session.
  std::string creative_instance_id = args->GetList()[0].GetString();
  std::string item_id = args->GetList()[1].GetString();
  if (!item_id.empty() && !creative_instance_id.empty()) {
    auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(profile_);
    ads_service_->OnPromotedContentAdEvent(
        item_id, creative_instance_id,
        ads::mojom::PresearchAdsPromotedContentAdEventType::kViewed);
  }
}

void PresearchNewTabMessageHandler::OnPrivatePropertiesChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPrivatePropertiesDictionary(prefs);
  FireWebUIListener("private-tab-data-updated", data);
}

void PresearchNewTabMessageHandler::OnStatsChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetStatsDictionary(prefs);
  FireWebUIListener("stats-updated", data);
}

void PresearchNewTabMessageHandler::OnPreferencesChanged() {
  PrefService* prefs = profile_->GetPrefs();
  auto data = GetPreferencesDictionary(prefs);
  FireWebUIListener("preferences-changed", data);
}

void PresearchNewTabMessageHandler::OnTorCircuitEstablished(bool result) {
  auto data = GetTorPropertiesDictionary(result, "");
  FireWebUIListener("tor-tab-data-updated", data);
}

void PresearchNewTabMessageHandler::OnTorInitializing(
    const std::string& percentage) {
  auto data = GetTorPropertiesDictionary(false, percentage);
  FireWebUIListener("tor-tab-data-updated", data);
}
