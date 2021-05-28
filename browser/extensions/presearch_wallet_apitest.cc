/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "presearch/common/presearch_paths.h"
#include "presearch/common/pref_names.h"
#include "presearch/components/presearch_wallet/browser/presearch_wallet_constants.h"
#include "presearch/components/presearch_wallet/browser/pref_names.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/common/constants.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {

class PresearchWalletExtensionApiTest : public ExtensionApiTest {
 public:
  void SetUp() override {
    presearch::RegisterPathProvider();
    base::PathService::Get(presearch::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }
  void TearDown() override {
    ExtensionApiTest::TearDown();
  }
  PrefService* GetPrefs() { return browser()->profile()->GetPrefs(); }
  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchExtensionWithWalletHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id,
      "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletAPIAvailable) {
  std::unique_ptr<base::Environment> env(base::Environment::Create());
  env->SetVar("PRESEARCH_INFURA_PROJECT_ID", "test-project-id");
  env->SetVar("PRESEARCH_SERVICES_KEY", "test-presearch-key");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletAPIKnownValuesTest) {
  GetPrefs()->SetString(kPresearchWalletAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(kPresearchWalletEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testKnownSeedValuesEndToEnd()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletAPIBitGoKnownValuesTest) {
  GetPrefs()->SetString(kPresearchWalletAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(kPresearchWalletEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testKnownBitGoSeedValuesEndToEnd()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletWeb3ProviderCryptoWallets) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsCryptoWallets()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletWeb3ProviderMetaMask) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::METAMASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsMetaMask()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletWeb3ProviderAsk) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::ASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsAsk()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletWeb3ProviderNone) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::NONE));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsNone()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
                       PresearchWalletWeb3ProviderPresearchWallet) {
  GetPrefs()->SetInteger(
      kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET));
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsPresearchWallet()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchWalletAPINotAvailable) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("notPresearchWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchShieldsDappDetectionWhenDefault) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchShieldsDappDetectionWhenAsk) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::ASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchShieldsNoDappDetectionWhenNone) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::NONE));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchShieldsNoDappDetectionWhenMetaMask) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::METAMASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchShieldsNoDappDetectionWhenCryptoWallets) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
                       PresearchShieldsNoDappDetectionWhenPresearchWallet) {
  GetPrefs()->SetInteger(
      kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::PRESEARCH_WALLET));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  LoadExtension(extension_dir_.AppendASCII("presearchWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(PresearchWalletExtensionApiTest,
    PresearchShieldsDappDetectionWhenCryptoWalletsNotReady) {
  GetPrefs()->SetInteger(kPresearchWalletWeb3Provider,
      static_cast<int>(PresearchWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("presearchShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), presearch_extension_id, "testDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

}  // namespace
}  // namespace extensions
