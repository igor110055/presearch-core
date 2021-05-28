/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_UI_OMNIBOX_PRESEARCH_OMNIBOX_CLIENT_IMPL_H_
#define PRESEARCH_BROWSER_UI_OMNIBOX_PRESEARCH_OMNIBOX_CLIENT_IMPL_H_

#include "presearch/browser/autocomplete/presearch_autocomplete_scheme_classifier.h"
#include "chrome/browser/ui/omnibox/chrome_omnibox_client.h"

class OmniboxEditController;
class PrefRegistrySimple;
class Profile;

class PresearchOmniboxClientImpl : public ChromeOmniboxClient {
 public:
  PresearchOmniboxClientImpl(OmniboxEditController* controller, Profile* profile);
  ~PresearchOmniboxClientImpl() override;

  static void RegisterPrefs(PrefRegistrySimple* prefs);

  const AutocompleteSchemeClassifier& GetSchemeClassifier() const override;
  bool IsAutocompleteEnabled() const override;

  void OnInputAccepted(const AutocompleteMatch& match) override;

 private:
  Profile* profile_;
  PresearchAutocompleteSchemeClassifier scheme_classifier_;

  DISALLOW_COPY_AND_ASSIGN(PresearchOmniboxClientImpl);
};

#endif  // PRESEARCH_BROWSER_UI_OMNIBOX_PRESEARCH_OMNIBOX_CLIENT_IMPL_H_
