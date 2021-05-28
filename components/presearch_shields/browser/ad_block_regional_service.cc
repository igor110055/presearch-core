/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_shields/browser/ad_block_regional_service.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/base_paths.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "base/threading/thread_restrictions.h"
#include "presearch/browser/presearch_browser_process.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/adblock_rust_ffi/src/wrapper.h"
#include "presearch/components/presearch_shields/browser/ad_block_regional_service_manager.h"
#include "presearch/components/presearch_shields/browser/ad_block_service.h"
#include "presearch/components/presearch_shields/browser/ad_block_service_helper.h"
#include "components/prefs/pref_service.h"

namespace presearch_shields {

std::string AdBlockRegionalService::g_ad_block_regional_component_id_;  // NOLINT
std::string
    AdBlockRegionalService::g_ad_block_regional_component_base64_public_key_;  // NOLINT

AdBlockRegionalService::AdBlockRegionalService(
    const adblock::FilterList& catalog_entry,
    presearch_component_updater::PresearchComponent::Delegate* delegate)
    : AdBlockBaseService(delegate),
      uuid_(catalog_entry.uuid),
      title_(catalog_entry.title),
      component_id_(catalog_entry.component_id),
      base64_public_key_(catalog_entry.base64_public_key) {
}

AdBlockRegionalService::~AdBlockRegionalService() {
}

void AdBlockRegionalService::SetCatalogEntry(const adblock::FilterList& entry) {
  DCHECK(entry.uuid == uuid_);
  title_ = entry.title;
  component_id_ = entry.component_id;
  base64_public_key_ = entry.base64_public_key;
}

bool AdBlockRegionalService::Init() {
  AdBlockBaseService::Init();

  Register(title_,
           !g_ad_block_regional_component_id_.empty()
               ? g_ad_block_regional_component_id_
               : component_id_,
           !g_ad_block_regional_component_base64_public_key_.empty()
               ? g_ad_block_regional_component_base64_public_key_
               : base64_public_key_);

  return true;
}

void AdBlockRegionalService::OnComponentReady(
    const std::string& component_id,
    const base::FilePath& install_dir,
    const std::string& manifest) {
  base::FilePath dat_file_path =
      install_dir.AppendASCII(std::string("rs-") + uuid_)
          .AddExtension(FILE_PATH_LITERAL(".dat"));
  GetDATFileData(dat_file_path);
  base::FilePath resources_file_path =
      install_dir.AppendASCII(kAdBlockResourcesFilename);

  base::PostTaskAndReplyWithResult(
      GetTaskRunner().get(), FROM_HERE,
      base::BindOnce(&presearch_component_updater::GetDATFileAsString,
                     resources_file_path),
      base::BindOnce(&AdBlockRegionalService::OnResourcesFileDataReady,
                     weak_factory_.GetWeakPtr()));
}

void AdBlockRegionalService::OnResourcesFileDataReady(
    const std::string& resources) {
  g_presearch_browser_process->ad_block_regional_service_manager()->AddResources(
      resources);
}

// static
void AdBlockRegionalService::SetComponentIdAndBase64PublicKeyForTest(
    const std::string& component_id,
    const std::string& component_base64_public_key) {
  g_ad_block_regional_component_id_ = component_id;
  g_ad_block_regional_component_base64_public_key_ =
      component_base64_public_key;
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<AdBlockRegionalService> AdBlockRegionalServiceFactory(
    const adblock::FilterList& catalog_entry,
    presearch_component_updater::PresearchComponent::Delegate* delegate) {
  return std::make_unique<AdBlockRegionalService>(catalog_entry, delegate);
}

}  // namespace presearch_shields
