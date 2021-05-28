/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/common/url_constants.h"

#include "build/branding_buildflags.h"
#include "chrome/common/webui_url_constants.h"

namespace chrome {

const char kAccessibilityLabelsLearnMoreURL[] =
    "https://support.presearch.org/";

const char kAutomaticSettingsResetLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017903152-How-do-I-reset-Presearch-settings-to-default-";

const char kAdvancedProtectionDownloadLearnMoreURL[] =
    "https://support.presearch.org";

const char kBluetoothAdapterOffHelpURL[] =
    "https://support.presearch.org/";

const char kCastCloudServicesHelpURL[] =
    "https://support.presearch.org/";

const char kCastNoDestinationFoundURL[] =
    "https://support.presearch.org/";

const char kChooserBluetoothOverviewURL[] =
    "https://support.presearch.org/";

const char kChooserHidOverviewUrl[] = "https://support.presearch.org/";

const char kChooserSerialOverviewUrl[] = "https://support.presearch.org/";

const char kChooserUsbOverviewURL[] =
    "https://support.presearch.org/";

const char kChromeBetaForumURL[] =
    "https://community.presearch.org/c/beta-builds";

const char kChromeFixUpdateProblems[] =
    "https://support.presearch.org/";

const char kChromeHelpViaKeyboardURL[] =
    "https://support.presearch.org/";

const char kChromeHelpViaMenuURL[] =
    "https://support.presearch.org/";

const char kChromeHelpViaWebUIURL[] =
    "https://support.presearch.org/";

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

const char kChromiumProjectURL[] = "https://github.com/brave/presearch-browser/";

const char kCloudPrintCertificateErrorLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017880792-How-do-I-print-from-Presearch-";

const char kContentSettingsExceptionsLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018205431-How-do-I-change-site-permissions-";

const char kCookiesSettingsHelpCenterURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018205431-How-do-I-change-site-permissions-";
const char kCpuX86Sse2ObsoleteURL[] =
    "https://support.presearch.org/";

const char kCrashReasonURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018192251-How-do-I-fix-page-crashes-and-other-page-loading-errors-";

const char kCrashReasonFeedbackDisplayedURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018192251-How-do-I-fix-page-crashes-and-other-page-loading-errors-";

const char kDoNotTrackLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017905612-How-do-I-turn-Do-Not-Track-on-or-off-";

const char kDownloadInterruptedLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018192491-How-do-I-fix-file-download-errors-";

const char kDownloadScanningLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018192491-How-do-I-fix-file-download-errors-";

const char kExtensionControlledSettingLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018185651-How-do-I-stop-extensions-from-changing-my-settings-";

const char kExtensionInvalidRequestURL[] = "chrome-extension://invalid/";

const char kFlashDeprecationLearnMoreURL[] =
    "https://blog.chromium.org/2017/07/so-long-and-thanks-for-all-flash.html";

const char kGoogleAccountActivityControlsURL[] =
    "https://support.presearch.org/";

const char kGoogleAccountURL[] = "https://support.presearch.org/";

const char kGoogleAccountChooserURL[] = "https://support.presearch.org/";

const char kGooglePasswordManagerURL[] = "https://support.presearch.org";

const char kLearnMoreReportingURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017905872-How-do-I-enable-or-disable-automatic-crash-reporting-";

const char kManagedUiLearnMoreUrl[] = "https://support.presearch.org/";

const char kMixedContentDownloadBlockingLearnMoreUrl[] =
    "https://support.presearch.org/";

const char kMyActivityUrlInClearBrowsingData[] =
    "https://support.presearch.org/";

const char kOmniboxLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017479752-How-do-I-set-my-default-search-engine-";

const char kPageInfoHelpCenterURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018185871-How-do-I-check-if-a-site-s-connection-is-secure-";

const char kPasswordCheckLearnMoreURL[] = "https://support.presearch.org/";

const char kPasswordGenerationLearnMoreURL[] = "https://support.presearch.org/";

const char kPasswordManagerLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018185951-How-do-I-use-the-built-in-password-manager-";

const char kPaymentMethodsURL[] = "https://support.presearch.org";

const char kPaymentMethodsLearnMoreURL[] =
    "https://support.presearch.org";

const char kPrivacyLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017989132-How-do-I-change-my-Privacy-Settings-";

const char kRemoveNonCWSExtensionURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017914832-Why-am-I-seeing-the-message-extensions-disabled-by-Presearch-";

const char kResetProfileSettingsLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017903152-How-do-I-reset-Presearch-settings-to-default-";

const char kSafeBrowsingHelpCenterURL[] =
    "https://support.presearch.org/";

const char kSafetyTipHelpCenterURL[] =
    "https://support.presearch.org/";
const char kSeeMoreSecurityTipsURL[] =
    "https://support.presearch.org/";

const char kSettingsSearchHelpURL[] =
    "https://support.presearch.org/";

const char kSyncAndGoogleServicesLearnMoreURL[] =
    "https://support.presearch.org/";

const char kSyncEncryptionHelpURL[] =
    "https://support.presearch.org/";

const char kSyncErrorsHelpURL[] =
    "https://support.presearch.org/";

const char kSyncGoogleDashboardURL[] =
    "https://support.presearch.org/";

const char kSyncLearnMoreURL[] =
    "https://support.presearch.org/";

const char kUpgradeHelpCenterBaseURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360025390311-How-do-I-download-and-install-Presearch-";

const char kWhoIsMyAdministratorHelpURL[] =
    "https://support.presearch.org/";

#if defined(OS_ANDROID)
const char kEnhancedPlaybackNotificationLearnMoreURL[] =
// Keep in sync with chrome/android/java/strings/android_chrome_strings.grd
    "https://community.presearch.org";
#endif

#if defined(OS_MAC)
const char kChromeEnterpriseSignInLearnMoreURL[] =
    "https://support.presearch.org/";

const char kMac10_10_ObsoleteURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360025390311-How-do-I-download-and-install-Presearch-";
#endif

#if defined(OS_WIN)
const char kChromeCleanerLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360017884152-How-do-I-remove-unwanted-ads-pop-ups-malware-";

const char kWindowsXPVistaDeprecationURL[] =
    "https://support.presearch.org/";
#endif

#if BUILDFLAG(ENABLE_ONE_CLICK_SIGNIN)
const char kChromeSyncLearnMoreURL[] =
    "https://support.presearch.org/";
#endif  // BUILDFLAG(ENABLE_ONE_CLICK_SIGNIN)

#if BUILDFLAG(ENABLE_PLUGINS)
const char kOutdatedPluginLearnMoreURL[] =
    "https://support.presearch.org/hc/en-us/articles/"
    "360018163151-How-do-I-manage-Flash-audio-video-";
#endif

}  // namespace chrome
