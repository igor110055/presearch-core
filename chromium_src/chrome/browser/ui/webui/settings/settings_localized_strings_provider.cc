/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/settings/settings_localized_strings_provider.h"

#include "base/stl_util.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"
#include "presearch/browser/ui/webui/settings/presearch_privacy_handler.h"
#include "presearch/browser/version_info.h"
#include "presearch/components/ipfs/ipfs_constants.h"
#include "presearch/components/ipfs/pref_names.h"
#include "presearch/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "chrome/common/pref_names.h"
#include "components/grit/presearch_components_strings.h"
#include "components/prefs/pref_service.h"

namespace settings {
void PresearchAddLocalizedStrings(content::WebUIDataSource*, Profile*);
}  // namespace settings

// Override some chromium strings
#include "presearch/grit/presearch_generated_resources.h"
#include "chrome/grit/chromium_strings.h"
#include "chrome/grit/generated_resources.h"
#include "extensions/common/extension_urls.h"

#undef IDS_SETTINGS_CUSTOMIZE_PROFILE
#define IDS_SETTINGS_CUSTOMIZE_PROFILE IDS_SETTINGS_PRESEARCH_EDIT_PROFILE
#undef IDS_SETTINGS_CUSTOMIZE_YOUR_CHROME_PROFILE
#define IDS_SETTINGS_CUSTOMIZE_YOUR_CHROME_PROFILE \
  IDS_SETTINGS_PRESEARCH_EDIT_PROFILE
#undef IDS_SETTINGS_SAFEBROWSING_STANDARD_BULLET_TWO
#define IDS_SETTINGS_SAFEBROWSING_STANDARD_BULLET_TWO \
  IDS_SETTINGS_PRESEARCH_SAFEBROWSING_STANDARD_BULLET_TWO
#undef IDS_SETTINGS_SAFEBROWSING_NONE_DESC
#define IDS_SETTINGS_SAFEBROWSING_NONE_DESC \
  IDS_SETTINGS_PRESEARCH_SAFEBROWSING_NONE_DESC

#define GetVersionNumber GetPresearchVersionNumberForDisplay

#include "../../../../../../../chrome/browser/ui/webui/settings/settings_localized_strings_provider.cc"
#undef GetVersionNumber

