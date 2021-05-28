/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/autocomplete/presearch_autocomplete_scheme_classifier.h"

#include <string>

#include "base/strings/string_util.h"
#include "presearch/common/url_constants.h"
#include "presearch/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"

#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
#include "presearch/components/presearch_webtorrent/browser/webtorrent_util.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "presearch/components/ipfs/ipfs_constants.h"
#endif

PresearchAutocompleteSchemeClassifier::PresearchAutocompleteSchemeClassifier(
    Profile* profile)
    : ChromeAutocompleteSchemeClassifier(profile) {
#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
  profile_ = profile;
#endif
}

PresearchAutocompleteSchemeClassifier::~PresearchAutocompleteSchemeClassifier() {
}

// Without this override, typing in presearch:// URLs will search Google
metrics::OmniboxInputType
PresearchAutocompleteSchemeClassifier::GetInputTypeForScheme(
    const std::string& scheme) const {
  if (scheme.empty()) {
    return metrics::OmniboxInputType::EMPTY;
  }
  if (base::IsStringASCII(scheme) &&
      base::LowerCaseEqualsASCII(scheme, kPresearchUIScheme)) {
    return metrics::OmniboxInputType::URL;
  }

#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
  if (base::IsStringASCII(scheme) &&
      webtorrent::IsWebtorrentPrefEnabled(profile_) &&
      base::LowerCaseEqualsASCII(scheme, kMagnetScheme)) {
    return metrics::OmniboxInputType::URL;
  }
#endif

#if BUILDFLAG(IPFS_ENABLED)
  if (base::IsStringASCII(scheme) &&
      (base::LowerCaseEqualsASCII(scheme, ipfs::kIPFSScheme) ||
       base::LowerCaseEqualsASCII(scheme, ipfs::kIPNSScheme))) {
    return metrics::OmniboxInputType::URL;
  }
#endif


  return ChromeAutocompleteSchemeClassifier::GetInputTypeForScheme(scheme);
}
