/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/common/network_constants.h"

const char kPresearchProxyPattern[] = "https://*.presearch.org/*";
const char kPresearchSoftwareProxyPattern[] = "https://*.presearchsoftware.com/*";

const char kPresearchUsageStandardPath[] = "/1/usage/presearch-core";
const char kPresearchUsageThresholdPath[] = "/1/usage/presearch-core-threshold";

const char kPresearchReferralsServer[] = "laptop-updates.presearch.org";
const char kPresearchReferralsHeadersPath[] = "/promo/custom-headers";
const char kPresearchReferralsInitPath[] = "/promo/initialize/nonua";
const char kPresearchReferralsActivityPath[] = "/promo/activity";

const char kPresearchSafeBrowsing2Proxy[] = "safebrowsing2.presearch.org";
const char kPresearchSafeBrowsingSslProxy[] = "sb-ssl.presearch.org";
const char kPresearchRedirectorProxy[] = "redirector.presearch.org";
const char kPresearchClients4Proxy[] = "clients4.presearch.org";
const char kPresearchStaticProxy[] = "static1.presearch.org";

const char kAutofillPrefix[] = "https://www.gstatic.com/autofill/*";
const char kClients4Prefix[] = "*://clients4.google.com/";
const char kCRXDownloadPrefix[] =
    "*://clients2.googleusercontent.com/crx/blobs/*crx*";
const char kEmptyDataURI[] = "data:text/plain,";
const char kEmptyImageDataURI[] =
    "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///"
    "yH5BAEAAAAALAAAAAABAAEAAAIBRAA7";
const char kJSDataURLPrefix[] = "data:application/javascript;base64,";
const char kGeoLocationsPattern[] =
    "https://www.googleapis.com/geolocation/v1/geolocate?key=*";
const char kSafeBrowsingPrefix[] = "https://safebrowsing.googleapis.com/";
const char kSafeBrowsingCrxListPrefix[] =
    "https://safebrowsing.google.com/safebrowsing/clientreport/crx-list-info";
const char kSafeBrowsingFileCheckPrefix[] =
    "https://sb-ssl.google.com/safebrowsing/clientreport/download";
const char kCRLSetPrefix1[] =
    "*://dl.google.com/release2/chrome_component/*crl-set*";
const char kCRLSetPrefix2[] =
    "*://*.gvt1.com/edgedl/release2/chrome_component/*";
const char kCRLSetPrefix3[] =
    "*://www.google.com/dl/release2/chrome_component/*crl-set*";
const char kCRLSetPrefix4[] =
    "*://storage.googleapis.com/update-delta/hfnkpimlhhgieaddgfemjhofmfblmnib"
    "/*crxd";
const char kChromeCastPrefix[] =
    "*://*.gvt1.com/edgedl/chromewebstore/*pkedcjkdefgpdelpbcmbmeomcjbeemfm*";

const char kWidevineGvt1Prefix[] =
    "*://*.gvt1.com/*oimompecagnajdejgnnjijobebaeigek*";
const char kWidevineGoogleDlPrefix[] =
    "*://dl.google.com/*oimompecagnajdejgnnjijobebaeigek*";

const char kUserAgentHeader[] = "User-Agent";
const char kPresearchPartnerHeader[] = "X-Presearch-Partner";
const char kPresearchServicesKeyHeader[] = "PresearchServiceKey";

const char kBittorrentMimeType[] = "application/x-bittorrent";
const char kOctetStreamMimeType[] = "application/octet-stream";

const char kSecGpcHeader[] = "Sec-GPC";