#include "presearch/browser/ui/webui/presearch_settings_ui.h"
namespace settings {

const char kWebRTCLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017989132-How-do-I-change-my-Privacy-Settings-#webrtc";
const char kPresearchBuildInstructionsUrl[] =
    "https://github.com/presearch/presearch-browser/wiki";
const char kPresearchLicenseUrl[] = "https://mozilla.org/MPL/2.0/";
const char kPresearchReleaseTagPrefix[] =
    "https://github.com/presearch/presearch-browser/releases/tag/v";
const char kGoogleLoginLearnMoreURL[] =
    "https://github.com/presearch/presearch-browser/wiki/"
    "Allow-Google-login---Third-Parties-and-Extensions";
const char kDNSLinkLearnMoreURL[] = "https://docs.ipfs.io/concepts/dnslink/";
const char kUnstoppableDomainsLearnMoreURL[] =
    "https://github.com/presearch/presearch-browser/wiki/"
    "Resolve-Methods-for-Unstoppable-Domains";

void PresearchAddCommonStrings(content::WebUIDataSource* html_source,
                           Profile* profile) {
  webui::LocalizedString localized_strings[] = {
    {"importExtensions", IDS_SETTINGS_IMPORT_EXTENSIONS_CHECKBOX},
    {"importPayments", IDS_SETTINGS_IMPORT_PAYMENTS_CHECKBOX},
    {"siteSettingsAutoplay", IDS_SETTINGS_SITE_SETTINGS_AUTOPLAY},
    {"siteSettingsCategoryAutoplay", IDS_SETTINGS_SITE_SETTINGS_AUTOPLAY},
    {"presearchGetStartedTitle", IDS_SETTINGS_PRESEARCH_GET_STARTED_TITLE},
    {"presearchAdditionalSettingsTitle", IDS_SETTINGS_PRESEARCH_ADDITIONAL_SETTINGS},
    {"appearanceSettingsPresearchTheme",
     IDS_SETTINGS_APPEARANCE_SETTINGS_PRESEARCH_THEMES},
    {"appearanceSettingsLocationBarIsWide",
     IDS_SETTINGS_APPEARANCE_SETTINGS_LOCATION_BAR_IS_WIDE},
    {"appearanceSettingsHidePresearchRewardsButtonLabel",
     IDS_SETTINGS_HIDE_PRESEARCH_REWARDS_BUTTON_LABEL},
    {"appearanceSettingsAlwaysShowBookmarkBarOnNTP",
     IDS_SETTINGS_ALWAYS_SHOW_BOOKMARK_BAR_ON_NTP},
    {"appearanceSettingsShowAutocompleteInAddressBar",
     IDS_SETTINGS_APPEARANCE_SETTINGS_SHOW_AUTOCOMPLETE_IN_ADDRESS_BAR},
    {"appearanceSettingsUseTopSiteSuggestions",
     IDS_SETTINGS_APPEARANCE_SETTINGS_USE_AUTOCOMPLETE_TOP_SITES},
    {"appearanceSettingsUsePresearchSuggestedSiteSuggestions",
     IDS_SETTINGS_APPEARANCE_SETTINGS_USE_AUTOCOMPLETE_PRESEARCH_SUGGESTED_SITES},
    {"appearanceSettingsGetMoreThemes",
     IDS_SETTINGS_APPEARANCE_SETTINGS_GET_MORE_THEMES},
    {"appearancePresearchDefaultImagesOptionLabel",
     IDS_SETTINGS_APPEARANCE_SETTINGS_PRESEARCH_DEFAULT_IMAGES_OPTION_LABEL},
#if BUILDFLAG(ENABLE_SIDEBAR)
    {"appearanceSettingsShowOptionTitle", IDS_SIDEBAR_SHOW_OPTION_TITLE},
    {"appearanceSettingsShowOptionAlways", IDS_SIDEBAR_SHOW_OPTION_ALWAYS},
    {"appearanceSettingsShowOptionMouseOver",
     IDS_SIDEBAR_SHOW_OPTION_MOUSEOVER},
    {"appearanceSettingsShowOptionOnClick", IDS_SIDEBAR_SHOW_OPTION_ONCLICK},
    {"appearanceSettingsShowOptionNever", IDS_SIDEBAR_SHOW_OPTION_NEVER},
    {"appearanceSettingsSidebarEnabledDesc",
     IDS_SETTINGS_APPEARANCE_SETTINGS_SIDEBAR_ENABLED_DESC},
    {"appearanceSettingsSidebarDisabledDesc",
     IDS_SETTINGS_APPEARANCE_SETTINGS_SIDEBAR_DISABLED_DESC},
#endif
    {"mruCyclingSettingLabel", IDS_SETTINGS_PRESEARCH_MRU_CYCLING_LABEL},
    {"presearchShieldsTitle", IDS_SETTINGS_PRESEARCH_SHIELDS_TITLE},
    {"presearchShieldsDefaultsSectionTitle",
     IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_TITLE},
    {"presearchShieldsDefaultsSectionDescription1",
     IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_DESCRIPTION_1},
    {"presearchShieldsDefaultsSectionDescription2",
     IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_DESCRIPTION_2},
    {"presearchShieldsDefaultsSectionDescription3",
     IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_DESCRIPTION_3},
    {"socialBlocking", IDS_SETTINGS_SOCIAL_BLOCKING_DEFAULTS_TITLE},
    {"defaultView", IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_DEFAULT_VIEW_LABEL},
    {"simpleView", IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_SIMPLE_VIEW_LABEL},
    {"advancedView", IDS_SETTINGS_PRESEARCH_SHIELDS_DEFAULTS_ADVANCED_VIEW_LABEL},
    {"adControlLabel", IDS_SETTINGS_PRESEARCH_SHIELDS_AD_CONTROL_LABEL},
    {"cookieControlLabel", IDS_SETTINGS_PRESEARCH_SHIELDS_COOKIE_CONTROL_LABEL},
    {"fingerprintingControlLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_FINGERPRINTING_CONTROL_LABEL},
    {"httpsEverywhereControlLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_HTTPS_EVERYWHERE_CONTROL_LABEL},
    {"noScriptControlLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_NO_SCRIPT_CONTROL_LABEL},
    {"shieldsLookFeelTitle", IDS_SETTINGS_PRESEARCH_SHIELDS_LOOK_AND_FEEL_TITLE},
    {"showStatsBlockedBadgeLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_SHOW_STATS_BLOCKED_BADGE_LABEL},
    {"googleLoginControlLabel", IDS_SETTINGS_PRESEARCH_SHIELDS_GOOGLE_LOGIN_LABEL},
    {"fbEmbedControlLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_FACEBOOK_EMBEDDED_POSTS_LABEL},
    {"twitterEmbedControlLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_TWITTER_EMBEDDED_TWEETS_LABEL},
    {"linkedInEmbedControlLabel",
     IDS_SETTINGS_PRESEARCH_SHIELDS_LINKEDIN_EMBEDDED_POSTS_LABEL},
    {"blockAdsTrackersAggressive", IDS_SETTINGS_BLOCK_ADS_TRACKERS_AGGRESSIVE},
    {"blockAdsTrackersStandard", IDS_SETTINGS_BLOCK_ADS_TRACKERS_STANDARD},
    {"allowAdsTrackers", IDS_SETTINGS_ALLOW_ADS_TRACKERS},
    {"block3rdPartyCookies", IDS_SETTINGS_BLOCK_3RD_PARTY_COOKIES},
    {"allowAllCookies", IDS_SETTINGS_ALLOW_ALL_COOKIES},
    {"blockAllCookies", IDS_SETTINGS_BLOCK_ALL_COOKIES},
    {"standardFingerprinting", IDS_SETTINGS_STANDARD_FINGERPRINTING},
    {"allowAllFingerprinting", IDS_SETTINGS_ALLOW_ALL_FINGERPRINTING},
    {"strictFingerprinting", IDS_SETTINGS_STRICT_FINGERPRINTING},
    {"webRTCPolicyLabel", IDS_SETTINGS_WEBRTC_POLICY_LABEL},
    {"webRTCPolicySubLabel", IDS_SETTINGS_WEBRTC_POLICY_SUB_LABEL},
    {"webRTCDefault", IDS_SETTINGS_WEBRTC_POLICY_DEFAULT},
    {"pushMessagingLabel", IDS_SETTINGS_PUSH_MESSAGING},
    {"defaultPublicAndPrivateInterfaces",
     IDS_SETTINGS_WEBRTC_POLICY_DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES},
    {"defaultPublicInterfaceOnly",
     IDS_SETTINGS_WEBRTC_POLICY_DEFAULT_PUBLIC_INTERFACE_ONLY},
    {"disableNonProxiedUdp",
     IDS_SETTINGS_WEBRTC_POLICY_DISABLE_NON_PROXIED_UDP},
    {"presearchSync", IDS_SETTINGS_PRESEARCH_SYNC_TITLE},
    {"presearchSyncSetupActionLabel", IDS_SETTINGS_PRESEARCH_SYNC_SETUP_ACTION_LABEL},
    {"presearchSyncSetupTitle", IDS_SETTINGS_PRESEARCH_SYNC_SETUP_TITLE},
    {"presearchSyncSetupSubtitle", IDS_SETTINGS_PRESEARCH_SYNC_SETUP_SUBTITLE},
    {"presearchSyncManageActionLabel", IDS_SETTINGS_PRESEARCH_SYNC_MANAGE_ACTION_LABEL},
    {"presearchSyncWordCount", IDS_SETTINGS_PRESEARCH_SYNC_WORD_COUNT},
    {"presearchSyncCopied", IDS_SETTINGS_PRESEARCH_SYNC_COPIED_TEXT},
    {"presearchSyncQRCodeAlt", IDS_SETTINGS_PRESEARCH_SYNC_QR_IMAGE_ALT},
    {"presearchSyncQRError", IDS_SETTINGS_PRESEARCH_SYNC_QR_ERROR},
    {"presearchSyncManagerTitle", IDS_SETTINGS_PRESEARCH_SYNC_MANAGER_TITLE},
    {"presearchSyncSettingsTitle", IDS_SETTINGS_PRESEARCH_SYNC_SETTINGS_TITLE},
    {"presearchSyncSettingsSubtitle", IDS_SETTINGS_PRESEARCH_SYNC_SETTINGS_SUBTITLE},
    {"presearchSyncDeviceListTitle", IDS_SETTINGS_PRESEARCH_SYNC_DEVICE_LIST_TITLE},
    {"presearchSyncDeviceListSubtitle",
     IDS_SETTINGS_PRESEARCH_SYNC_DEVICE_LIST_SUBTITLE},
    {"presearchSyncDeviceListNameColumn",
     IDS_SETTINGS_PRESEARCH_SYNC_DEVICE_LIST_NAME_COLUMN},
    {"presearchSyncDeviceListNameThisDevice",
     IDS_SETTINGS_PRESEARCH_SYNC_DEVICE_LIST_NAME_THIS_DEVICE},
    {"presearchSyncDeviceListLastActiveColumn",
     IDS_SETTINGS_PRESEARCH_SYNC_DEVICE_LIST_LAST_ACTIVE_COLUMN},
    {"presearchSyncDeviceListRemoveColumn",
     IDS_SETTINGS_PRESEARCH_SYNC_DEVICE_LIST_REMOVE_COLUMN},
    {"presearchSyncSetupTitle", IDS_PRESEARCH_SYNC_SETUP_TITLE},
    {"presearchSyncSetupDesc", IDS_PRESEARCH_SYNC_SETUP_DESCRIPTION},
    {"presearchSyncStartNewChain", IDS_PRESEARCH_SYNC_START_NEW_CHAIN_BUTTON},
    {"presearchSyncEnterCode", IDS_PRESEARCH_SYNC_ENTER_CODE_BUTTON},
    {"presearchSyncChooseDeviceMobileTitle",
     IDS_PRESEARCH_SYNC_CHOOSE_DEVICE_MOBILE_TITLE},
    {"presearchSyncChooseDeviceComputerTitle",
     IDS_PRESEARCH_SYNC_CHOOSE_DEVICE_COMPUTER_TITLE},
    {"presearchSyncScanCodeTitle", IDS_PRESEARCH_SYNC_SCAN_CODE_TITLE},
    {"presearchSyncScanCodeDesc1", IDS_PRESEARCH_SYNC_SCAN_CODE_DESCRIPTION_PARTIAL_1},
    {"presearchSyncScanCodeDesc2", IDS_PRESEARCH_SYNC_SCAN_CODE_DESCRIPTION_PARTIAL_2},
    {"presearchSyncScanCodeDesc3", IDS_PRESEARCH_SYNC_SCAN_CODE_DESCRIPTION_PARTIAL_3},
    {"presearchSyncViewCodeTitle", IDS_PRESEARCH_SYNC_VIEW_CODE_TITLE},
    {"presearchSyncViewCodeDesc1", IDS_PRESEARCH_SYNC_VIEW_CODE_DESCRIPTION_PARTIAL_1},
    {"presearchSyncViewCodeDesc2", IDS_PRESEARCH_SYNC_VIEW_CODE_DESCRIPTION_PARTIAL_2},
    {"presearchSyncViewCodeDesc3", IDS_PRESEARCH_SYNC_VIEW_CODE_DESCRIPTION_PARTIAL_3},
    {"presearchSyncCodeWarning", IDS_PRESEARCH_SYNC_CODE_WARNING},
    {"presearchSyncViewCodeQRCodeButton", IDS_PRESEARCH_SYNC_VIEW_CODE_QR_CODE_BUTTON},
    {"presearchSyncEnterCodeTitle", IDS_PRESEARCH_SYNC_ENTER_CODE_TITLE},
    {"presearchSyncEnterCodeDesc", IDS_PRESEARCH_SYNC_ENTER_CODE_DESCRIPTION},
    {"presearchSyncViewCodeButton", IDS_PRESEARCH_SYNC_VIEW_CODE_BUTTON},
    {"presearchSyncAddDevice", IDS_PRESEARCH_SYNC_ADD_DEVICE_BUTTON},
    {"presearchSyncChooseDeviceTitle", IDS_PRESEARCH_SYNC_CHOOSE_DEVICE_TITLE},
    {"presearchSyncChooseDeviceDesc", IDS_PRESEARCH_SYNC_CHOOSE_DEVICE_DESCRIPTION},
    {"presearchSyncInvalidSyncCodeTitle", IDS_PRESEARCH_SYNC_INVALID_SYNC_CODE_TITLE},
    {"presearchSyncResetButton", IDS_PRESEARCH_SYNC_RESET_BUTTON},
    {"presearchSyncResetConfirmation", IDS_PRESEARCH_SYNC_RESET_CONFIRMATION},
    {"presearchSyncDeleteDeviceConfirmation",
     IDS_PRESEARCH_SYNC_DELETE_DEVICE_CONFIRMATION},
    {"presearchIPFS", IDS_PRESEARCH_IPFS_SETTINGS_SECTION},
    {"presearchWallet", IDS_PRESEARCH_WALLET_SETTINGS_SECTION},
    {"presearchHelpTips", IDS_SETTINGS_HELP_TIPS},
    {"presearchHelpTipsWaybackMachineLabel",
     IDS_SETTINGS_HELP_TIPS_SHOW_PRESEARCH_WAYBACK_MACHINE_PROMPT},
    // New Tab Page
    {"presearchNewTab", IDS_SETTINGS_NEW_TAB},
    {"presearchNewTabPresearchRewards", IDS_SETTINGS_NEW_TAB_PRESEARCH_REWARDS},
    {"presearchNewTabNewTabPageShows", IDS_SETTINGS_NEW_TAB_NEW_TAB_PAGE_SHOWS},
    {"presearchNewTabNewTabCustomizeWidgets",
     IDS_SETTINGS_NEW_TAB_NEW_TAB_CUSTOMIZE_WIDGETS},
    // Misc (TODO: Organize this)
    {"onExitPageTitle", IDS_SETTINGS_PRESEARCH_ON_EXIT},
    {"presearchDefaultExtensions", IDS_SETTINGS_PRESEARCH_DEFAULT_EXTENSIONS_TITLE},
    {"webTorrentEnabledDesc", IDS_SETTINGS_WEBTORRENT_ENABLED_DESC},
    {"presearchWeb3ProviderDesc", IDS_SETTINGS_PRESEARCH_WEB3_PROVIDER_DESC},
    {"loadCryptoWalletsOnStartupDesc",
     IDS_SETTINGS_LOAD_CRYPTO_WALLETS_ON_STARTUP},
    {"loadCryptoWalletsOnStartupDescDeprecated",
     IDS_SETTINGS_LOAD_CRYPTO_WALLETS_ON_STARTUP_DEPRECATED},
    {"googleLoginForExtensionsDesc", IDS_SETTINGS_GOOGLE_LOGIN_FOR_EXTENSIONS},
    {"hangoutsEnabledDesc", IDS_SETTINGS_HANGOUTS_ENABLED_DESC},
    {"resolveUnstoppableDomainsDesc",
     IDS_SETTINGS_RESOLVE_UNSTOPPABLE_DOMAINS_DESC},
    {"resolveENSDesc", IDS_SETTINGS_RESOLVE_ENS_DESC},
    {"resolveIPFSURLDesc", IDS_SETTINGS_RESOLVE_IPFS_URLS_DESC},
    {"ipfsPublicGatewayDesc", IDS_SETTINGS_IPFS_PUBLIC_GATEWAY_DESC},
    {"ipfsChangeGatewayButtonLabel",
     IDS_SETTINGS_IPFS_CHANGE_GATEWAY_BUTTON_LABEL},
    {"changeIpfsGatewayDialogTitle",
     IDS_SETTINGS_CHANGE_IPFS_GATEWAY_DIALOG_TITLE},
    {"changeIpfsGatewayDialogLabel",
     IDS_SETTINGS_CHANGE_IPFS_GATEWAY_DIALOG_LABEL},
    {"changeIpfsStorageMaxLabel", IDS_SETTINGS_CHANGE_IPFS_STORAGE_MAX_LABEL},
    {"changeIpfsStorageMaxDesc", IDS_SETTINGS_CHANGE_IPFS_STORAGE_MAX_DESC},
    {"ipfsErrorInvalidAddress", IDS_SETTINGS_IPFS_ERROR_INVALID_ADDRESS},
    {"ipfsAutoFallbackToGatewayLabel",
     IDS_SETTINGS_IPFS_AUTO_FALLBACK_TO_GATEWAY_LABEL},
    {"ipfsAutoFallbackToGatewayDesc",
     IDS_SETTINGS_IPFS_AUTO_FALLBACK_TO_GATEWAY_DESC},
    {"ipfsAutoRedirectGatewayLabel",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_GATEWAY_LABEL},
    {"ipfsAutoRedirectGatewayDesc",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_GATEWAY_DESC},
    {"ipfsAutoRedirectDNSLinkLabel",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_DNSLINK_RESOURCES_LABEL},
    {"ipfsAutoRedirectDNSLinkDesc",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_DNSLINK_RESOURCES_DESC},
    {"ipfsCompanionEnabledDesc", IDS_SETTINGS_IPFS_COMPANION_ENABLED_DESC},
    {"mediaRouterEnabledDesc", IDS_SETTINGS_MEDIA_ROUTER_ENABLED_DESC},
    {"torEnabledLabel", IDS_SETTINGS_ENABLE_TOR_TITLE},
    {"torEnabledDesc", IDS_SETTINGS_ENABLE_TOR_DESC},
    {"autoOnionLocationLabel", IDS_SETTINGS_AUTO_ONION_LOCATION_TITLE},
    {"autoOnionLocationDesc", IDS_SETTINGS_AUTO_ONION_LOCATION_DESC},
    {"widevineEnabledDesc", IDS_SETTINGS_ENABLE_WIDEVINE_DESC},
    {"restartNotice", IDS_SETTINGS_RESTART_NOTICE},
    {"relaunchButtonLabel", IDS_SETTINGS_RELAUNCH_BUTTON_LABEL},
    {"manageExtensionsLabel", IDS_SETTINGS_MANAGE_EXTENSIONS_LABEL},
    {"keyboardShortcuts", IDS_EXTENSIONS_SIDEBAR_KEYBOARD_SHORTCUTS},
    {"getMoreExtensionsLabel", IDS_PRESEARCH_SETTINGS_GET_MORE_EXTENSIONS_LABEL},
    {"getMoreExtensionsSubLabel", IDS_EXTENSIONS_SIDEBAR_OPEN_CHROME_WEB_STORE},
    {"p3aEnableTitle", IDS_PRESEARCH_P3A_ENABLE_SETTING},
    {"p3aEnabledDesc", IDS_PRESEARCH_P3A_ENABLE_SETTING_SUBITEM},
    {"siteSettings", IDS_SETTINGS_SITE_AND_SHIELDS_SETTINGS},
    {"resetRewardsData", IDS_SETTINGS_RESET_REWARDS_DATA},
    {"showFullUrls", IDS_CONTEXT_MENU_SHOW_FULL_URLS},
  };
  html_source->AddLocalizedStrings(localized_strings);
  html_source->AddString("webRTCLearnMoreURL",
                         base::ASCIIToUTF16(kWebRTCLearnMoreURL));
  html_source->AddString("googleLoginLearnMoreURL",
                         base::ASCIIToUTF16(kGoogleLoginLearnMoreURL));
  html_source->AddString("ipfsDNSLinkLearnMoreURL",
                         base::UTF8ToUTF16(kDNSLinkLearnMoreURL));
  html_source->AddString(
      "getMoreExtensionsUrl",
      base::ASCIIToUTF16(
          google_util::AppendGoogleLocaleParam(
              GURL(extension_urls::GetWebstoreExtensionsCategoryURL()),
              g_browser_process->GetApplicationLocale())
              .spec()));
  html_source->AddString(
      "ipfsStorageMaxValue",
      std::to_string(profile->GetPrefs()->GetInteger(kIpfsStorageMax)));

  base::string16 ipfs_method_desc =
      l10n_util::GetStringFUTF16(IDS_SETTINGS_IPFS_METHOD_DESC,
                                 base::ASCIIToUTF16(ipfs::kIPFSLearnMoreURL));
  html_source->AddString("ipfsMethodDesc", ipfs_method_desc);

  html_source->AddString(
      "resolveUnstoppableDomainsSubDesc",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_RESOLVE_UNSTOPPABLE_DOMAINS_SUB_DESC,
          base::ASCIIToUTF16(kUnstoppableDomainsLearnMoreURL)));
}

void PresearchAddResources(content::WebUIDataSource* html_source,
                       Profile* profile) {
  PresearchSettingsUI::AddResources(html_source, profile);
}

void PresearchAddAboutStrings(content::WebUIDataSource* html_source,
                          Profile* profile) {
  base::string16 license = l10n_util::GetStringFUTF16(
      IDS_PRESEARCH_VERSION_UI_LICENSE, base::ASCIIToUTF16(kPresearchLicenseUrl),
      base::ASCIIToUTF16(chrome::kChromeUICreditsURL),
      base::ASCIIToUTF16(kPresearchBuildInstructionsUrl),
      base::ASCIIToUTF16(kPresearchReleaseTagPrefix) +
          base::UTF8ToUTF16(
              version_info::GetPresearchVersionWithoutChromiumMajorVersion()));
  html_source->AddString("aboutProductLicense", license);
}

void PresearchAddSocialBlockingLoadTimeData(content::WebUIDataSource* html_source,
                                        Profile* profile) {
  html_source->AddBoolean(
      "signInAllowedOnNextStartupInitialValue",
      profile->GetPrefs()->GetBoolean(prefs::kSigninAllowedOnNextStartup));
}

void PresearchAddLocalizedStrings(content::WebUIDataSource* html_source,
                              Profile* profile) {
  PresearchAddCommonStrings(html_source, profile);
  PresearchAddResources(html_source, profile);
  PresearchAddAboutStrings(html_source, profile);
  PresearchPrivacyHandler::AddLoadTimeData(html_source, profile);
  PresearchAddSocialBlockingLoadTimeData(html_source, profile);
}

}  // namespace settings
