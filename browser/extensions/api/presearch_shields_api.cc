/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/presearch_shields_api.h"

#include <utility>

#include "base/feature_list.h"
#include "base/strings/string_number_conversions.h"
#include "presearch/browser/presearch_browser_process.h"
#include "presearch/browser/extensions/api/presearch_action_api.h"
#include "presearch/browser/ui/presearch_pages.h"
#include "presearch/browser/webcompat_reporter/webcompat_reporter_dialog.h"
#include "presearch/common/extensions/api/presearch_shields.h"
#include "presearch/components/presearch_shields/browser/ad_block_custom_filters_service.h"
#include "presearch/components/presearch_shields/browser/ad_block_service.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_p3a.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_util.h"
#include "presearch/components/presearch_shields/browser/presearch_shields_web_contents_observer.h"
#include "presearch/components/presearch_shields/common/presearch_shield_constants.h"
#include "presearch/components/presearch_shields/common/features.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/chrome_extension_function_details.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_util.h"
#include "extensions/common/constants.h"

using presearch_shields::PresearchShieldsWebContentsObserver;
using presearch_shields::ControlType;
using presearch_shields::ControlTypeFromString;
using presearch_shields::ControlTypeToString;

namespace extensions {
namespace api {

namespace {

const char kInvalidUrlError[] = "Invalid URL.";
const char kInvalidControlTypeError[] = "Invalid ControlType.";

}  // namespace

ExtensionFunction::ResponseAction
PresearchShieldsUrlCosmeticResourcesFunction::Run() {
  std::unique_ptr<presearch_shields::UrlCosmeticResources::Params> params(
      presearch_shields::UrlCosmeticResources::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  g_presearch_browser_process->ad_block_service()
      ->GetTaskRunner()
      ->PostTaskAndReplyWithResult(
          FROM_HERE,
          base::BindOnce(&PresearchShieldsUrlCosmeticResourcesFunction::
                             GetUrlCosmeticResourcesOnTaskRunner,
                         this, params->url),
          base::BindOnce(&PresearchShieldsUrlCosmeticResourcesFunction::
                             GetUrlCosmeticResourcesOnUI,
                         this));
  return RespondLater();
}

std::unique_ptr<base::ListValue>
PresearchShieldsUrlCosmeticResourcesFunction::GetUrlCosmeticResourcesOnTaskRunner(
    const std::string& url) {
  base::Optional<base::Value> resources =
      g_presearch_browser_process->ad_block_service()->UrlCosmeticResources(url);

  if (!resources || !resources->is_dict()) {
    return std::unique_ptr<base::ListValue>();
  }

  auto result_list = std::make_unique<base::ListValue>();
  result_list->Append(std::move(*resources));
  return result_list;
}

void PresearchShieldsUrlCosmeticResourcesFunction::GetUrlCosmeticResourcesOnUI(
    std::unique_ptr<base::ListValue> resources) {
  if (!resources) {
    Respond(Error("Url-specific cosmetic resources could not be returned"));
    return;
  }
  Respond(ArgumentList(std::move(resources)));
}

ExtensionFunction::ResponseAction
PresearchShieldsHiddenClassIdSelectorsFunction::Run() {
  std::unique_ptr<presearch_shields::HiddenClassIdSelectors::Params> params(
      presearch_shields::HiddenClassIdSelectors::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  g_presearch_browser_process->ad_block_service()
      ->GetTaskRunner()
      ->PostTaskAndReplyWithResult(
          FROM_HERE,
          base::BindOnce(&PresearchShieldsHiddenClassIdSelectorsFunction::
                             GetHiddenClassIdSelectorsOnTaskRunner,
                         this, params->classes, params->ids,
                         params->exceptions),
          base::BindOnce(&PresearchShieldsHiddenClassIdSelectorsFunction::
                             GetHiddenClassIdSelectorsOnUI,
                         this));
  return RespondLater();
}

std::unique_ptr<base::ListValue> PresearchShieldsHiddenClassIdSelectorsFunction::
    GetHiddenClassIdSelectorsOnTaskRunner(
        const std::vector<std::string>& classes,
        const std::vector<std::string>& ids,
        const std::vector<std::string>& exceptions) {
  base::Optional<base::Value> hide_selectors =
      g_presearch_browser_process->ad_block_service()->HiddenClassIdSelectors(
          classes, ids, exceptions);

  if (!hide_selectors || !hide_selectors->is_list())
    return std::make_unique<base::ListValue>();

  auto result_list =
      std::make_unique<base::ListValue>(hide_selectors->GetList());

  return result_list;
}

void PresearchShieldsHiddenClassIdSelectorsFunction::GetHiddenClassIdSelectorsOnUI(
    std::unique_ptr<base::ListValue> selectors) {
  Respond(ArgumentList(std::move(selectors)));
}

ExtensionFunction::ResponseAction
PresearchShieldsMigrateLegacyCosmeticFiltersFunction::Run() {
  std::unique_ptr<presearch_shields::MigrateLegacyCosmeticFilters::Params> params(
      presearch_shields::MigrateLegacyCosmeticFilters::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const bool success =
      g_presearch_browser_process->ad_block_custom_filters_service()
          ->MigrateLegacyCosmeticFilters(
              params->legacy_filters.additional_properties);

  auto callback_args = std::make_unique<base::ListValue>();
  callback_args->Append(base::Value(success));
  return RespondNow(ArgumentList(std::move(callback_args)));
}

ExtensionFunction::ResponseAction
PresearchShieldsAddSiteCosmeticFilterFunction::Run() {
  std::unique_ptr<presearch_shields::AddSiteCosmeticFilter::Params> params(
      presearch_shields::AddSiteCosmeticFilter::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  auto* custom_filters_service =
      g_presearch_browser_process->ad_block_custom_filters_service();
  std::string custom_filters = custom_filters_service->GetCustomFilters();
  custom_filters_service->UpdateCustomFilters(custom_filters + '\n' +
                                              params->host + "##" +
                                              params->css_selector + '\n');

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsOpenFilterManagementPageFunction::Run() {
  Browser* browser = chrome::FindLastActive();
  if (browser) {
    presearch::ShowPresearchAdblock(browser);
  }

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction PresearchShieldsAllowScriptsOnceFunction::Run() {
  std::unique_ptr<presearch_shields::AllowScriptsOnce::Params> params(
      presearch_shields::AllowScriptsOnce::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
          params->tab_id, Profile::FromBrowserContext(browser_context()),
          include_incognito_information(), nullptr, nullptr, &contents,
          nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  PresearchShieldsWebContentsObserver::FromWebContents(contents)->AllowScriptsOnce(
      params->origins, contents);
  return RespondNow(NoArguments());
}

PresearchShieldsOpenBrowserActionUIFunction::
~PresearchShieldsOpenBrowserActionUIFunction() {
}

ExtensionFunction::ResponseAction
PresearchShieldsOpenBrowserActionUIFunction::Run() {
  std::unique_ptr<presearch_shields::OpenBrowserActionUI::Params> params(
      presearch_shields::OpenBrowserActionUI::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  std::string error;
  if (!PresearchActionAPI::ShowActionUI(this,
      presearch_extension_id,
      std::move(params->window_id),
      std::move(params->relative_path), &error)) {
    return RespondNow(Error(error));
  }
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsSetPresearchShieldsEnabledFunction::Run() {
  std::unique_ptr<presearch_shields::SetPresearchShieldsEnabled::Params> params(
      presearch_shields::SetPresearchShieldsEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetPresearchShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      params->enabled,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsGetPresearchShieldsEnabledFunction::Run() {
  std::unique_ptr<presearch_shields::GetPresearchShieldsEnabled::Params> params(
      presearch_shields::GetPresearchShieldsEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto enabled = ::presearch_shields::GetPresearchShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(enabled)));
}

ExtensionFunction::ResponseAction
PresearchShieldsShouldDoCosmeticFilteringFunction::Run() {
#if !defined(OS_ANDROID) && !defined(CHROME_OS)
  if (base::FeatureList::IsEnabled(
          ::presearch_shields::features::kPresearchAdblockCosmeticFilteringNative))
    return RespondNow(OneArgument(base::Value(false)));
#endif
  std::unique_ptr<presearch_shields::ShouldDoCosmeticFiltering::Params>
    params(
      presearch_shields::ShouldDoCosmeticFiltering::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  const bool enabled = ::presearch_shields::ShouldDoCosmeticFiltering(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(enabled)));
}

ExtensionFunction::ResponseAction
PresearchShieldsSetCosmeticFilteringControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::SetCosmeticFilteringControlType::Params>
    params(
      presearch_shields::SetCosmeticFilteringControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsIsFirstPartyCosmeticFilteringEnabledFunction::Run() {
  std::unique_ptr<presearch_shields::IsFirstPartyCosmeticFilteringEnabled::Params>
      params(
          presearch_shields::IsFirstPartyCosmeticFilteringEnabled::Params::Create(
          *args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  const bool enabled = ::presearch_shields::IsFirstPartyCosmeticFilteringEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile), url);

  return RespondNow(OneArgument(base::Value(enabled)));
}

ExtensionFunction::ResponseAction PresearchShieldsSetAdControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::SetAdControlType::Params> params(
      presearch_shields::SetAdControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction PresearchShieldsGetAdControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::GetAdControlType::Params> params(
      presearch_shields::GetAdControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::presearch_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
PresearchShieldsSetCookieControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::SetCookieControlType::Params> params(
      presearch_shields::SetCookieControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsGetCookieControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::GetCookieControlType::Params> params(
      presearch_shields::GetCookieControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::presearch_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
PresearchShieldsSetFingerprintingControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::SetFingerprintingControlType::Params> params(
      presearch_shields::SetFingerprintingControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsGetFingerprintingControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::GetFingerprintingControlType::Params> params(
      presearch_shields::GetFingerprintingControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::presearch_shields::GetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
PresearchShieldsSetHTTPSEverywhereEnabledFunction::Run() {
  std::unique_ptr<presearch_shields::SetHTTPSEverywhereEnabled::Params> params(
      presearch_shields::SetHTTPSEverywhereEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      params->enabled,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsGetHTTPSEverywhereEnabledFunction::Run() {
  std::unique_ptr<presearch_shields::GetHTTPSEverywhereEnabled::Params> params(
      presearch_shields::GetHTTPSEverywhereEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::presearch_shields::GetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(type)));
}

ExtensionFunction::ResponseAction
PresearchShieldsSetNoScriptControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::SetNoScriptControlType::Params> params(
      presearch_shields::SetNoScriptControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::presearch_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchShieldsGetNoScriptControlTypeFunction::Run() {
  std::unique_ptr<presearch_shields::GetNoScriptControlType::Params> params(
      presearch_shields::GetNoScriptControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::presearch_shields::GetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
PresearchShieldsOnShieldsPanelShownFunction::Run() {
  ::presearch_shields::MaybeRecordShieldsUsageP3A(::presearch_shields::kClicked,
                                              g_browser_process->local_state());
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction PresearchShieldsReportBrokenSiteFunction::Run() {
  std::unique_ptr<presearch_shields::ReportBrokenSite::Params> params(
      presearch_shields::ReportBrokenSite::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
        params->tab_id,
        Profile::FromBrowserContext(browser_context()),
        false,
        nullptr,
        nullptr,
        &contents,
        nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  OpenWebcompatReporterDialog(contents);

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
