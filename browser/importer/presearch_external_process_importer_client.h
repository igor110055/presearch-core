/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#define PRESEARCH_BROWSER_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "presearch/common/importer/profile_import.mojom.h"
#include "chrome/browser/importer/external_process_importer_client.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"

class PresearchExternalProcessImporterClient
    : public ExternalProcessImporterClient,
      public presearch::mojom::ProfileImportObserver {
 public:
  PresearchExternalProcessImporterClient(
      base::WeakPtr<ExternalProcessImporterHost> importer_host,
      const importer::SourceProfile& source_profile,
      uint16_t items,
      InProcessImporterBridge* bridge);

  PresearchExternalProcessImporterClient(
      const PresearchExternalProcessImporterClient&) = delete;
  PresearchExternalProcessImporterClient& operator=(
      const PresearchExternalProcessImporterClient&) = delete;

  // ExternalProcessImportClient overrides:
  void Start() override;
  void Cancel() override;
  void CloseMojoHandles() override;
  void OnImportItemFinished(importer::ImportItem import_item) override;

  // presearch::mojom::ProfileImportObserver overrides:
  void OnCreditCardImportReady(
      const base::string16& name_on_card,
      const base::string16& expiration_month,
      const base::string16& expiration_year,
      const base::string16& decrypted_card_number,
      const std::string& origin) override;

 protected:
  ~PresearchExternalProcessImporterClient() override;

 private:
  // Used to start and stop the actual presearch importer running in a different
  // process.
  mojo::Remote<presearch::mojom::ProfileImport> presearch_profile_import_;

  // Used to receive progress updates from the presearch importer.
  mojo::Receiver<presearch::mojom::ProfileImportObserver> presearch_receiver_{this};
};

#endif  // PRESEARCH_BROWSER_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
