/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/presearch_rewards_api.h"

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "base/bind.h"
#include "base/strings/string_number_conversions.h"
#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/browser/presearch_rewards/rewards_service_factory.h"
#include "presearch/browser/presearch_rewards/tip_dialog.h"
#include "presearch/browser/extensions/api/presearch_action_api.h"
#include "presearch/browser/extensions/presearch_component_loader.h"
#include "presearch/browser/profiles/profile_util.h"
#include "presearch/common/extensions/api/presearch_rewards.h"
#include "presearch/components/presearch_ads/browser/ads_service.h"
#include "presearch/components/presearch_rewards/browser/rewards_service.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/chrome_extension_function_details.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"

using presearch_ads::AdsService;
using presearch_ads::AdsServiceFactory;
using presearch_rewards::RewardsService;
using presearch_rewards::RewardsServiceFactory;

namespace extensions {
namespace api {

PresearchRewardsOpenBrowserActionUIFunction::
~PresearchRewardsOpenBrowserActionUIFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsOpenBrowserActionUIFunction::Run() {
  std::unique_ptr<presearch_rewards::OpenBrowserActionUI::Params> params(
      presearch_rewards::OpenBrowserActionUI::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  auto* profile = Profile::FromBrowserContext(browser_context());

  // Start the rewards ledger process if it is not already started
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service)
    return RespondNow(Error("Rewards service is not initialized"));

  rewards_service->StartProcess(base::DoNothing());

  // Load the rewards extension if it is not already loaded
  auto* extension_service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!extension_service)
    return RespondNow(Error("Extension service is not initialized"));

  static_cast<PresearchComponentLoader*>(extension_service->component_loader())
      ->AddRewardsExtension();

