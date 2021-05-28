/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/common/extensions/presearch_extensions_api_provider.h"

#include "presearch/common/extensions/api/generated_includes.h"
#include "extensions/common/features/json_feature_provider_source.h"
#include "extensions/common/permissions/permissions_info.h"

namespace extensions {

PresearchExtensionsAPIProvider::PresearchExtensionsAPIProvider() {}
PresearchExtensionsAPIProvider::~PresearchExtensionsAPIProvider() = default;

void PresearchExtensionsAPIProvider::AddAPIFeatures(FeatureProvider* provider) {
  AddPresearchAPIFeatures(provider);
}

void PresearchExtensionsAPIProvider::AddManifestFeatures(
    FeatureProvider* provider) {
  AddPresearchManifestFeatures(provider);
}

void PresearchExtensionsAPIProvider::AddPermissionFeatures(
    FeatureProvider* provider) {
  AddPresearchPermissionFeatures(provider);
}

void PresearchExtensionsAPIProvider::AddBehaviorFeatures(
    FeatureProvider* provider) {
  // No presearch-specific behavior features.
}

void PresearchExtensionsAPIProvider::AddAPIJSONSources(
    JSONFeatureProviderSource* json_source) {
  json_source->LoadJSON(IDR_PRESEARCH_EXTENSION_API_FEATURES);
}

bool PresearchExtensionsAPIProvider::IsAPISchemaGenerated(
    const std::string& name) {
  return api::PresearchGeneratedSchemas::IsGenerated(name);
}

base::StringPiece PresearchExtensionsAPIProvider::GetAPISchema(
    const std::string& name) {
  return api::PresearchGeneratedSchemas::Get(name);
}

void PresearchExtensionsAPIProvider::RegisterPermissions(
    PermissionsInfo* permissions_info) {
  // No presearch-specific permissions.
}

void PresearchExtensionsAPIProvider::RegisterManifestHandlers() {
  // No presearch-specific manifest handlers.
}

}  // namespace extensions
