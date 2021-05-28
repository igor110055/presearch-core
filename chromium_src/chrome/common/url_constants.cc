/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/common/url_constants.h"

#include "build/branding_buildflags.h"
#include "chrome/common/webui_url_constants.h"

namespace chrome {

const char kAccessibilityLabelsLearnMoreURL[] =
    "https://support.presearch.com/";

const char kAutomaticSettingsResetLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017903152-How-do-I-reset-Presearch-settings-to-default-";

const char kAdvancedProtectionDownloadLearnMoreURL[] =
    "https://support.presearch.com";

const char kBluetoothAdapterOffHelpURL[] =
    "https://support.presearch.com/";

const char kCastCloudServicesHelpURL[] =
    "https://support.presearch.com/";

const char kCastNoDestinationFoundURL[] =
    "https://support.presearch.com/";

const char kChooserBluetoothOverviewURL[] =
    "https://support.presearch.com/";

const char kChooserHidOverviewUrl[] = "https://support.presearch.com/";

const char kChooserSerialOverviewUrl[] = "https://support.presearch.com/";

const char kChooserUsbOverviewURL[] =
    "https://support.presearch.com/";

const char kChromeBetaForumURL[] =
    "https://community.presearch.com/c/beta-builds";

const char kChromeFixUpdateProblems[] =
    "https://support.presearch.com/";

const char kChromeHelpViaKeyboardURL[] =
    "https://support.presearch.com/";

const char kChromeHelpViaMenuURL[] =
    "https://support.presearch.com/";

const char kChromeHelpViaWebUIURL[] =
    "https://support.presearch.com/";

const char kChromeNativeScheme[] = "chrome-native";

const char kChromeSearchLocalNtpHost[] = "local-ntp";
const char kChromeSearchLocalNtpUrl[] =
    "chrome-search://local-ntp/local-ntp.html";

const char kChromeSearchMostVisitedHost[] = "most-visited";
const char kChromeSearchMostVisitedUrl[] = "chrome-search://most-visited/";

const char kChromeSearchLocalNtpBackgroundUrl[] =
    "chrome-search://local-ntp/background.jpg";
const char kChromeSearchLocalNtpBackgroundFilename[] = "background.jpg";

const char kChromeSearchRemoteNtpHost[] = "remote-ntp";

const char kChromeSearchScheme[] = "chrome-search";

const char kChromeUIUntrustedNewTabPageUrl[] =
    "chrome-untrusted://new-tab-page/";

const char kChromiumProjectURL[] = "https://github.com/presearch/presearch-browser/";

const char kCloudPrintCertificateErrorLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017880792-How-do-I-print-from-Presearch-";

const char kContentSettingsExceptionsLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018205431-How-do-I-change-site-permissions-";

const char kCookiesSettingsHelpCenterURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018205431-How-do-I-change-site-permissions-";
const char kCpuX86Sse2ObsoleteURL[] =
    "https://support.presearch.com/";

const char kCrashReasonURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018192251-How-do-I-fix-page-crashes-and-other-page-loading-errors-";

const char kCrashReasonFeedbackDisplayedURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018192251-How-do-I-fix-page-crashes-and-other-page-loading-errors-";

const char kDoNotTrackLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017905612-How-do-I-turn-Do-Not-Track-on-or-off-";

const char kDownloadInterruptedLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018192491-How-do-I-fix-file-download-errors-";

const char kDownloadScanningLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018192491-How-do-I-fix-file-download-errors-";

const char kExtensionControlledSettingLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018185651-How-do-I-stop-extensions-from-changing-my-settings-";

const char kExtensionInvalidRequestURL[] = "chrome-extension://invalid/";

const char kFlashDeprecationLearnMoreURL[] =
    "https://blog.chromium.org/2017/07/so-long-and-thanks-for-all-flash.html";

const char kGoogleAccountActivityControlsURL[] =
    "https://support.presearch.com/";

const char kGoogleAccountURL[] = "https://support.presearch.com/";

const char kGoogleAccountChooserURL[] = "https://support.presearch.com/";

const char kGooglePasswordManagerURL[] = "https://support.presearch.com";

const char kLearnMoreReportingURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017905872-How-do-I-enable-or-disable-automatic-crash-reporting-";

const char kManagedUiLearnMoreUrl[] = "https://support.presearch.com/";

const char kMixedContentDownloadBlockingLearnMoreUrl[] =
    "https://support.presearch.com/";

const char kMyActivityUrlInClearBrowsingData[] =
    "https://support.presearch.com/";

const char kOmniboxLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017479752-How-do-I-set-my-default-search-engine-";

const char kPageInfoHelpCenterURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018185871-How-do-I-check-if-a-site-s-connection-is-secure-";

const char kPasswordCheckLearnMoreURL[] = "https://support.presearch.com/";

const char kPasswordGenerationLearnMoreURL[] = "https://support.presearch.com/";

const char kPasswordManagerLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018185951-How-do-I-use-the-built-in-password-manager-";

const char kPaymentMethodsURL[] = "https://support.presearch.com";

const char kPaymentMethodsLearnMoreURL[] =
    "https://support.presearch.com";

const char kPrivacyLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017989132-How-do-I-change-my-Privacy-Settings-";

const char kRemoveNonCWSExtensionURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017914832-Why-am-I-seeing-the-message-extensions-disabled-by-Presearch-";

const char kResetProfileSettingsLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017903152-How-do-I-reset-Presearch-settings-to-default-";

const char kSafeBrowsingHelpCenterURL[] =
    "https://support.presearch.com/";

const char kSafetyTipHelpCenterURL[] =
    "https://support.presearch.com/";
const char kSeeMoreSecurityTipsURL[] =
    "https://support.presearch.com/";

const char kSettingsSearchHelpURL[] =
    "https://support.presearch.com/";

const char kSyncAndGoogleServicesLearnMoreURL[] =
    "https://support.presearch.com/";

const char kSyncEncryptionHelpURL[] =
    "https://support.presearch.com/";

const char kSyncErrorsHelpURL[] =
    "https://support.presearch.com/";

const char kSyncGoogleDashboardURL[] =
    "https://support.presearch.com/";

const char kSyncLearnMoreURL[] =
    "https://support.presearch.com/";

const char kUpgradeHelpCenterBaseURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360025390311-How-do-I-download-and-install-Presearch-";

const char kWhoIsMyAdministratorHelpURL[] =
    "https://support.presearch.com/";

#if defined(OS_ANDROID)
const char kEnhancedPlaybackNotificationLearnMoreURL[] =
// Keep in sync with chrome/android/java/strings/android_chrome_strings.grd
    "https://community.presearch.com";
#endif

#if defined(OS_MAC)
const char kChromeEnterpriseSignInLearnMoreURL[] =
    "https://support.presearch.com/";

const char kMac10_10_ObsoleteURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360025390311-How-do-I-download-and-install-Presearch-";
#endif

#if defined(OS_WIN)
const char kChromeCleanerLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360017884152-How-do-I-remove-unwanted-ads-pop-ups-malware-";

const char kWindowsXPVistaDeprecationURL[] =
    "https://support.presearch.com/";
#endif

#if BUILDFLAG(ENABLE_ONE_CLICK_SIGNIN)
const char kChromeSyncLearnMoreURL[] =
    "https://support.presearch.com/";
#endif  // BUILDFLAG(ENABLE_ONE_CLICK_SIGNIN)

#if BUILDFLAG(ENABLE_PLUGINS)
const char kOutdatedPluginLearnMoreURL[] =
    "https://support.presearch.com/hc/en-us/articles/"
    "360018163151-How-do-I-manage-Flash-audio-video-";
#endif

}  // namespace chrome