  std::string error;
  if (!PresearchActionAPI::ShowActionUI(this,
      presearch_rewards_extension_id,
      std::move(params->window_id),
      std::move(params->relative_path), &error)) {
    return RespondNow(Error(error));
  }
  return RespondNow(NoArguments());
}

PresearchRewardsUpdateMediaDurationFunction::
    ~PresearchRewardsUpdateMediaDurationFunction() {}

ExtensionFunction::ResponseAction
PresearchRewardsUpdateMediaDurationFunction::Run() {
  std::unique_ptr<presearch_rewards::UpdateMediaDuration::Params> params(
      presearch_rewards::UpdateMediaDuration::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  rewards_service->UpdateMediaDuration(
      params->window_id,
      params->publisher_key,
      params->duration,
      params->first_visit);

  return RespondNow(NoArguments());
}

PresearchRewardsGetPublisherInfoFunction::
~PresearchRewardsGetPublisherInfoFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetPublisherInfoFunction::Run() {
  std::unique_ptr<presearch_rewards::GetPublisherInfo::Params> params(
      presearch_rewards::GetPublisherInfo::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetPublisherInfo(
      params->publisher_key,
      base::Bind(
          &PresearchRewardsGetPublisherInfoFunction::OnGetPublisherInfo,
          this));

  return RespondLater();
}

void PresearchRewardsGetPublisherInfoFunction::OnGetPublisherInfo(
    const ledger::type::Result result,
    ledger::type::PublisherInfoPtr info) {
  if (!info) {
    Respond(OneArgument(base::Value(static_cast<int>(result))));
    return;
  }

  base::Value dict(base::Value::Type::DICTIONARY);
  dict.SetStringKey("publisherKey", info->id);
  dict.SetStringKey("name", info->name);
  dict.SetIntKey("percentage", info->percent);
  dict.SetIntKey("status", static_cast<int>(info->status));
  dict.SetBoolKey("excluded",
                  info->excluded == ledger::type::PublisherExclude::EXCLUDED);
  dict.SetStringKey("url", info->url);
  dict.SetStringKey("provider", info->provider);
  dict.SetStringKey("favIconUrl", info->favicon_url);

  Respond(TwoArguments(base::Value(static_cast<int>(result)), std::move(dict)));
}

PresearchRewardsGetPublisherPanelInfoFunction::
    ~PresearchRewardsGetPublisherPanelInfoFunction() {}

ExtensionFunction::ResponseAction
PresearchRewardsGetPublisherPanelInfoFunction::Run() {
  std::unique_ptr<presearch_rewards::GetPublisherPanelInfo::Params> params(
      presearch_rewards::GetPublisherPanelInfo::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  rewards_service->GetPublisherPanelInfo(
      params->publisher_key,
      base::Bind(
          &PresearchRewardsGetPublisherPanelInfoFunction::OnGetPublisherPanelInfo,
          this));

  return RespondLater();
}

void PresearchRewardsGetPublisherPanelInfoFunction::OnGetPublisherPanelInfo(
    const ledger::type::Result result,
    ledger::type::PublisherInfoPtr info) {
  if (!info) {
    Respond(OneArgument(base::Value(static_cast<int>(result))));
    return;
  }

  base::Value dict(base::Value::Type::DICTIONARY);
  dict.SetStringKey("publisherKey", info->id);
  dict.SetStringKey("name", info->name);
  dict.SetIntKey("percentage", info->percent);
  dict.SetIntKey("status", static_cast<int>(info->status));
  dict.SetBoolKey("excluded",
                  info->excluded == ledger::type::PublisherExclude::EXCLUDED);
  dict.SetStringKey("url", info->url);
  dict.SetStringKey("provider", info->provider);
  dict.SetStringKey("favIconUrl", info->favicon_url);

  Respond(TwoArguments(base::Value(static_cast<int>(result)), std::move(dict)));
}

PresearchRewardsSavePublisherInfoFunction::
    ~PresearchRewardsSavePublisherInfoFunction() {}

ExtensionFunction::ResponseAction
PresearchRewardsSavePublisherInfoFunction::Run() {
  std::unique_ptr<presearch_rewards::SavePublisherInfo::Params>
      params(presearch_rewards::SavePublisherInfo::Params::Create(
          *args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  auto publisher_info = ledger::type::PublisherInfo::New();
  publisher_info->id = params->publisher_key;
  publisher_info->name = params->publisher_name;
  publisher_info->url = params->url;
  publisher_info->provider = params->media_type;
  publisher_info->favicon_url = params->fav_icon_url;

  rewards_service->SavePublisherInfo(
      params->window_id,
      std::move(publisher_info),
      base::Bind(
          &PresearchRewardsSavePublisherInfoFunction::OnSavePublisherInfo,
          this));

  return RespondLater();
}

void PresearchRewardsSavePublisherInfoFunction::OnSavePublisherInfo(
    const ledger::type::Result result) {
  Respond(OneArgument(base::Value(static_cast<int>(result))));
}

PresearchRewardsTipSiteFunction::~PresearchRewardsTipSiteFunction() {
}

ExtensionFunction::ResponseAction PresearchRewardsTipSiteFunction::Run() {
  std::unique_ptr<presearch_rewards::TipSite::Params> params(
      presearch_rewards::TipSite::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Sanity check: don't allow tips in private / tor contexts,
  // although the command should not have been enabled in the first place.
  if (!presearch::IsRegularProfile(browser_context())) {
    return RespondNow(Error("Cannot tip to site in a private context"));
  }

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

  auto params_dict = std::make_unique<base::DictionaryValue>();
  params_dict->SetString("publisherKey", params->publisher_key);
  params_dict->SetString("entryPoint", params->entry_point);
  params_dict->SetString(
      "url", contents ? contents->GetLastCommittedURL().spec() : std::string());
  ::presearch_rewards::OpenTipDialog(contents, std::move(params_dict));

  return RespondNow(NoArguments());
}

PresearchRewardsTipUserFunction::PresearchRewardsTipUserFunction()
    : weak_factory_(this) {
}

PresearchRewardsTipUserFunction::~PresearchRewardsTipUserFunction() {
}

ExtensionFunction::ResponseAction PresearchRewardsTipUserFunction::Run() {
  std::unique_ptr<presearch_rewards::TipUser::Params> params(
      presearch_rewards::TipUser::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Sanity check: don't allow tips in private / tor contexts,
  // although the command should not have been enabled in the first place.
  if (!presearch::IsRegularProfile(browser_context())) {
    return RespondNow(Error("Cannot tip user in a private context"));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  extensions::ExtensionService* extension_service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!extension_service) {
    return RespondNow(Error("Extension service is not initialized"));
  }

  AddRef();

  extensions::ComponentLoader* component_loader =
      extension_service->component_loader();
  static_cast<extensions::PresearchComponentLoader*>(component_loader)
      ->AddRewardsExtension();

  rewards_service->StartProcess(
      base::BindOnce(&PresearchRewardsTipUserFunction::OnProcessStarted, this,
                     params->publisher_key));

  return RespondNow(NoArguments());
}

void PresearchRewardsTipUserFunction::OnProcessStarted(
    const std::string& publisher_key) {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    Release();
    return;
  }
  rewards_service->GetPublisherInfo(
      publisher_key,
      base::Bind(&PresearchRewardsTipUserFunction::OnTipUserGetPublisherInfo,
                 this));
}

void PresearchRewardsTipUserFunction::OnTipUserGetPublisherInfo(
    const ledger::type::Result result,
    ledger::type::PublisherInfoPtr info) {
  if (result != ledger::type::Result::LEDGER_OK &&
      result != ledger::type::Result::NOT_FOUND) {
    Release();
    return;
  }

  if (result == ledger::type::Result::LEDGER_OK) {
    ShowTipDialog();
    Release();
    return;
  }

  std::unique_ptr<presearch_rewards::TipUser::Params> params(
      presearch_rewards::TipUser::Params::Create(*args_));

  auto publisher_info = ledger::type::PublisherInfo::New();
  publisher_info->id = params->publisher_key;
  publisher_info->name = params->publisher_name;
  publisher_info->url = params->url;
  publisher_info->provider = params->media_type;
  publisher_info->favicon_url = params->fav_icon_url;

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    Release();
    return;
  }

  rewards_service->SavePublisherInfo(
      0,
      std::move(publisher_info),
      base::Bind(&PresearchRewardsTipUserFunction::
                 OnTipUserSavePublisherInfo,
                 weak_factory_.GetWeakPtr()));
}

void PresearchRewardsTipUserFunction::OnTipUserSavePublisherInfo(
    const ledger::type::Result result) {
  if (result != ledger::type::Result::LEDGER_OK) {
    Release();
    return;
  }

  ShowTipDialog();
  Release();
}

void PresearchRewardsTipUserFunction::ShowTipDialog() {
  std::unique_ptr<presearch_rewards::TipUser::Params> params(
      presearch_rewards::TipUser::Params::Create(*args_));
  if (!params) {
    Release();
    return;
  }

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
    Release();
    return;
  }

  base::Value media_meta_data_dict(base::Value::Type::DICTIONARY);
  media_meta_data_dict.SetStringKey("mediaType", params->media_type);
  media_meta_data_dict.SetStringKey("publisherKey", params->publisher_key);
  media_meta_data_dict.SetStringKey("publisherName", params->publisher_name);
  media_meta_data_dict.SetStringKey(
      "publisherScreenName",
      params->publisher_screen_name);
  media_meta_data_dict.SetStringKey("postId", params->post_id);
  media_meta_data_dict.SetStringKey("postTimestamp", params->post_timestamp);
  media_meta_data_dict.SetStringKey("postText", params->post_text);

  auto params_dict = std::make_unique<base::DictionaryValue>();
  params_dict->SetString("publisherKey", params->publisher_key);
  params_dict->SetString("url", params->url);
  params_dict->SetPath("mediaMetaData", std::move(media_meta_data_dict));

  ::presearch_rewards::OpenTipDialog(contents, std::move(params_dict));
}

PresearchRewardsGetPublisherDataFunction::~PresearchRewardsGetPublisherDataFunction() {
}

PresearchRewardsIncludeInAutoContributionFunction::
  ~PresearchRewardsIncludeInAutoContributionFunction() {
}

ExtensionFunction::ResponseAction
  PresearchRewardsIncludeInAutoContributionFunction::Run() {
  std::unique_ptr<presearch_rewards::IncludeInAutoContribution::Params> params(
    presearch_rewards::IncludeInAutoContribution::Params::Create(*args_));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (rewards_service) {
    rewards_service->SetPublisherExclude(
      params->publisher_key,
      params->exclude);
  }
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction PresearchRewardsGetPublisherDataFunction::Run() {
  std::unique_ptr<presearch_rewards::GetPublisherData::Params> params(
      presearch_rewards::GetPublisherData::Params::Create(*args_));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (rewards_service) {
    rewards_service->GetPublisherActivityFromUrl(params->window_id,
                                                  params->url,
                                                  params->favicon_url,
                                                  params->publisher_blob);
  }
  return RespondNow(NoArguments());
}

PresearchRewardsGetRewardsParametersFunction::
~PresearchRewardsGetRewardsParametersFunction() = default;

ExtensionFunction::ResponseAction
PresearchRewardsGetRewardsParametersFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    base::Value data(base::Value::Type::DICTIONARY);
    return RespondNow(OneArgument(std::move(data)));
  }

  rewards_service->GetRewardsParameters(base::BindOnce(
      &PresearchRewardsGetRewardsParametersFunction::OnGet,
      this));
  return RespondLater();
}

void PresearchRewardsGetRewardsParametersFunction::OnGet(
    ledger::type::RewardsParametersPtr parameters) {
  base::DictionaryValue data;

  if (!parameters) {
    return Respond(OneArgument(std::move(data)));
  }

  data.SetDouble("rate", parameters->rate);
  auto monthly_choices = std::make_unique<base::ListValue>();
  for (auto const& item : parameters->monthly_tip_choices) {
    monthly_choices->Append(base::Value(item));
  }
  data.SetList("monthlyTipChoices", std::move(monthly_choices));

  auto ac_choices = std::make_unique<base::ListValue>();
  for (double const& choice : parameters->auto_contribute_choices) {
    ac_choices->AppendDouble(choice);
  }
  data.SetList("autoContributeChoices", std::move(ac_choices));

  Respond(OneArgument(std::move(data)));
}

PresearchRewardsGetBalanceReportFunction::
~PresearchRewardsGetBalanceReportFunction() = default;

ExtensionFunction::ResponseAction PresearchRewardsGetBalanceReportFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    base::Value data(base::Value::Type::DICTIONARY);
    return RespondNow(OneArgument(std::move(data)));
  }

  std::unique_ptr<presearch_rewards::GetBalanceReport::Params> params(
      presearch_rewards::GetBalanceReport::Params::Create(*args_));

  rewards_service->GetBalanceReport(
      params->month,
      params->year,
      base::BindOnce(
          &PresearchRewardsGetBalanceReportFunction::OnBalanceReport,
          this));
  return RespondLater();
}

void PresearchRewardsGetBalanceReportFunction::OnBalanceReport(
    const ledger::type::Result result,
    ledger::type::BalanceReportInfoPtr report) {
  base::Value data(base::Value::Type::DICTIONARY);
  if (!report) {
    Respond(OneArgument(std::move(data)));
    return;
  }
  data.SetDoubleKey("ads", report->earning_from_ads);
  data.SetDoubleKey("contribute", report->auto_contribute);
  data.SetDoubleKey("grant", report->grants);
  data.SetDoubleKey("tips", report->one_time_donation);
  data.SetDoubleKey("monthly", report->recurring_donation);
  Respond(OneArgument(std::move(data)));
}

PresearchRewardsFetchPromotionsFunction::
~PresearchRewardsFetchPromotionsFunction() = default;

ExtensionFunction::ResponseAction PresearchRewardsFetchPromotionsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (rewards_service) {
    rewards_service->FetchPromotions();
  }
  return RespondNow(NoArguments());
}

PresearchRewardsClaimPromotionFunction::
~PresearchRewardsClaimPromotionFunction() = default;

ExtensionFunction::ResponseAction PresearchRewardsClaimPromotionFunction::Run() {
  std::unique_ptr<presearch_rewards::ClaimPromotion::Params> params(
      presearch_rewards::ClaimPromotion::Params::Create(*args_));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    base::Value data(base::Value::Type::DICTIONARY);
    data.SetIntKey("result", 1);
    return RespondNow(OneArgument(std::move(data)));
  }

