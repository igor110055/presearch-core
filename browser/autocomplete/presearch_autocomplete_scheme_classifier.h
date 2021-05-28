/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_AUTOCOMPLETE_PRESEARCH_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_
#define PRESEARCH_BROWSER_AUTOCOMPLETE_PRESEARCH_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

#include <string>

#include "presearch/components/presearch_webtorrent/browser/buildflags/buildflags.h"
#include "chrome/browser/autocomplete/chrome_autocomplete_scheme_classifier.h"

class PresearchAutocompleteSchemeClassifier
    : public ChromeAutocompleteSchemeClassifier {
 public:
  explicit PresearchAutocompleteSchemeClassifier(Profile* profile);
  ~PresearchAutocompleteSchemeClassifier() override;

  metrics::OmniboxInputType GetInputTypeForScheme(
      const std::string& scheme) const override;

 private:
#if BUILDFLAG(ENABLE_PRESEARCH_WEBTORRENT)
  Profile* profile_ = nullptr;
#endif

  DISALLOW_COPY_AND_ASSIGN(PresearchAutocompleteSchemeClassifier);
};

#endif  // PRESEARCH_BROWSER_AUTOCOMPLETE_PRESEARCH_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

