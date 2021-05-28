/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_CUSTOM_FILTERS_SERVICE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_CUSTOM_FILTERS_SERVICE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "presearch/components/presearch_shields/browser/ad_block_base_service.h"

class AdBlockServiceTest;

using presearch_component_updater::PresearchComponent;

namespace presearch_shields {

// The presearch shields service in charge of custom filter ad-block
// checking and init.
class AdBlockCustomFiltersService : public AdBlockBaseService {
 public:
  explicit AdBlockCustomFiltersService(PresearchComponent::Delegate* delegate);
  ~AdBlockCustomFiltersService() override;

  std::string GetCustomFilters();
  bool UpdateCustomFilters(const std::string& custom_filters);
  bool MigrateLegacyCosmeticFilters(
      const std::map<std::string, std::vector<std::string>> legacyFilters);

 protected:
  bool Init() override;

 private:
  friend class ::AdBlockServiceTest;
  void UpdateCustomFiltersOnFileTaskRunner(const std::string& custom_filters);

  DISALLOW_COPY_AND_ASSIGN(AdBlockCustomFiltersService);
};

// Creates the AdBlockCustomFiltersService
std::unique_ptr<AdBlockCustomFiltersService>
AdBlockCustomFiltersServiceFactory(PresearchComponent::Delegate* delegate);

}  // namespace presearch_shields

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_CUSTOM_FILTERS_SERVICE_H_
