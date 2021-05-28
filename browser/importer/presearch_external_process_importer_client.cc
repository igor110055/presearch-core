/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/importer/presearch_external_process_importer_client.h"

#include <utility>

#include "base/bind.h"
#include "presearch/browser/importer/presearch_in_process_importer_bridge.h"
#include "chrome/browser/service_sandbox_type.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/service_process_host.h"

namespace {
bool ShouldUsePresearchImporter(importer::ImporterType type) {
  if (type == importer::TYPE_CHROME)
    return true;

  return false;
}
}  // namespace

PresearchExternalProcessImporterClient::PresearchExternalProcessImporterClient(
    base::WeakPtr<ExternalProcessImporterHost> importer_host,
    const importer::SourceProfile& source_profile,
    uint16_t items,
    InProcessImporterBridge* bridge)
    : ExternalProcessImporterClient(
          importer_host, source_profile, items, bridge) {}

PresearchExternalProcessImporterClient::
    ~PresearchExternalProcessImporterClient() = default;

void PresearchExternalProcessImporterClient::Start() {
  if (!ShouldUsePresearchImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Start();
    return;
  }

  AddRef();  // balanced in Cleanup.

  auto options = content::ServiceProcessHost::Options()
                     .WithDisplayName(IDS_UTILITY_PROCESS_PROFILE_IMPORTER_NAME)
                     .Pass();
  options.sandbox_type =
      content::GetServiceSandboxType<presearch::mojom::ProfileImport>();
  content::ServiceProcessHost::Launch(
      presearch_profile_import_.BindNewPipeAndPassReceiver(), std::move(options));

  presearch_profile_import_.set_disconnect_handler(
      base::BindOnce(&ExternalProcessImporterClient::OnProcessCrashed, this));

  base::flat_map<uint32_t, std::string> localized_strings;
  presearch_profile_import_->StartImport(
      source_profile_, items_, localized_strings,
      receiver_.BindNewPipeAndPassRemote(),
      presearch_receiver_.BindNewPipeAndPassRemote());
}

void PresearchExternalProcessImporterClient::Cancel() {
  if (!ShouldUsePresearchImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Cancel();
    return;
  }

  if (cancelled_)
    return;

  cancelled_ = true;
  presearch_profile_import_->CancelImport();
  CloseMojoHandles();
  Release();
}

void PresearchExternalProcessImporterClient::CloseMojoHandles() {
  if (!ShouldUsePresearchImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::CloseMojoHandles();
    return;
  }

  presearch_profile_import_.reset();
  presearch_receiver_.reset();
  receiver_.reset();
}

void PresearchExternalProcessImporterClient::OnImportItemFinished(
    importer::ImportItem import_item) {
  if (!ShouldUsePresearchImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::OnImportItemFinished(import_item);
    return;
  }

  if (cancelled_)
    return;

  bridge_->NotifyItemEnded(import_item);
  presearch_profile_import_->ReportImportItemFinished(import_item);
}

void PresearchExternalProcessImporterClient::OnCreditCardImportReady(
    const base::string16& name_on_card,
    const base::string16& expiration_month,
    const base::string16& expiration_year,
    const base::string16& decrypted_card_number,
    const std::string& origin) {
  if (cancelled_)
    return;

  static_cast<PresearchInProcessImporterBridge*>(
      bridge_.get())->SetCreditCard(name_on_card,
                                    expiration_month,
                                    expiration_year,
                                    decrypted_card_number,
                                    origin);
}
