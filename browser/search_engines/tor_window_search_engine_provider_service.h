/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_
#define PRESEARCH_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_

#include <memory>

#include "presearch/browser/search_engines/search_engine_provider_service.h"

class PrefService;
struct TemplateURLData;

// The purpose of this service for tor is making user changed search engine
// provider persist across the sessions.
// Also, PresearchProfileManager::SetNonPersonalProfilePrefs() overrides for it.
class TorWindowSearchEngineProviderService
    : public SearchEngineProviderService {
 public:
  explicit TorWindowSearchEngineProviderService(Profile* otr_profile);
  ~TorWindowSearchEngineProviderService() override;

 private:
  std::unique_ptr<TemplateURLData> GetInitialSearchEngineProvider(
      PrefService* prefs) const;

  DISALLOW_COPY_AND_ASSIGN(TorWindowSearchEngineProviderService);
};

#endif  // PRESEARCH_BROWSER_SEARCH_ENGINES_TOR_WINDOW_SEARCH_ENGINE_PROVIDER_SERVICE_H_