  rewards_service->ClaimPromotion(
      params->promotion_id,
      base::BindOnce(
          &PresearchRewardsClaimPromotionFunction::OnClaimPromotion,
          this,
          params->promotion_id));
  return RespondLater();
}

void PresearchRewardsClaimPromotionFunction::OnClaimPromotion(
    const std::string& promotion_id,
    const ledger::type::Result result,
    const std::string& captcha_image,
    const std::string& hint,
    const std::string& captcha_id) {
  base::Value data(base::Value::Type::DICTIONARY);
  data.SetIntKey("result", static_cast<int>(result));
  data.SetStringKey("promotionId", promotion_id);
  data.SetStringKey("captchaImage", captcha_image);
  data.SetStringKey("captchaId", captcha_id);
  data.SetStringKey("hint", hint);
  Respond(OneArgument(std::move(data)));
}

PresearchRewardsAttestPromotionFunction::
~PresearchRewardsAttestPromotionFunction() = default;

ExtensionFunction::ResponseAction PresearchRewardsAttestPromotionFunction::Run() {
  std::unique_ptr<presearch_rewards::AttestPromotion::Params> params(
      presearch_rewards::AttestPromotion::Params::Create(*args_));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(OneArgument(base::Value(1)));
  }

  rewards_service->AttestPromotion(params->promotion_id, params->solution,
      base::BindOnce(
        &PresearchRewardsAttestPromotionFunction::OnAttestPromotion,
        this,
        params->promotion_id));
  return RespondLater();
}

