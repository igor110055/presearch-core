/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_SHIELDS_API_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_SHIELDS_API_H_

#include <memory>
#include <string>
#include <vector>

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class PresearchShieldsUrlCosmeticResourcesFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.urlCosmeticResources", UNKNOWN)

 protected:
  ~PresearchShieldsUrlCosmeticResourcesFunction() override {}

  ResponseAction Run() override;

 private:
  std::unique_ptr<base::ListValue> GetUrlCosmeticResourcesOnTaskRunner(
      const std::string& url);
  void GetUrlCosmeticResourcesOnUI(std::unique_ptr<base::ListValue> resources);
};

class PresearchShieldsHiddenClassIdSelectorsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.hiddenClassIdSelectors", UNKNOWN)

 protected:
  ~PresearchShieldsHiddenClassIdSelectorsFunction() override {}

  ResponseAction Run() override;

 private:
  std::unique_ptr<base::ListValue> GetHiddenClassIdSelectorsOnTaskRunner(
      const std::vector<std::string>& classes,
      const std::vector<std::string>& ids,
      const std::vector<std::string>& exceptions);
  void GetHiddenClassIdSelectorsOnUI(
      std::unique_ptr<base::ListValue> selectors);
};

class PresearchShieldsMigrateLegacyCosmeticFiltersFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.migrateLegacyCosmeticFilters",
                             UNKNOWN)

 protected:
  ~PresearchShieldsMigrateLegacyCosmeticFiltersFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsAddSiteCosmeticFilterFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.addSiteCosmeticFilter", UNKNOWN)

 protected:
  ~PresearchShieldsAddSiteCosmeticFilterFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsOpenFilterManagementPageFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.openFilterManagementPage", UNKNOWN)

 protected:
  ~PresearchShieldsOpenFilterManagementPageFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsAllowScriptsOnceFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.allowScriptsOnce", UNKNOWN)

 protected:
  ~PresearchShieldsAllowScriptsOnceFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsOpenBrowserActionUIFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.openBrowserActionUI", UNKNOWN)

 protected:
  ~PresearchShieldsOpenBrowserActionUIFunction() override;

  ResponseAction Run() override;
};

class PresearchShieldsSetPresearchShieldsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setPresearchShieldsEnabled", UNKNOWN)

 protected:
  ~PresearchShieldsSetPresearchShieldsEnabledFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsGetPresearchShieldsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.getPresearchShieldsEnabled", UNKNOWN)

 protected:
  ~PresearchShieldsGetPresearchShieldsEnabledFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsShouldDoCosmeticFilteringFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.shouldDoCosmeticFiltering", UNKNOWN)

 protected:
  ~PresearchShieldsShouldDoCosmeticFilteringFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsSetCosmeticFilteringControlTypeFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setCosmeticFilteringControlType",
                             UNKNOWN)

 protected:
  ~PresearchShieldsSetCosmeticFilteringControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsIsFirstPartyCosmeticFilteringEnabledFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "presearchShields.isFirstPartyCosmeticFilteringEnabled",
      UNKNOWN)

 protected:
  ~PresearchShieldsIsFirstPartyCosmeticFilteringEnabledFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsSetAdControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setAdControlType", UNKNOWN)

 protected:
  ~PresearchShieldsSetAdControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsGetAdControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.getAdControlType", UNKNOWN)

 protected:
  ~PresearchShieldsGetAdControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsSetCookieControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setCookieControlType", UNKNOWN)

 protected:
  ~PresearchShieldsSetCookieControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsGetCookieControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.getCookieControlType", UNKNOWN)

 protected:
  ~PresearchShieldsGetCookieControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsSetFingerprintingControlTypeFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setFingerprintingControlType",
                             UNKNOWN)

 protected:
  ~PresearchShieldsSetFingerprintingControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsGetFingerprintingControlTypeFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.getFingerprintingControlType",
                             UNKNOWN)

 protected:
  ~PresearchShieldsGetFingerprintingControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsSetHTTPSEverywhereEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setHTTPSEverywhereEnabled",
                             UNKNOWN)

 protected:
  ~PresearchShieldsSetHTTPSEverywhereEnabledFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsGetHTTPSEverywhereEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.getHTTPSEverywhereEnabled",
                             UNKNOWN)

 protected:
  ~PresearchShieldsGetHTTPSEverywhereEnabledFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsSetNoScriptControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.setNoScriptControlType", UNKNOWN)

 protected:
  ~PresearchShieldsSetNoScriptControlTypeFunction() override {}

  ResponseAction Run() override;
};

class PresearchShieldsGetNoScriptControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.getNoScriptControlType", UNKNOWN)

 protected:
  ~PresearchShieldsGetNoScriptControlTypeFunction() override {}

  ResponseAction Run() override;
};

// Notifies the browser that the shields panel was shown to the user.
class PresearchShieldsOnShieldsPanelShownFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.onShieldsPanelShown", UNKNOWN)

 protected:
  ~PresearchShieldsOnShieldsPanelShownFunction() override {}

  // ExtensionFunction:
  ResponseAction Run() override;
};

class PresearchShieldsReportBrokenSiteFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchShields.reportBrokenSite", UNKNOWN)

 protected:
  ~PresearchShieldsReportBrokenSiteFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_SHIELDS_API_H_
