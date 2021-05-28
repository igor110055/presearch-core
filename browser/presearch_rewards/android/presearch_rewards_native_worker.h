/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_BROWSER_PRESEARCH_REWARDS_ANDROID_PRESEARCH_REWARDS_NATIVE_WORKER_H_
#define PRESEARCH_BROWSER_PRESEARCH_REWARDS_ANDROID_PRESEARCH_REWARDS_NATIVE_WORKER_H_

#include <jni.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/android/jni_weak_ref.h"
#include "base/containers/flat_map.h"
#include "base/memory/weak_ptr.h"
#include "bat/ledger/mojom_structs.h"
#include "presearch/components/presearch_rewards/browser/rewards_service_observer.h"
#include "presearch/components/presearch_rewards/browser/rewards_notification_service_observer.h"
#include "presearch/components/presearch_rewards/browser/rewards_service_private_observer.h"

namespace presearch_rewards {
class RewardsService;
}

namespace chrome {
namespace android {

typedef std::map<uint64_t, ledger::type::PublisherInfoPtr> PublishersInfoMap;

class PresearchRewardsNativeWorker : public presearch_rewards::RewardsServiceObserver,
    public presearch_rewards::RewardsServicePrivateObserver,
    public presearch_rewards::RewardsNotificationServiceObserver {
 public:
    PresearchRewardsNativeWorker(JNIEnv* env,
        const base::android::JavaRef<jobject>& obj);
    ~PresearchRewardsNativeWorker() override;

    void Destroy(JNIEnv* env);

    void GetRewardsParameters(JNIEnv* env);

    void GetPublisherInfo(JNIEnv* env,
                          int tabId,
                          const base::android::JavaParamRef<jstring>& host);

    base::android::ScopedJavaLocalRef<jstring> GetWalletBalance(JNIEnv* env);

    double GetWalletRate(JNIEnv* env);

    base::android::ScopedJavaLocalRef<jstring> GetPublisherURL(JNIEnv* env,
                                                               uint64_t tabId);

    base::android::ScopedJavaLocalRef<jstring> GetPublisherFavIconURL(
        JNIEnv* env,
        uint64_t tabId);

    base::android::ScopedJavaLocalRef<jstring> GetPublisherName(JNIEnv* env,
                                                                uint64_t tabId);

    base::android::ScopedJavaLocalRef<jstring> GetPublisherId(JNIEnv* env,
        uint64_t tabId);

    int GetPublisherPercent(JNIEnv* env, uint64_t tabId);

    bool GetPublisherExcluded(JNIEnv* env, uint64_t tabId);

    int GetPublisherStatus(JNIEnv* env, uint64_t tabId);

    void GetCurrentBalanceReport(JNIEnv* env);

    void IncludeInAutoContribution(JNIEnv* env, uint64_t tabId, bool exclude);

    void RemovePublisherFromMap(JNIEnv* env, uint64_t tabId);

    void Donate(JNIEnv* env,
                const base::android::JavaParamRef<jstring>& publisher_key,
                int amount,
                bool recurring);

    void GetAllNotifications(JNIEnv* env);

    void DeleteNotification(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& notification_id);

    void GetGrant(JNIEnv* env,
                  const base::android::JavaParamRef<jstring>& promotionId);

    base::android::ScopedJavaLocalRef<jobjectArray> GetCurrentGrant(
        JNIEnv* env,
        int position);

    void GetPendingContributionsTotal(JNIEnv* env);

    void GetRecurringDonations(JNIEnv* env);

    bool IsCurrentPublisherInRecurrentDonations(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& publisher);

    void GetAutoContributeProperties(JNIEnv* env);

    bool IsAutoContributeEnabled(JNIEnv* env);

    void GetReconcileStamp(JNIEnv* env);

    void ResetTheWholeState(JNIEnv* env);

    double GetPublisherRecurrentDonationAmount(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& publisher);

    void RemoveRecurring(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& publisher);

    void FetchGrants(JNIEnv* env);

    int GetAdsPerHour(JNIEnv* env);

    void SetAdsPerHour(JNIEnv* env, jint value);

    void SetAutoContributionAmount(JNIEnv* env, jdouble value);

    void GetExternalWallet(JNIEnv* env);

    void DisconnectWallet(JNIEnv* env);

    void ProcessRewardsPageUrl(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& path,
        const base::android::JavaParamRef<jstring>& query);

    void RecoverWallet(JNIEnv* env,
                       const base::android::JavaParamRef<jstring>& pass_phrase);

    void RefreshPublisher(
        JNIEnv* env,
        const base::android::JavaParamRef<jstring>& publisher_key);

    void OnResetTheWholeState(const bool success);

    void OnGetGetReconcileStamp(uint64_t timestamp);

    void OnGetAutoContributeProperties(
        ledger::type::AutoContributePropertiesPtr properties);

    void OnGetPendingContributionsTotal(double amount);

    void OnPanelPublisherInfo(
        presearch_rewards::RewardsService* rewards_service,
        const ledger::type::Result result,
        const ledger::type::PublisherInfo* info,
        uint64_t tabId) override;

    void OnGetCurrentBalanceReport(
        presearch_rewards::RewardsService* rewards_service,
        const ledger::type::Result result,
        ledger::type::BalanceReportInfoPtr report);

    void OnGetRewardsParameters(
        presearch_rewards::RewardsService* rewards_service,
        ledger::type::RewardsParametersPtr parameters);

    void OnNotificationAdded(
      presearch_rewards::RewardsNotificationService* rewards_notification_service,
      const presearch_rewards::RewardsNotificationService::RewardsNotification&
        notification) override;

    void OnGetAllNotifications(
      presearch_rewards::RewardsNotificationService* rewards_notification_service,
      const presearch_rewards::RewardsNotificationService::RewardsNotificationsList&
          notifications_list) override;

    void OnNotificationDeleted(
      presearch_rewards::RewardsNotificationService* rewards_notification_service,
      const presearch_rewards::RewardsNotificationService::RewardsNotification&
        notification) override;

    void OnPromotionFinished(
        presearch_rewards::RewardsService* rewards_service,
        const ledger::type::Result result,
        ledger::type::PromotionPtr promotion) override;

    void OnGetRecurringTips(ledger::type::PublisherInfoList list);

    bool IsAnonWallet(JNIEnv* env);

    bool IsRewardsEnabled(JNIEnv* env);

    void OnClaimPromotion(
        const ledger::type::Result result,
        ledger::type::PromotionPtr promotion);

    void OnGetExternalWallet(const ledger::type::Result result,
                             ledger::type::ExternalWalletPtr wallet);

    void OnDisconnectWallet(
      presearch_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const std::string& wallet_type) override;

    void OnProcessRewardsPageUrl(
        const ledger::type::Result result,
        const std::string& wallet_type,
        const std::string& action,
        const base::flat_map<std::string, std::string>& args);

    void OnRecoverWallet(
        presearch_rewards::RewardsService* rewards_service,
        const ledger::type::Result result) override;

    void OnRefreshPublisher(
        const ledger::type::PublisherStatus status,
        const std::string& publisher_key);
    void SetAutoContributeEnabled(
        JNIEnv* env,
        bool isAutoContributeEnabled);
    void StartProcess(JNIEnv* env);

 private:
    std::string StdStrStrMapToJsonString(
        const base::flat_map<std::string, std::string>& args);

    void OnBalance(
        const ledger::type::Result result,
        ledger::type::BalancePtr balance);

    void OnStartProcess();

    JavaObjectWeakGlobalRef weak_java_presearch_rewards_native_worker_;
    presearch_rewards::RewardsService* presearch_rewards_service_;
    ledger::type::RewardsParameters parameters_;
    ledger::type::Balance balance_;
    ledger::type::AutoContributePropertiesPtr auto_contrib_properties_;
    PublishersInfoMap map_publishers_info_;
    std::map<std::string, ledger::type::PublisherInfoPtr>
      map_recurrent_publishers_;
    std::map<std::string, std::string> addresses_;
    ledger::type::PromotionList promotions_;
    base::WeakPtrFactory<PresearchRewardsNativeWorker> weak_factory_;
};
}  // namespace android
}  // namespace chrome

#endif  // PRESEARCH_BROWSER_PRESEARCH_REWARDS_ANDROID_PRESEARCH_REWARDS_NATIVE_WORKER_H_