void PresearchRewardsAttestPromotionFunction::OnAttestPromotion(
    const std::string& promotion_id,
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  base::Value data(base::Value::Type::DICTIONARY);
  data.SetStringKey("promotionId", promotion_id);

  if (!promotion) {
    Respond(
        TwoArguments(base::Value(static_cast<int>(result)), std::move(data)));
    return;
  }

  data.SetIntKey("expiresAt", promotion->expires_at);
  data.SetDoubleKey("amount", promotion->approximate_value);
  data.SetIntKey("type", static_cast<int>(promotion->type));
  Respond(TwoArguments(base::Value(static_cast<int>(result)), std::move(data)));
}

PresearchRewardsGetPendingContributionsTotalFunction::
~PresearchRewardsGetPendingContributionsTotalFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetPendingContributionsTotalFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(OneArgument(base::Value(0.0)));
  }

  rewards_service->GetPendingContributionsTotal(base::Bind(
        &PresearchRewardsGetPendingContributionsTotalFunction::OnGetPendingTotal,
        this));
  return RespondLater();
}

void PresearchRewardsGetPendingContributionsTotalFunction::OnGetPendingTotal(
    double amount) {
  Respond(OneArgument(base::Value(amount)));
}

PresearchRewardsSaveAdsSettingFunction::~PresearchRewardsSaveAdsSettingFunction() {
}

