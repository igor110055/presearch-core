/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/utility/importer/presearch_external_process_importer_bridge.h"

#include <utility>

PresearchExternalProcessImporterBridge::PresearchExternalProcessImporterBridge(
    const base::flat_map<uint32_t, std::string>& localized_strings,
    mojo::SharedRemote<chrome::mojom::ProfileImportObserver> observer,
    mojo::SharedRemote<presearch::mojom::ProfileImportObserver> presearch_observer)
    : ExternalProcessImporterBridge(std::move(localized_strings),
                                         std::move(observer)),
      presearch_observer_(std::move(presearch_observer)) {}

PresearchExternalProcessImporterBridge::
    ~PresearchExternalProcessImporterBridge() = default;

void PresearchExternalProcessImporterBridge::SetCreditCard(
    const base::string16& name_on_card,
    const base::string16& expiration_month,
    const base::string16& expiration_year,
    const base::string16& decrypted_card_number,
    const std::string& origin) {
  presearch_observer_->OnCreditCardImportReady(
      name_on_card, expiration_month,
      expiration_year, decrypted_card_number,
      origin);
}
