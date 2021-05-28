/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_REWARDS_API_H_
#define PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_REWARDS_API_H_

#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "presearch/vendor/bat-native-ledger/include/bat/ledger/mojom_structs.h"
#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class PresearchRewardsOpenBrowserActionUIFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.openBrowserActionUI", UNKNOWN)

 protected:
  ~PresearchRewardsOpenBrowserActionUIFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsUpdateMediaDurationFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.updateMediaDuration", UNKNOWN)

 protected:
  ~PresearchRewardsUpdateMediaDurationFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetPublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getPublisherInfo", UNKNOWN)

 protected:
  ~PresearchRewardsGetPublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherInfo(
      const ledger::type::Result result,
      ledger::type::PublisherInfoPtr info);
};

class PresearchRewardsGetPublisherPanelInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getPublisherPanelInfo", UNKNOWN)

 protected:
  ~PresearchRewardsGetPublisherPanelInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherPanelInfo(
      const ledger::type::Result result,
      ledger::type::PublisherInfoPtr info);
};

class PresearchRewardsSavePublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.savePublisherInfo", UNKNOWN)

 protected:
  ~PresearchRewardsSavePublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnSavePublisherInfo(const ledger::type::Result result);
};

class PresearchRewardsTipSiteFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.tipSite", UNKNOWN)

 protected:
  ~PresearchRewardsTipSiteFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsTipUserFunction : public ExtensionFunction {
 public:
  PresearchRewardsTipUserFunction();
  DECLARE_EXTENSION_FUNCTION("presearchRewards.tipUser", UNKNOWN)

 protected:
  ~PresearchRewardsTipUserFunction() override;

  ResponseAction Run() override;

 private:
  void OnProcessStarted(const std::string& publisher_key);
  void OnTipUserGetPublisherInfo(
      const ledger::type::Result result,
      ledger::type::PublisherInfoPtr info);
  void OnTipUserSavePublisherInfo(const ledger::type::Result result);
  void ShowTipDialog();

  base::WeakPtrFactory<PresearchRewardsTipUserFunction> weak_factory_;
};

class PresearchRewardsGetPublisherDataFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getPublisherData", UNKNOWN)

 protected:
  ~PresearchRewardsGetPublisherDataFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetRewardsParametersFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getRewardsParameters", UNKNOWN)

 protected:
  ~PresearchRewardsGetRewardsParametersFunction() override;

  ResponseAction Run() override;

 private:
  void OnGet(ledger::type::RewardsParametersPtr parameters);
};

class PresearchRewardsGetBalanceReportFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getBalanceReport", UNKNOWN)

 protected:
  ~PresearchRewardsGetBalanceReportFunction() override;

  ResponseAction Run() override;

 private:
  void OnBalanceReport(
      const ledger::type::Result result,
      ledger::type::BalanceReportInfoPtr report);
};

class PresearchRewardsIncludeInAutoContributionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.includeInAutoContribution", UNKNOWN)

 protected:
  ~PresearchRewardsIncludeInAutoContributionFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsFetchPromotionsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.fetchPromotions", UNKNOWN)

 protected:
  ~PresearchRewardsFetchPromotionsFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsClaimPromotionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.claimPromotion", UNKNOWN)

 protected:
  ~PresearchRewardsClaimPromotionFunction() override;

  ResponseAction Run() override;

 private:
  void OnClaimPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      const std::string& captcha_image,
      const std::string& hint,
      const std::string& captcha_id);
};

class PresearchRewardsAttestPromotionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.attestPromotion", UNKNOWN)

 protected:
  ~PresearchRewardsAttestPromotionFunction() override;

  ResponseAction Run() override;

 private:
  void OnAttestPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      ledger::type::PromotionPtr promotion);
};

class PresearchRewardsGetPendingContributionsTotalFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "presearchRewards.getPendingContributionsTotal", UNKNOWN)

 protected:
  ~PresearchRewardsGetPendingContributionsTotalFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPendingTotal(double amount);
};

class PresearchRewardsSaveAdsSettingFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.saveAdsSetting", UNKNOWN)

 protected:
  ~PresearchRewardsSaveAdsSettingFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsSetAutoContributeEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.setAutoContributeEnabled", UNKNOWN)

 protected:
  ~PresearchRewardsSetAutoContributeEnabledFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetACEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getACEnabled", UNKNOWN)

 protected:
  ~PresearchRewardsGetACEnabledFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetACEnabled(bool enabled);
};

class PresearchRewardsSaveRecurringTipFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.saveRecurringTip", UNKNOWN)

 protected:
  ~PresearchRewardsSaveRecurringTipFunction() override;

  ResponseAction Run() override;

 private:
  void OnSaveRecurringTip(bool success);
};

class PresearchRewardsRemoveRecurringTipFunction :
  public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.removeRecurringTip", UNKNOWN)

 protected:
  ~PresearchRewardsRemoveRecurringTipFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetRecurringTipsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getRecurringTips", UNKNOWN)

 protected:
  ~PresearchRewardsGetRecurringTipsFunction() override;

  ResponseAction Run() override;

 private:
    void OnGetRecurringTips(ledger::type::PublisherInfoList list);
};

class PresearchRewardsGetPublisherBannerFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "presearchRewards.getPublisherBanner", UNKNOWN)

 protected:
  ~PresearchRewardsGetPublisherBannerFunction() override;

  ResponseAction Run() override;

 private:
  void OnPublisherBanner(ledger::type::PublisherBannerPtr banner);
};

class PresearchRewardsRefreshPublisherFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.refreshPublisher", UNKNOWN)

 protected:
  ~PresearchRewardsRefreshPublisherFunction() override;

  ResponseAction Run() override;

 private:
  void OnRefreshPublisher(
      const ledger::type::PublisherStatus status,
      const std::string& publisher_key);
};

class PresearchRewardsGetAllNotificationsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getAllNotifications", UNKNOWN)

 protected:
  ~PresearchRewardsGetAllNotificationsFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetInlineTippingPlatformEnabledFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "presearchRewards.getInlineTippingPlatformEnabled",
      UNKNOWN)

 protected:
  ~PresearchRewardsGetInlineTippingPlatformEnabledFunction() override;

  ResponseAction Run() override;

 private:
  void OnInlineTipSetting(bool value);
};

class PresearchRewardsFetchBalanceFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.fetchBalance", UNKNOWN)

 protected:
  ~PresearchRewardsFetchBalanceFunction() override;

  ResponseAction Run() override;

 private:
  void OnBalance(
      const ledger::type::Result result,
      ledger::type::BalancePtr balance);
};

class PresearchRewardsGetExternalWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getExternalWallet", UNKNOWN)

 protected:
  ~PresearchRewardsGetExternalWalletFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetExternalWallet(const ledger::type::Result result,
                           ledger::type::ExternalWalletPtr wallet);
};

class PresearchRewardsDisconnectWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.disconnectWallet", UNKNOWN)

 protected:
  ~PresearchRewardsDisconnectWalletFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsOnlyAnonWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.onlyAnonWallet", UNKNOWN)

 protected:
  ~PresearchRewardsOnlyAnonWalletFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetAdsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getAdsEnabled", UNKNOWN)

 protected:
  ~PresearchRewardsGetAdsEnabledFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetAdsAccountStatementFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getAdsAccountStatement", UNKNOWN)

 protected:
  ~PresearchRewardsGetAdsAccountStatementFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetAdsAccountStatement(const bool success,
                                const double estimated_pending_rewards,
                                const int64_t next_payment_date,
                                const int ads_received_this_month,
                                const double earnings_this_month,
                                const double earnings_last_month);
};

class PresearchRewardsGetAdsSupportedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getAdsSupported", UNKNOWN)

 protected:
  ~PresearchRewardsGetAdsSupportedFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetAnonWalletStatusFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getAnonWalletStatus", UNKNOWN)

 protected:
  ~PresearchRewardsGetAnonWalletStatusFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetAnonWalletStatus(const ledger::type::Result result);
};

class PresearchRewardsIsInitializedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.isInitialized", UNKNOWN)

 protected:
  ~PresearchRewardsIsInitializedFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsShouldShowOnboardingFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.shouldShowOnboarding", UNKNOWN)

 protected:
  ~PresearchRewardsShouldShowOnboardingFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsEnableRewardsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.enableRewards", UNKNOWN)

 protected:
  ~PresearchRewardsEnableRewardsFunction() override;

  ResponseAction Run() override;
};

class PresearchRewardsGetPrefsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.getPrefs", UNKNOWN)

 protected:
  ~PresearchRewardsGetPrefsFunction() override;

  ResponseAction Run() override;

 private:
  void GetAutoContributePropertiesCallback(
      ledger::type::AutoContributePropertiesPtr properties);
};

class PresearchRewardsUpdatePrefsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("presearchRewards.updatePrefs", UNKNOWN)

 protected:
  ~PresearchRewardsUpdatePrefsFunction() override;

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // PRESEARCH_BROWSER_EXTENSIONS_API_PRESEARCH_REWARDS_API_H_