ExtensionFunction::ResponseAction PresearchRewardsSaveAdsSettingFunction::Run() {
  std::unique_ptr<presearch_rewards::SaveAdsSetting::Params> params(
      presearch_rewards::SaveAdsSetting::Params::Create(*args_));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);

  if (!rewards_service || !ads_service) {
    return RespondNow(Error("Service is not initialized"));
  }

  if (params->key == "adsEnabled") {
    const auto is_enabled =
        params->value == "true" && ads_service->IsSupportedLocale();
    rewards_service->SetAdsEnabled(is_enabled);
  }

  return RespondNow(NoArguments());
}

PresearchRewardsSetAutoContributeEnabledFunction::
~PresearchRewardsSetAutoContributeEnabledFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsSetAutoContributeEnabledFunction::Run() {
  std::unique_ptr<presearch_rewards::SetAutoContributeEnabled::Params> params(
      presearch_rewards::SetAutoContributeEnabled::Params::Create(*args_));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->SetAutoContributeEnabled(params->enabled);
  return RespondNow(NoArguments());
}

PresearchRewardsGetACEnabledFunction::
~PresearchRewardsGetACEnabledFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetACEnabledFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetAutoContributeEnabled(base::BindOnce(
        &PresearchRewardsGetACEnabledFunction::OnGetACEnabled,
        this));
  return RespondLater();
}

void PresearchRewardsGetACEnabledFunction::OnGetACEnabled(bool enabled) {
  Respond(OneArgument(base::Value(enabled)));
}

PresearchRewardsSaveRecurringTipFunction::
~PresearchRewardsSaveRecurringTipFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsSaveRecurringTipFunction::Run() {
  std::unique_ptr<presearch_rewards::SaveRecurringTip::Params> params(
    presearch_rewards::SaveRecurringTip::Params::Create(*args_));

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service_ =
    RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service_) {
    return RespondNow(NoArguments());
  }

  rewards_service_->SaveRecurringTip(
      params->publisher_key,
      params->new_amount,
      base::Bind(
          &PresearchRewardsSaveRecurringTipFunction::OnSaveRecurringTip,
          this));

  return RespondLater();
}

void PresearchRewardsSaveRecurringTipFunction::OnSaveRecurringTip(bool success) {
  if (!success) {
    Respond(Error("Failed to save"));
    return;
  }
  Respond(NoArguments());
}

PresearchRewardsRemoveRecurringTipFunction::
~PresearchRewardsRemoveRecurringTipFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsRemoveRecurringTipFunction::Run() {
  std::unique_ptr<presearch_rewards::RemoveRecurringTip::Params> params(
    presearch_rewards::RemoveRecurringTip::Params::Create(*args_));

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service_ =
    RewardsServiceFactory::GetForProfile(profile);

  if (rewards_service_) {
    rewards_service_->RemoveRecurringTip(params->publisher_key);
  }

  return RespondNow(NoArguments());
}

PresearchRewardsGetRecurringTipsFunction::
~PresearchRewardsGetRecurringTipsFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetRecurringTipsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetRecurringTips(base::Bind(
        &PresearchRewardsGetRecurringTipsFunction::OnGetRecurringTips,
        this));
  return RespondLater();
}

void PresearchRewardsGetRecurringTipsFunction::OnGetRecurringTips(
    ledger::type::PublisherInfoList list) {
  base::DictionaryValue result;
  auto recurringTips = std::make_unique<base::ListValue>();

  if (!list.empty()) {
    for (const auto& item : list) {
      auto tip = std::make_unique<base::DictionaryValue>();
      tip->SetString("publisherKey", item->id);
      tip->SetDouble("amount", item->weight);
      recurringTips->Append(std::move(tip));
    }
  }

  result.SetList("recurringTips", std::move(recurringTips));
  Respond(OneArgument(std::move(result)));
}

