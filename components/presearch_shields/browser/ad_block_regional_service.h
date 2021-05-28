/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "presearch/components/adblock_rust_ffi/src/wrapper.h"
#include "presearch/components/presearch_shields/browser/ad_block_base_service.h"

class AdBlockServiceTest;

namespace presearch_shields {

// The presearch shields service in charge of ad-block checking and init
// for a specific region.
class AdBlockRegionalService : public AdBlockBaseService {
 public:
  explicit AdBlockRegionalService(
      const adblock::FilterList& catalog_entry,
      presearch_component_updater::PresearchComponent::Delegate* delegate);
  ~AdBlockRegionalService() override;

  void SetCatalogEntry(const adblock::FilterList& entry);

  std::string GetUUID() const { return uuid_; }
  std::string GetTitle() const { return title_; }

 protected:
  bool Init() override;
  void OnComponentReady(const std::string& component_id,
                        const base::FilePath& install_dir,
                        const std::string& manifest) override;
  void OnResourcesFileDataReady(const std::string& resources);

 private:
  friend class ::AdBlockServiceTest;
  static std::string g_ad_block_regional_component_id_;
  static std::string g_ad_block_regional_component_base64_public_key_;
  static std::string g_ad_block_regional_dat_file_version_;
  static void SetComponentIdAndBase64PublicKeyForTest(
      const std::string& component_id,
      const std::string& component_base64_public_key);

  std::string uuid_;
  std::string title_;
  std::string component_id_;
  std::string base64_public_key_;

  base::WeakPtrFactory<AdBlockRegionalService> weak_factory_{this};
  DISALLOW_COPY_AND_ASSIGN(AdBlockRegionalService);
};

// Creates the AdBlockRegionalService
std::unique_ptr<AdBlockRegionalService> AdBlockRegionalServiceFactory(
    const adblock::FilterList& catalog_entry,
    presearch_component_updater::PresearchComponent::Delegate* delegate);

}  // namespace presearch_shields

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_H_
