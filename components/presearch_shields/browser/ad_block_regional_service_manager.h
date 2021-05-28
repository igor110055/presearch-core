/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_MANAGER_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/macros.h"
#include "base/memory/scoped_refptr.h"
#include "base/optional.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "presearch/components/adblock_rust_ffi/src/wrapper.h"
#include "presearch/components/presearch_component_updater/browser/presearch_component.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

namespace base {
class ListValue;
}  // namespace base

class AdBlockServiceTest;

using presearch_component_updater::PresearchComponent;

namespace presearch_shields {

class AdBlockRegionalService;

// The AdBlock regional service manager, in charge of initializing and
// managing regional AdBlock clients.
class AdBlockRegionalServiceManager {
 public:
  explicit AdBlockRegionalServiceManager(PresearchComponent::Delegate* delegate);
  ~AdBlockRegionalServiceManager();

  std::unique_ptr<base::ListValue> GetRegionalLists();

  void SetRegionalCatalog(std::vector<adblock::FilterList> catalog);
  const std::vector<adblock::FilterList>& GetRegionalCatalog();

  bool IsInitialized() const;
  bool Start();
  void ShouldStartRequest(const GURL& url,
                          blink::mojom::ResourceType resource_type,
                          const std::string& tab_host,
                          bool* did_match_rule,
                          bool* did_match_exception,
                          bool* did_match_important,
                          std::string* mock_data_url);
  base::Optional<std::string> GetCspDirectives(
      const GURL& url,
      blink::mojom::ResourceType resource_type,
      const std::string& tab_host);
  void EnableTag(const std::string& tag, bool enabled);
  void AddResources(const std::string& resources);
  void EnableFilterList(const std::string& uuid, bool enabled);

  base::Optional<base::Value> UrlCosmeticResources(
          const std::string& url);
  base::Optional<base::Value> HiddenClassIdSelectors(
          const std::vector<std::string>& classes,
          const std::vector<std::string>& ids,
          const std::vector<std::string>& exceptions);

 private:
  friend class ::AdBlockServiceTest;
  void StartRegionalServices();
  void UpdateFilterListPrefs(const std::string& uuid, bool enabled);

  presearch_component_updater::PresearchComponent::Delegate* delegate_;  // NOT OWNED
  bool initialized_;
  base::Lock regional_services_lock_;
  std::map<std::string, std::unique_ptr<AdBlockRegionalService>>
      regional_services_;

  std::vector<adblock::FilterList> regional_catalog_;

  DISALLOW_COPY_AND_ASSIGN(AdBlockRegionalServiceManager);
};

// Creates the AdBlockRegionalServiceManager
std::unique_ptr<AdBlockRegionalServiceManager>
AdBlockRegionalServiceManagerFactory(PresearchComponent::Delegate* delegate);

}  // namespace presearch_shields

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_MANAGER_H_