PresearchRewardsGetPublisherBannerFunction::
~PresearchRewardsGetPublisherBannerFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetPublisherBannerFunction::Run() {
  std::unique_ptr<presearch_rewards::GetPublisherBanner::Params> params(
    presearch_rewards::GetPublisherBanner::Params::Create(*args_));

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetPublisherBanner(
      params->publisher_key,
      base::BindOnce(
        &PresearchRewardsGetPublisherBannerFunction::OnPublisherBanner,
        this));
  return RespondLater();
}

void PresearchRewardsGetPublisherBannerFunction::OnPublisherBanner(
    ledger::type::PublisherBannerPtr banner) {
  base::DictionaryValue result;

  if (banner) {
    result.SetString("publisherKey", banner->publisher_key);
    result.SetString("title", banner->title);
    result.SetString("name", banner->name);
    result.SetString("description", banner->description);
    result.SetString("background", banner->background);
    result.SetString("logo", banner->logo);
    result.SetString("provider", banner->provider);
    result.SetInteger("verified", static_cast<int>(banner->status));

    auto amounts = std::make_unique<base::ListValue>();
    for (auto const& value : banner->amounts) {
      amounts->AppendInteger(value);
    }
    result.SetList("amounts", std::move(amounts));

    auto links = std::make_unique<base::DictionaryValue>();
    for (auto const& item : banner->links) {
      links->SetString(item.first, item.second);
    }
    result.SetDictionary("links", std::move(links));
  }

  Respond(OneArgument(std::move(result)));
}

PresearchRewardsRefreshPublisherFunction::~PresearchRewardsRefreshPublisherFunction() {
}

ExtensionFunction::ResponseAction PresearchRewardsRefreshPublisherFunction::Run() {
  std::unique_ptr<presearch_rewards::RefreshPublisher::Params> params(
      presearch_rewards::RefreshPublisher::Params::Create(*args_));

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(
        TwoArguments(base::Value(false), base::Value(std::string())));
  }
  rewards_service->RefreshPublisher(
      params->publisher_key,
      base::BindOnce(
        &PresearchRewardsRefreshPublisherFunction::OnRefreshPublisher,
        this));
  return RespondLater();
}

void PresearchRewardsRefreshPublisherFunction::OnRefreshPublisher(
    const ledger::type::PublisherStatus status,
    const std::string& publisher_key) {
  Respond(TwoArguments(base::Value(static_cast<int>(status)),
                       base::Value(publisher_key)));
}

PresearchRewardsGetAllNotificationsFunction::
~PresearchRewardsGetAllNotificationsFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetAllNotificationsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);

  base::ListValue list;

  if (!rewards_service) {
    return RespondNow(OneArgument(std::move(list)));
  }

  auto notifications = rewards_service->GetAllNotifications();

  for (auto const& notification : notifications) {
    auto item = std::make_unique<base::DictionaryValue>();
    item->SetString("id", notification.second.id_);
    item->SetInteger("type", notification.second.type_);
    item->SetInteger("timestamp", notification.second.timestamp_);

    auto args = std::make_unique<base::ListValue>();
    for (auto const& arg : notification.second.args_) {
      args->AppendString(arg);
    }

    item->SetList("args", std::move(args));
    list.Append(std::move(item));
  }

  return RespondNow(OneArgument(std::move(list)));
}

PresearchRewardsGetInlineTippingPlatformEnabledFunction::
~PresearchRewardsGetInlineTippingPlatformEnabledFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetInlineTippingPlatformEnabledFunction::Run() {
  std::unique_ptr<presearch_rewards::GetInlineTippingPlatformEnabled::Params>
      params(presearch_rewards::GetInlineTippingPlatformEnabled::Params::Create(
          *args_));

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(OneArgument(base::Value(false)));
  }

  rewards_service->GetInlineTippingPlatformEnabled(
      params->key,
      base::BindOnce(
          &PresearchRewardsGetInlineTippingPlatformEnabledFunction::
          OnInlineTipSetting,
          this));
  return RespondLater();
}

void PresearchRewardsGetInlineTippingPlatformEnabledFunction::OnInlineTipSetting(
    bool value) {
  Respond(OneArgument(base::Value(value)));
}

PresearchRewardsFetchBalanceFunction::
~PresearchRewardsFetchBalanceFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsFetchBalanceFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    base::DictionaryValue balance_value;
    return RespondNow(OneArgument(std::move(balance_value)));
  }

  rewards_service->FetchBalance(
      base::BindOnce(
          &PresearchRewardsFetchBalanceFunction::OnBalance,
          this));
  return RespondLater();
}

