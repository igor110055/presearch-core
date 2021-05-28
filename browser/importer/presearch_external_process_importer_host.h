/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_HOST_H_
#define PRESEARCH_BROWSER_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_HOST_H_

#include "base/memory/weak_ptr.h"
#include "base/optional.h"
#include "base/values.h"
#include "chrome/browser/importer/external_process_importer_host.h"
#include "extensions/buildflags/buildflags.h"

class PresearchExternalProcessImporterHost : public ExternalProcessImporterHost {
 public:
  PresearchExternalProcessImporterHost();

 private:
  friend class ExternalProcessImporterHost;

  ~PresearchExternalProcessImporterHost() override;

  // ExternalProcessImporterHost overrides:
  void NotifyImportEnded() override;

#if BUILDFLAG(ENABLE_EXTENSIONS)
  void LaunchExtensionsImport();
  void OnGetChromeExtensionsList(base::Optional<base::Value> extensions_list);
#endif

  // Vends weak pointers for the importer to call us back.
  base::WeakPtrFactory<PresearchExternalProcessImporterHost> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PresearchExternalProcessImporterHost);
};

#endif  // PRESEARCH_BROWSER_IMPORTER_PRESEARCH_EXTERNAL_PROCESS_IMPORTER_HOST_H_
