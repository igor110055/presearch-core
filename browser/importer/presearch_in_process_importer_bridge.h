/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_IMPORTER_PRESEARCH_IN_PROCESS_IMPORTER_BRIDGE_H_
#define PRESEARCH_BROWSER_IMPORTER_PRESEARCH_IN_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "presearch/common/importer/presearch_importer_bridge.h"
#include "chrome/browser/importer/in_process_importer_bridge.h"

class PresearchInProcessImporterBridge : public InProcessImporterBridge,
                                     public PresearchImporterBridge {
 public:
  using InProcessImporterBridge::InProcessImporterBridge;

  PresearchInProcessImporterBridge(const PresearchInProcessImporterBridge&) = delete;
  PresearchInProcessImporterBridge operator=(
      const PresearchInProcessImporterBridge&) = delete;

  // PresearchImporterBridge overrides:
  void SetCreditCard(const base::string16& name_on_card,
                     const base::string16& expiration_month,
                     const base::string16& expiration_year,
                     const base::string16& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~PresearchInProcessImporterBridge() override;
};

#endif  // PRESEARCH_BROWSER_IMPORTER_PRESEARCH_IN_PROCESS_IMPORTER_BRIDGE_H_
