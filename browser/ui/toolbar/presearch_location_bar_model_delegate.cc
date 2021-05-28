/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/toolbar/presearch_location_bar_model_delegate.h"

#include "base/strings/utf_string_conversions.h"
#include "presearch/common/url_constants.h"
#include "presearch/common/webui_url_constants.h"
#include "presearch/components/presearch_wallet/common/buildflags/buildflags.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "presearch/components/ipfs/ipfs_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#endif

PresearchLocationBarModelDelegate::PresearchLocationBarModelDelegate(Browser* browser) :
    BrowserLocationBarModelDelegate(browser) {}

PresearchLocationBarModelDelegate::~PresearchLocationBarModelDelegate() {}

// static
void PresearchLocationBarModelDelegate::FormattedStringFromURL(const GURL& url,
    base::string16* new_formatted_url) {
  if (url.SchemeIs("chrome")) {
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url,
        0,
        base::UTF8ToUTF16("chrome://"),
        base::UTF8ToUTF16("presearch://"));
  }

#if BUILDFLAG(PRESEARCH_WALLET_ENABLED)
  if (url.SchemeIs(kChromeExtensionScheme) &&
      url.host() == ethereum_remote_client_extension_id) {
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url,
        0,
        base::UTF8ToUTF16(ethereum_remote_client_base_url),
        base::UTF8ToUTF16("presearch://wallet"));
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url,
        0,
        base::UTF8ToUTF16(ethereum_remote_client_phishing_url),
        base::UTF8ToUTF16("presearch://wallet"));
    base::ReplaceFirstSubstringAfterOffset(
        new_formatted_url,
        0,
        base::UTF8ToUTF16(ethereum_remote_client_ens_redirect_url),
        base::UTF8ToUTF16("presearch://wallet"));
  }
#endif
}

base::string16
PresearchLocationBarModelDelegate::FormattedStringWithEquivalentMeaning(
    const GURL& url,
    const base::string16& formatted_url) const {
  base::string16 new_formatted_url =
      BrowserLocationBarModelDelegate::FormattedStringWithEquivalentMeaning(
          url, formatted_url);
  PresearchLocationBarModelDelegate::FormattedStringFromURL(url,
      &new_formatted_url);
  return new_formatted_url;
}
