/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/importer/presearch_in_process_importer_bridge.h"

#include "chrome/browser/importer/profile_writer.h"

PresearchInProcessImporterBridge::~PresearchInProcessImporterBridge() = default;

void PresearchInProcessImporterBridge::SetCreditCard(
    const base::string16& name_on_card,
    const base::string16& expiration_month,
    const base::string16& expiration_year,
    const base::string16& decrypted_card_number,
    const std::string& origin) {
  writer_->AddCreditCard(name_on_card,
                         expiration_month,
                         expiration_year,
                         decrypted_card_number,
                         origin);
}
