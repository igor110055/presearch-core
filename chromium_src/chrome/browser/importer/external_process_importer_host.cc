/* Copyright 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/importer/presearch_external_process_importer_client.h"
#include "presearch/browser/importer/presearch_in_process_importer_bridge.h"

#define ExternalProcessImporterClient PresearchExternalProcessImporterClient
#define InProcessImporterBridge PresearchInProcessImporterBridge
#include "../../../../../chrome/browser/importer/external_process_importer_host.cc"
#undef ExternalProcessImporterClient
#undef InProcessImporterBridge
