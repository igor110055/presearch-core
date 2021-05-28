// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "presearch/browser/ui/webui/new_tab_page/presearch_new_tab_ui.h"

#include <string>

#include "presearch/browser/new_tab/new_tab_shows_options.h"
#include "presearch/browser/ui/webui/presearch_webui_source.h"
#include "presearch/browser/ui/webui/new_tab_page/presearch_new_tab_message_handler.h"
#include "presearch/browser/ui/webui/new_tab_page/instant_service_message_handler.h"
#include "presearch/components/presearch_new_tab/resources/grit/presearch_new_tab_generated_map.h"
#include "chrome/browser/profiles/profile.h"
#include "components/grit/presearch_components_resources.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/base/l10n/l10n_util.h"

PresearchNewTabUI::PresearchNewTabUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  Profile* profile = Profile::FromWebUI(web_ui);

  if (presearch::ShouldNewTabShowBlankpage(profile)) {
    content::WebUIDataSource* source =
        content::WebUIDataSource::Create(name);
    source->SetDefaultResource(IDR_PRESEARCH_BLANK_NEW_TAB_HTML);
    content::WebUIDataSource::Add(profile, source);
  } else {
    content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
        web_ui, name, kPresearchNewTabGenerated, kPresearchNewTabGeneratedSize,
        IDR_PRESEARCH_NEW_TAB_HTML);
    web_ui->AddMessageHandler(
        base::WrapUnique(PresearchNewTabMessageHandler::Create(source, profile)));
    web_ui->AddMessageHandler(
        base::WrapUnique(new InstantServiceMessageHandler(profile)));
  }

  web_ui->OverrideTitle(l10n_util::GetStringUTF16(IDS_NEW_TAB_TITLE));
}

PresearchNewTabUI::~PresearchNewTabUI() {
}
