/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/webui/presearch_adblock_ui.h"

#include <memory>

#include "presearch/browser/presearch_browser_process.h"
#include "presearch/browser/ui/webui/presearch_webui_source.h"
#include "presearch/common/webui_url_constants.h"
#include "presearch/components/presearch_adblock/resources/grit/presearch_adblock_generated_map.h"
#include "presearch/components/presearch_shields/browser/ad_block_custom_filters_service.h"
#include "presearch/components/presearch_shields/browser/ad_block_regional_service_manager.h"
#include "components/grit/presearch_components_resources.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace {

class AdblockDOMHandler : public content::WebUIMessageHandler {
 public:
  AdblockDOMHandler();
  ~AdblockDOMHandler() override;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

 private:
  void HandleEnableFilterList(const base::ListValue* args);
  void HandleGetCustomFilters(const base::ListValue* args);
  void HandleGetRegionalLists(const base::ListValue* args);
  void HandleUpdateCustomFilters(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(AdblockDOMHandler);
};

AdblockDOMHandler::AdblockDOMHandler() {}

AdblockDOMHandler::~AdblockDOMHandler() {}

void AdblockDOMHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "presearch_adblock.enableFilterList",
      base::BindRepeating(&AdblockDOMHandler::HandleEnableFilterList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "presearch_adblock.getCustomFilters",
      base::BindRepeating(&AdblockDOMHandler::HandleGetCustomFilters,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "presearch_adblock.getRegionalLists",
      base::BindRepeating(&AdblockDOMHandler::HandleGetRegionalLists,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "presearch_adblock.updateCustomFilters",
      base::BindRepeating(&AdblockDOMHandler::HandleUpdateCustomFilters,
                          base::Unretained(this)));
}

void AdblockDOMHandler::HandleEnableFilterList(const base::ListValue* args) {
  DCHECK_EQ(args->GetSize(), 2U);
  std::string uuid;
  if (!args->GetString(0, &uuid))
    return;
  bool enabled;
  if (!args->GetBoolean(1, &enabled))
    return;
  g_presearch_browser_process->ad_block_regional_service_manager()
      ->EnableFilterList(uuid, enabled);
}

void AdblockDOMHandler::HandleGetCustomFilters(const base::ListValue* args) {
  DCHECK_EQ(args->GetSize(), 0U);
  const std::string custom_filters =
      g_presearch_browser_process->ad_block_custom_filters_service()
          ->GetCustomFilters();
  if (!web_ui()->CanCallJavascript())
    return;
  web_ui()->CallJavascriptFunctionUnsafe("presearch_adblock.onGetCustomFilters",
                                         base::Value(custom_filters));
}

void AdblockDOMHandler::HandleGetRegionalLists(const base::ListValue* args) {
  DCHECK_EQ(args->GetSize(), 0U);
  if (!web_ui()->CanCallJavascript())
    return;
  std::unique_ptr<base::ListValue> regional_lists =
      g_presearch_browser_process->ad_block_regional_service_manager()
          ->GetRegionalLists();
  web_ui()->CallJavascriptFunctionUnsafe("presearch_adblock.onGetRegionalLists",
                                         *regional_lists);
}

void AdblockDOMHandler::HandleUpdateCustomFilters(const base::ListValue* args) {
  DCHECK_EQ(args->GetSize(), 1U);
  std::string custom_filters;
  if (!args->GetString(0, &custom_filters))
    return;

  g_presearch_browser_process->ad_block_custom_filters_service()
      ->UpdateCustomFilters(custom_filters);
}

}  // namespace

PresearchAdblockUI::PresearchAdblockUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, name, kPresearchAdblockGenerated,
                              kPresearchAdblockGeneratedSize,
                              IDR_PRESEARCH_ADBLOCK_HTML);
  web_ui->AddMessageHandler(std::make_unique<AdblockDOMHandler>());
}

PresearchAdblockUI::~PresearchAdblockUI() = default;
