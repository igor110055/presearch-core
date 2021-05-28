/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_UTILITY_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
#define PRESEARCH_UTILITY_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "presearch/common/importer/presearch_importer_bridge.h"
#include "presearch/common/importer/profile_import.mojom.h"
#include "chrome/utility/importer/external_process_importer_bridge.h"

class PresearchExternalProcessImporterBridge : public ExternalProcessImporterBridge,
                                           public PresearchImporterBridge {
 public:
  PresearchExternalProcessImporterBridge(
      const base::flat_map<uint32_t, std::string>& localized_strings,
      mojo::SharedRemote<chrome::mojom::ProfileImportObserver> observer,
      mojo::SharedRemote<presearch::mojom::ProfileImportObserver> presearch_observer);

  PresearchExternalProcessImporterBridge(
      const PresearchExternalProcessImporterBridge&) = delete;
  PresearchExternalProcessImporterBridge& operator=(
      const PresearchExternalProcessImporterBridge&) = delete;

  void SetCreditCard(const base::string16& name_on_card,
                     const base::string16& expiration_month,
                     const base::string16& expiration_year,
                     const base::string16& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~PresearchExternalProcessImporterBridge() override;

  mojo::SharedRemote<presearch::mojom::ProfileImportObserver> presearch_observer_;
};

#endif  // PRESEARCH_UTILITY_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
