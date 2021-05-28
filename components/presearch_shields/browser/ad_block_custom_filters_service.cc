/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_shields/browser/ad_block_custom_filters_service.h"

#include "base/logging.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/adblock_rust_ffi/src/wrapper.h"
#include "presearch/components/presearch_shields/browser/ad_block_service.h"
#include "chrome/browser/browser_process.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_thread.h"

using presearch_component_updater::PresearchComponent;

namespace presearch_shields {

AdBlockCustomFiltersService::AdBlockCustomFiltersService(
    PresearchComponent::Delegate* delegate) : AdBlockBaseService(delegate) {
}

AdBlockCustomFiltersService::~AdBlockCustomFiltersService() {
}

bool AdBlockCustomFiltersService::Init() {
  return UpdateCustomFilters(GetCustomFilters());
}

std::string AdBlockCustomFiltersService::GetCustomFilters() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  PrefService* local_state = g_browser_process->local_state();
  if (!local_state)
    return std::string();
  return local_state->GetString(kAdBlockCustomFilters);
}

bool AdBlockCustomFiltersService::UpdateCustomFilters(
    const std::string& custom_filters) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  PrefService* local_state = g_browser_process->local_state();
  if (!local_state)
    return false;
  local_state->SetString(kAdBlockCustomFilters, custom_filters);

  GetTaskRunner()->PostTask(
      FROM_HERE,
      base::BindOnce(
          &AdBlockCustomFiltersService::UpdateCustomFiltersOnFileTaskRunner,
          base::Unretained(this), custom_filters));

  return true;
}

bool AdBlockCustomFiltersService::MigrateLegacyCosmeticFilters(
    const std::map<std::string, std::vector<std::string>> legacyFilters) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  PrefService* local_state = g_browser_process->local_state();
  if (!local_state)
    return false;
  std::string filters_update = local_state->GetString(kAdBlockCustomFilters);

  filters_update +=
      "\n\n! Filters migrated from "
      "'Right click > Presearch > Block element via selector'";
  for (const auto& hostEntry : legacyFilters) {
    const std::string& host = hostEntry.first;
    const std::vector<std::string>& hostSelectors = hostEntry.second;

    for (const auto& selector : hostSelectors) {
      if (selector.empty()) {
        continue;
      }

      const std::string rule = '\n' + host + "##" + selector;

      filters_update += rule;
    }
  }

  return UpdateCustomFilters(filters_update);
}

void AdBlockCustomFiltersService::UpdateCustomFiltersOnFileTaskRunner(
    const std::string& custom_filters) {
  DCHECK(GetTaskRunner()->RunsTasksInCurrentSequence());
  ad_block_client_.reset(new adblock::Engine(custom_filters.c_str()));
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<AdBlockCustomFiltersService>
AdBlockCustomFiltersServiceFactory(PresearchComponent::Delegate* delegate) {
  return std::make_unique<AdBlockCustomFiltersService>(delegate);
}

}  // namespace presearch_shields
