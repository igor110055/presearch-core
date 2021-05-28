/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../../../../chrome/browser/flag_descriptions.cc"

namespace flag_descriptions {
const char kUseDevUpdaterUrlName[] =
    "Use dev updater url";
const char kUseDevUpdaterUrlDescription[] =
    "Use the dev url for the component updater. "
    "This is for internal testing only.";
const char kPresearchNTPBrandedWallpaperName[] =
    "New Tab Page Branded Wallpapers";
const char kPresearchNTPBrandedWallpaperDescription[] =
    "Allow New Tab Page Branded Wallpapers and user preference.";
const char kPresearchNTPBrandedWallpaperDemoName[] =
    "New Tab Page Demo Branded Wallpaper";
const char kPresearchNTPBrandedWallpaperDemoDescription[] =
    "Force dummy data for the Branded Wallpaper New Tab Page Experience. "
    "View rate and user opt-in conditionals will still be followed to decide "
    "when to display the Branded Wallpaper.";
const char kPresearchAdblockCosmeticFilteringName[] = "Enable cosmetic filtering";
const char kPresearchAdblockCosmeticFilteringDescription[] =
    "Enable support for cosmetic filtering";
const char kPresearchAdblockCosmeticFilteringNativeName[] =
    "Use native implementation for cosmetic filtering";
const char kPresearchAdblockCosmeticFilteringNativeDescription[] =
    "Uses native implementation for cosmetic filtering instead of extension";
const char kPresearchAdblockCspRulesName[] = "Enable support for CSP rules";
const char kPresearchAdblockCspRulesDescription[] =
    "Applies additional CSP rules to pages for which a $csp rule has been "
    "loaded from a filter list";
const char kPresearchDomainBlockName[] = "Enable domain blocking";
const char kPresearchDomainBlockDescription[] =
    "Enable support for blocking domains with an interstitial page";
const char kPresearchExtensionNetworkBlockingName[] =
    "Enable extension network blocking";
const char kPresearchExtensionNetworkBlockingDescription[] =
    "Enable blocking for network requests initiated by extensions";
const char kPresearchSidebarName[] = "Enable Sidebar";
// TODO(simon): Use more better description.
const char kPresearchSidebarDescription[] = "Enable Sidebar";
const char kPresearchSpeedreaderName[] = "Enable SpeedReader";
const char kPresearchSpeedreaderDescription[] =
    "Enables faster loading of simplified article-style web pages.";
const char kPresearchSpeedreaderReadabilityName[] =
    "Enable Readability Backend for SpeedReader";
const char kPresearchSpeedreaderReadabilityDescription[] =
    "Enables the readability backend for SpeedReader. Runs a classifier to "
    "determine if pages are readable and distills using the Arc90 algorithm.";
const char kPresearchSyncName[] = "Enable Presearch Sync v2";
const char kPresearchSyncDescription[] =
    "Presearch Sync v2 integrates with chromium sync engine with Presearch specific "
    "authentication flow and enforce client side encryption";
const char kPresearchIpfsName[] = "Enable IPFS";
const char kPresearchIpfsDescription[] =
    "Enable native support of IPFS.";
const char kPresearchRewardsBitflyerName[] = "Enable bitFlyer for Presearch Rewards";
const char kPresearchRewardsBitflyerDescription[] =
    "Enables support for bitFlyer as an external wallet provider for Presearch "
    "Rewards users in Japan";
const char kNativePresearchWalletName[] = "Enable experimental Presearch native wallet";
const char kNativePresearchWalletDescription[] =
    "Experimental native cryptocurrency wallet support without the use of "
    "extensions";
const char kPresearchDecentralizedDnsName[] = "Enable decentralized DNS";
const char kPresearchDecentralizedDnsDescription[] =
    "Enable decentralized DNS support, such as Unstoppable Domains and "
    "Ethereum Name Service (ENS).";
const char kPresearchSuperReferralName[] = "Enable Presearch Super Referral";
const char kPresearchSuperReferralDescription[] =
    "Use custom theme for Presearch Super Referral";
const char kPresearchEphemeralStorageName[] = "Enable Ephemeral Storage";
const char kPresearchEphemeralStorageDescription[] =
    "Use ephemeral storage for third-party frames";
const char kPresearchEphemeralStorageKeepAliveName[] =
    "Ephemeral Storage Keep Alive";
const char kPresearchEphemeralStorageKeepAliveDescription[] =
    "Keep ephemeral storage partitions alive for a specified time after all "
    "tabs for that origin are closed";
const char kPresearchPermissionLifetimeName[] = "Permission Lifetime";
const char kPresearchPermissionLifetimeDescription[] =
    "Enables the option to choose a time period after which a permission will "
    "be automatically revoked";
}  // namespace flag_descriptions
