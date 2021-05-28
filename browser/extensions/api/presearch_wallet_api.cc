/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/extensions/api/presearch_wallet_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "presearch/browser/presearch_wallet/presearch_wallet_service_factory.h"
#include "presearch/browser/extensions/presearch_wallet_util.h"
#include "presearch/browser/infobars/crypto_wallets_infobar_delegate.h"
#include "presearch/common/extensions/api/presearch_wallet.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_service.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_utils.h"
#include "presearch/components/presearch_wallet/browser/pref_names.h"
#include "presearch/grit/presearch_generated_resources.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_util.h"
#include "extensions/common/constants.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

PresearchWalletService* GetPresearchWalletService(
    content::BrowserContext* context) {
  return PresearchWalletServiceFactory::GetInstance()->GetForContext(context);
}

base::Value MakeSelectValue(const  base::string16& name,
                            PresearchWalletWeb3ProviderTypes value) {
  base::Value item(base::Value::Type::DICTIONARY);
  item.SetKey("value", base::Value(static_cast<int>(value)));
  item.SetKey("name", base::Value(name));
  return item;
}

}  // namespace

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction
PresearchWalletPromptToEnableWalletFunction::Run() {
  std::unique_ptr<presearch_wallet::PromptToEnableWallet::Params> params(
      presearch_wallet::PromptToEnableWallet::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  if (browser_context()->IsTor()) {
    return RespondNow(Error("Not available in Tor context"));
  }

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
        params->tab_id,
        Profile::FromBrowserContext(browser_context()),
        include_incognito_information(),
        nullptr,
        nullptr,
        &contents,
        nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  InfoBarService* infobar_service =
      InfoBarService::FromWebContents(contents);
  if (infobar_service) {
    CryptoWalletsInfoBarDelegate::InfobarSubType subtype =
        CryptoWalletsInfoBarDelegate::InfobarSubType::GENERIC_SETUP;
    auto* service = GetPresearchWalletService(browser_context());
    if (service->ShouldShowLazyLoadInfobar()) {
      subtype = CryptoWalletsInfoBarDelegate::InfobarSubType::
          LOAD_CRYPTO_WALLETS;
    }
    CryptoWalletsInfoBarDelegate::Create(infobar_service, subtype);
  }

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchWalletReadyFunction::Run() {
  if (browser_context()->IsTor()) {
    return RespondNow(Error("Not available in Tor context"));
  }

  auto* service = GetPresearchWalletService(browser_context());
  service->CryptoWalletsExtensionReady();
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchWalletLoadUIFunction::Run() {
  auto* service = GetPresearchWalletService(browser_context());
  // If the extension is already ready, respond right away
  if (service->IsCryptoWalletsReady()) {
    return RespondNow(NoArguments());
  }

  // If the user has opt-ed in and MetaMask is not installed, and
  // the new Presearch Wallet is not the default, then
  // set the Dapp provider to Crypto Wallets.
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto provider = static_cast<PresearchWalletWeb3ProviderTypes>(
      profile->GetPrefs()->GetInteger(kPresearchWalletWeb3Provider));
  auto* registry = extensions::ExtensionRegistry::Get(profile);
  if (!registry->ready_extensions().Contains(metamask_extension_id) &&
      provider != PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET) {
    profile->GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
        static_cast<int>(PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  }
  profile->GetPrefs()->SetBoolean(kOptedIntoCryptoWallets, true);
  service->MaybeLoadCryptoWalletsExtension(
      base::BindOnce(&PresearchWalletLoadUIFunction::OnLoaded, this));
  return RespondLater();
}

void PresearchWalletLoadUIFunction::OnLoaded() {
  Respond(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchWalletShouldPromptForSetupFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* service = GetPresearchWalletService(browser_context());
  bool should_prompt = !service->IsCryptoWalletsSetup() &&
      !profile->GetPrefs()->GetBoolean(kOptedIntoCryptoWallets);
  return RespondNow(OneArgument(base::Value(should_prompt)));
}

ExtensionFunction::ResponseAction
PresearchWalletShouldCheckForDappsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  if (browser_context()->IsTor()) {
    return RespondNow(OneArgument(base::Value(false)));
  }
  auto provider = static_cast<PresearchWalletWeb3ProviderTypes>(
      profile->GetPrefs()->GetInteger(kPresearchWalletWeb3Provider));
  if (provider == PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET) {
    return RespondNow(OneArgument(base::Value(false)));
  }
  auto* registry = extensions::ExtensionRegistry::Get(profile);
  bool has_metamask =
      registry->ready_extensions().Contains(metamask_extension_id);

  auto* service = GetPresearchWalletService(browser_context());
  bool dappDetection = (
      provider == PresearchWalletWeb3ProviderTypes::ASK && !has_metamask) ||
      (provider == PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS &&
       !service->IsCryptoWalletsReady());

  return RespondNow(OneArgument(base::Value(dappDetection)));
}

ExtensionFunction::ResponseAction
PresearchWalletGetWalletSeedFunction::Run() {
  // make sure the passed in enryption key is 32 bytes.
  std::unique_ptr<presearch_wallet::GetWalletSeed::Params> params(
    presearch_wallet::GetWalletSeed::Params::Create(*args_));
  if (params->key.size() != 32) {
    return RespondNow(Error("Invalid input key size"));
  }

  auto* service = GetPresearchWalletService(browser_context());

  base::Value::BlobStorage blob;
  std::string derived = service->GetWalletSeed(params->key);

  if (derived.empty()) {
    return RespondNow(Error("Error getting wallet seed"));
  }

  blob.assign(derived.begin(), derived.end());

  return RespondNow(OneArgument(base::Value(blob)));
}

ExtensionFunction::ResponseAction
PresearchWalletGetBitGoSeedFunction::Run() {
  // make sure the passed in enryption key is 32 bytes.
  std::unique_ptr<presearch_wallet::GetBitGoSeed::Params> params(
    presearch_wallet::GetBitGoSeed::Params::Create(*args_));
  if (params->key.size() != 32) {
    return RespondNow(Error("Invalid input key size"));
  }

  auto* service = GetPresearchWalletService(browser_context());

  base::Value::BlobStorage blob;
  std::string derived = service->GetBitGoSeed(params->key);

  if (derived.empty()) {
    return RespondNow(Error("Error getting wallet seed"));
  }

  blob.assign(derived.begin(), derived.end());

  return RespondNow(OneArgument(base::Value(blob)));
}

ExtensionFunction::ResponseAction
PresearchWalletGetProjectIDFunction::Run() {
  std::string project_id = extensions::GetInfuraProjectID();
  return RespondNow(OneArgument(base::Value(project_id)));
}

ExtensionFunction::ResponseAction
PresearchWalletGetPresearchKeyFunction::Run() {
  std::string presearch_key = extensions::GetPresearchKey();
  return RespondNow(OneArgument(base::Value(presearch_key)));
}

ExtensionFunction::ResponseAction
PresearchWalletResetWalletFunction::Run() {
  auto* service = GetPresearchWalletService(browser_context());
  service->ResetCryptoWallets();
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
PresearchWalletGetWeb3ProviderFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto provider = static_cast<PresearchWalletWeb3ProviderTypes>(
      profile->GetPrefs()->GetInteger(kPresearchWalletWeb3Provider));
  std::string extension_id;
  if (provider == PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET) {
    // This API is used so an extension can know when to prompt to
    // be the default Dapp provider. Since the new wallet is not an
    // extension at all, we can just re-use the Crypto Wallets ID.
    // We also don't want to prompt in Crypto Wallets when it's set
    // to Presearch Wallet.
    extension_id = ethereum_remote_client_extension_id;
  } else if (provider == PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS) {
    extension_id = ethereum_remote_client_extension_id;
  } else if (provider == PresearchWalletWeb3ProviderTypes::METAMASK) {
    extension_id = metamask_extension_id;
  }
  return RespondNow(OneArgument(base::Value(extension_id)));
}

ExtensionFunction::ResponseAction
PresearchWalletGetWeb3ProviderListFunction::Run() {
  base::Value list(base::Value::Type::LIST);
  // There is no Ask mode in the new wallet flow, instead it is
  // just defaulted to the new wallet since there is no overhead.
  bool new_wallet = ::presearch_wallet::IsNativeWalletEnabled();
  if (new_wallet) {
    list.Append(MakeSelectValue(
        l10n_util::GetStringUTF16(IDS_PRESEARCH_WALLET_WEB3_PROVIDER_PRESEARCH),
        PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET));
  } else {
    list.Append(MakeSelectValue(
        l10n_util::GetStringUTF16(IDS_PRESEARCH_WALLET_WEB3_PROVIDER_ASK),
        PresearchWalletWeb3ProviderTypes::ASK));
  }
  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(
          new_wallet ? IDS_PRESEARCH_WALLET_WEB3_PROVIDER_CRYPTO_WALLETS_DEPRECATED
                     : IDS_PRESEARCH_WALLET_WEB3_PROVIDER_CRYPTO_WALLETS),
      PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* registry = extensions::ExtensionRegistry::Get(profile);
  if (registry->ready_extensions().Contains(metamask_extension_id)) {
    list.Append(MakeSelectValue(
        l10n_util::GetStringUTF16(IDS_PRESEARCH_WALLET_WEB3_PROVIDER_METAMASK),
        PresearchWalletWeb3ProviderTypes::METAMASK));
  }
  list.Append(MakeSelectValue(
      l10n_util::GetStringUTF16(IDS_PRESEARCH_WALLET_WEB3_PROVIDER_NONE),
      PresearchWalletWeb3ProviderTypes::NONE));
  std::string json_string;
  base::JSONWriter::Write(list, &json_string);
  return RespondNow(OneArgument(base::Value(json_string)));
}

ExtensionFunction::ResponseAction
PresearchWalletIsNativeWalletEnabledFunction::Run() {
  return RespondNow(
      OneArgument(base::Value(::presearch_wallet::IsNativeWalletEnabled())));
}

}  // namespace api
}  // namespace extensions
