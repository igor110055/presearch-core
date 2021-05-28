/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_WALLET_API_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_WALLET_API_H_

#include <string>

#include "extensions/browser/extension_function.h"

class Profile;

namespace extensions {
namespace api {

class PresearchWalletPromptToEnableWalletFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.promptToEnableWallet", UNKNOWN)

 protected:
  ~PresearchWalletPromptToEnableWalletFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletReadyFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.ready", UNKNOWN)

 protected:
  ~PresearchWalletReadyFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletShouldCheckForDappsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.shouldCheckForDapps", UNKNOWN)

 protected:
  ~PresearchWalletShouldCheckForDappsFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletLoadUIFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.loadUI", UNKNOWN)
  void OnLoaded();

 protected:
  ~PresearchWalletLoadUIFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletShouldPromptForSetupFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.shouldPromptForSetup", UNKNOWN)

 protected:
  ~PresearchWalletShouldPromptForSetupFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletGetWalletSeedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.getWalletSeed", UNKNOWN)

 protected:
  ~PresearchWalletGetWalletSeedFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletGetBitGoSeedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.getBitGoSeed", UNKNOWN)

 protected:
  ~PresearchWalletGetBitGoSeedFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletGetProjectIDFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.getProjectID", UNKNOWN)

 protected:
  ~PresearchWalletGetProjectIDFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletGetPresearchKeyFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.getPresearchKey", UNKNOWN)

 protected:
  ~PresearchWalletGetPresearchKeyFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletResetWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.resetWallet", UNKNOWN)

 protected:
  ~PresearchWalletResetWalletFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletGetWeb3ProviderFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.getWeb3Provider", UNKNOWN)

 protected:
  ~PresearchWalletGetWeb3ProviderFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletGetWeb3ProviderListFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.getWeb3ProviderList", UNKNOWN)

 protected:
  ~PresearchWalletGetWeb3ProviderListFunction() override {}
  ResponseAction Run() override;
};

class PresearchWalletIsNativeWalletEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchWallet.isNativeWalletEnabled", UNKNOWN)

 protected:
  ~PresearchWalletIsNativeWalletEnabledFunction() override {}
  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_WALLET_API_H_