void PresearchRewardsFetchBalanceFunction::OnBalance(
    const ledger::type::Result result,
    ledger::type::BalancePtr balance) {
  base::Value balance_value(base::Value::Type::DICTIONARY);
  if (result == ledger::type::Result::LEDGER_OK && balance) {
    balance_value.SetDoubleKey("total", balance->total);

    base::Value wallets(base::Value::Type::DICTIONARY);
    for (auto const& rate : balance->wallets) {
      wallets.SetDoubleKey(rate.first, rate.second);
    }
    balance_value.SetKey("wallets", std::move(wallets));
  } else {
    balance_value.SetDoubleKey("total", 0.0);
    base::Value wallets(base::Value::Type::DICTIONARY);
    balance_value.SetKey("wallets", std::move(wallets));
  }

  Respond(OneArgument(std::move(balance_value)));
}

PresearchRewardsGetExternalWalletFunction::
~PresearchRewardsGetExternalWalletFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetExternalWalletFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    base::Value data(base::Value::Type::DICTIONARY);
    return RespondNow(OneArgument(std::move(data)));
  }

  rewards_service->GetExternalWallet(base::BindOnce(
      &PresearchRewardsGetExternalWalletFunction::OnGetExternalWallet, this));
  return RespondLater();
}

void PresearchRewardsGetExternalWalletFunction::OnGetExternalWallet(
    const ledger::type::Result result,
    ledger::type::ExternalWalletPtr wallet) {
  if (!wallet) {
    Respond(OneArgument(base::Value(static_cast<int>(result))));
    return;
  }

  base::Value data(base::Value::Type::DICTIONARY);

  data.SetStringKey("type", wallet->type);
  data.SetStringKey("address", wallet->address);
  data.SetIntKey("status", static_cast<int>(wallet->status));
  data.SetStringKey("verifyUrl", wallet->verify_url);
  data.SetStringKey("addUrl", wallet->add_url);
  data.SetStringKey("withdrawUrl", wallet->withdraw_url);
  data.SetStringKey("userName", wallet->user_name);
  data.SetStringKey("accountUrl", wallet->account_url);
  data.SetStringKey("loginUrl", wallet->login_url);

  Respond(TwoArguments(base::Value(static_cast<int>(result)), std::move(data)));
}

PresearchRewardsDisconnectWalletFunction::
~PresearchRewardsDisconnectWalletFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsDisconnectWalletFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  rewards_service->DisconnectWallet();
  return RespondNow(NoArguments());
}

PresearchRewardsOnlyAnonWalletFunction::
~PresearchRewardsOnlyAnonWalletFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsOnlyAnonWalletFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(OneArgument(base::Value(false)));
  }

  const auto only = rewards_service->OnlyAnonWallet();
  return RespondNow(OneArgument(base::Value(only)));
}

PresearchRewardsGetAdsEnabledFunction::
~PresearchRewardsGetAdsEnabledFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetAdsEnabledFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  AdsService* ads_service =
      AdsServiceFactory::GetForProfile(profile);

  if (!ads_service) {
    return RespondNow(Error("Ads service is not initialized"));
  }

  const bool enabled = ads_service->IsEnabled();
  return RespondNow(OneArgument(base::Value(enabled)));
}

PresearchRewardsGetAdsAccountStatementFunction::
    ~PresearchRewardsGetAdsAccountStatementFunction() {}

ExtensionFunction::ResponseAction
PresearchRewardsGetAdsAccountStatementFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  AdsService* ads_service =
      AdsServiceFactory::GetForProfile(profile);

  if (!ads_service) {
    return RespondNow(Error("Ads service is not initialized"));
  }

  AddRef();  // Balanced in OnGetAdsAccountStatement().

  ads_service->GetAccountStatement(base::BindOnce(
      &PresearchRewardsGetAdsAccountStatementFunction::OnGetAdsAccountStatement,
      this));
  return RespondLater();
}

void PresearchRewardsGetAdsAccountStatementFunction::OnGetAdsAccountStatement(
    const bool success,
    const double estimated_pending_rewards,
    const int64_t next_payment_date,
    const int ads_received_this_month,
    const double earnings_this_month,
    const double earnings_last_month) {
  if (!success) {
    Respond(OneArgument(base::Value(success)));
  } else {
    base::Value statement(base::Value::Type::DICTIONARY);
    statement.SetDoubleKey("estimatedPendingRewards",
                           estimated_pending_rewards);
    const std::string next_payment_date_as_string =
        base::NumberToString(next_payment_date);
    statement.SetStringKey("nextPaymentDate", next_payment_date_as_string);
    statement.SetIntKey("adsReceivedThisMonth", ads_received_this_month);
    statement.SetDoubleKey("earningsThisMonth", earnings_this_month);
    statement.SetDoubleKey("earningsLastMonth", earnings_last_month);

    Respond(TwoArguments(base::Value(success), std::move(statement)));
  }

  Release();  // Balanced in Run()
}

PresearchRewardsGetAdsSupportedFunction::
~PresearchRewardsGetAdsSupportedFunction() {
}

