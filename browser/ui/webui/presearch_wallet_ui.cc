/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/presearch_wallet_ui.h"

#include <memory>

#include "presearch/browser/ui/webui/presearch_webui_source.h"
#include "presearch/components/presearch_wallet/resources/grit/presearch_wallet_generated_map.h"
#include "components/grit/presearch_components_resources.h"

PresearchWalletUI::PresearchWalletUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, name, kPresearchWalletGenerated,
                              kPresearchWalletGeneratedSize, IDR_PRESEARCH_WALLET_HTML,
                              true /*disable_trusted_types_csp*/);
}

PresearchWalletUI::~PresearchWalletUI() = default;