ExtensionFunction::ResponseAction
PresearchRewardsGetAdsSupportedFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  AdsService* ads_service =
      AdsServiceFactory::GetForProfile(profile);

  if (!ads_service) {
    return RespondNow(Error("Ads service is not initialized"));
  }

  const bool supported = ads_service->IsSupportedLocale();
  return RespondNow(OneArgument(base::Value(supported)));
}

PresearchRewardsGetAnonWalletStatusFunction::
~PresearchRewardsGetAnonWalletStatusFunction() = default;

ExtensionFunction::ResponseAction
PresearchRewardsGetAnonWalletStatusFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
    RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetAnonWalletStatus(base::Bind(
        &PresearchRewardsGetAnonWalletStatusFunction::OnGetAnonWalletStatus,
        this));
  return RespondLater();
}

void PresearchRewardsGetAnonWalletStatusFunction::OnGetAnonWalletStatus(
    const ledger::type::Result result) {
  Respond(OneArgument(base::Value(static_cast<int>(result))));
}

PresearchRewardsIsInitializedFunction::
~PresearchRewardsIsInitializedFunction() = default;

ExtensionFunction::ResponseAction
PresearchRewardsIsInitializedFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  return RespondNow(OneArgument(
      base::Value(rewards_service && rewards_service->IsInitialized())));
}

PresearchRewardsShouldShowOnboardingFunction::
~PresearchRewardsShouldShowOnboardingFunction() = default;

ExtensionFunction::ResponseAction
PresearchRewardsShouldShowOnboardingFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  const bool should_show = rewards_service->ShouldShowOnboarding();
  return RespondNow(OneArgument(base::Value(should_show)));
}

PresearchRewardsEnableRewardsFunction::~PresearchRewardsEnableRewardsFunction() =
    default;

ExtensionFunction::ResponseAction PresearchRewardsEnableRewardsFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service)
    return RespondNow(Error("Rewards service is not initialized"));

  rewards_service->EnableRewards();
  return RespondNow(NoArguments());
}

PresearchRewardsGetPrefsFunction::~PresearchRewardsGetPrefsFunction() = default;

ExtensionFunction::ResponseAction PresearchRewardsGetPrefsFunction::Run() {
  auto* rewards_service = RewardsServiceFactory::GetForProfile(
      Profile::FromBrowserContext(browser_context()));

  if (!rewards_service)
    return RespondNow(Error("Rewards service is not initialized"));

  rewards_service->GetAutoContributeProperties(base::BindRepeating(
      &PresearchRewardsGetPrefsFunction::GetAutoContributePropertiesCallback,
      this));

  return RespondLater();
}

void PresearchRewardsGetPrefsFunction::GetAutoContributePropertiesCallback(
    ledger::type::AutoContributePropertiesPtr properties) {
  base::Value prefs(base::Value::Type::DICTIONARY);
  prefs.SetBoolKey("autoContributeEnabled", properties->enabled_contribute);
  prefs.SetDoubleKey("autoContributeAmount", properties->amount);

  auto* ads_service = AdsServiceFactory::GetForProfile(
      Profile::FromBrowserContext(browser_context()));

  if (ads_service) {
    prefs.SetBoolKey("adsEnabled", ads_service->IsEnabled());
    prefs.SetDoubleKey("adsPerHour",
                       static_cast<double>(ads_service->GetAdsPerHour()));
  } else {
    prefs.SetBoolKey("adsEnabled", false);
    prefs.SetDoubleKey("adsPerHour", 0);
  }

  Respond(OneArgument(std::move(prefs)));
}

PresearchRewardsUpdatePrefsFunction::~PresearchRewardsUpdatePrefsFunction() = default;

ExtensionFunction::ResponseAction PresearchRewardsUpdatePrefsFunction::Run() {
  auto params = presearch_rewards::UpdatePrefs::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(params);

  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  auto* ads_service = AdsServiceFactory::GetForProfile(profile);

  if (rewards_service) {
    bool* ac_enabled = params->prefs.auto_contribute_enabled.get();
    if (ac_enabled)
      rewards_service->SetAutoContributeEnabled(*ac_enabled);

    double* ac_amount = params->prefs.auto_contribute_amount.get();
    if (ac_amount)
      rewards_service->SetAutoContributionAmount(*ac_amount);
  }

  if (ads_service) {
    bool* ads_enabled = params->prefs.ads_enabled.get();
    if (ads_enabled)
      ads_service->SetEnabled(*ads_enabled);

    int* ads_per_hour = params->prefs.ads_per_hour.get();
    if (ads_per_hour)
      ads_service->SetAdsPerHour(*ads_per_hour);
  }

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
