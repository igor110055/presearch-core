/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/presearch_rewards/android/presearch_rewards_native_worker.h"

#include <iomanip>
#include <string>
#include <vector>
#include <utility>

#include "base/android/jni_android.h"
#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
#include "base/containers/flat_map.h"
#include "base/json/json_writer.h"
#include "base/time/time.h"
#include "presearch/browser/presearch_ads/ads_service_factory.h"
#include "presearch/browser/presearch_rewards/rewards_service_factory.h"
#include "presearch/build/android/jni_headers/PresearchRewardsNativeWorker_jni.h"
#include "presearch/components/presearch_ads/browser/ads_service.h"
#include "presearch/components/presearch_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "content/public/browser/url_data_source.h"

#define DEFAULT_ADS_PER_HOUR 2
#define DEFAULT_AUTO_CONTRIBUTION_AMOUNT 10

namespace chrome {
namespace android {

PresearchRewardsNativeWorker::PresearchRewardsNativeWorker(JNIEnv* env,
    const base::android::JavaRef<jobject>& obj):
    weak_java_presearch_rewards_native_worker_(env, obj),
    presearch_rewards_service_(nullptr),
    weak_factory_(this) {
  Java_PresearchRewardsNativeWorker_setNativePtr(env, obj,
    reinterpret_cast<intptr_t>(this));

  presearch_rewards_service_ = presearch_rewards::RewardsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (presearch_rewards_service_) {
    presearch_rewards_service_->AddObserver(this);
    presearch_rewards_service_->AddPrivateObserver(this);
    presearch_rewards::RewardsNotificationService* notification_service =
      presearch_rewards_service_->GetNotificationService();
    if (notification_service) {
      notification_service->AddObserver(this);
    }
  }
}

PresearchRewardsNativeWorker::~PresearchRewardsNativeWorker() {
}

void PresearchRewardsNativeWorker::Destroy(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->RemoveObserver(this);
    presearch_rewards_service_->RemovePrivateObserver(this);
    presearch_rewards::RewardsNotificationService* notification_service =
      presearch_rewards_service_->GetNotificationService();
    if (notification_service) {
      notification_service->RemoveObserver(this);
    }
  }
  delete this;
}

void PresearchRewardsNativeWorker::GetRewardsParameters(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetRewardsParameters(
        base::BindOnce(&PresearchRewardsNativeWorker::OnGetRewardsParameters,
                       weak_factory_.GetWeakPtr(), presearch_rewards_service_));
  }
}

void PresearchRewardsNativeWorker::GetPublisherInfo(
    JNIEnv* env,
    int tabId,
    const base::android::JavaParamRef<jstring>& host) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetPublisherActivityFromUrl(tabId,
      base::android::ConvertJavaStringToUTF8(env, host), "", "");
  }
}

void PresearchRewardsNativeWorker::OnPanelPublisherInfo(
      presearch_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const ledger::type::PublisherInfo* info,
      uint64_t tabId) {
  if (!info) {
    return;
  }
  ledger::type::PublisherInfoPtr pi = info->Clone();
  map_publishers_info_[tabId] = std::move(pi);
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnPublisherInfo(env,
        weak_java_presearch_rewards_native_worker_.get(env), tabId);
}

base::android::ScopedJavaLocalRef<jstring>
PresearchRewardsNativeWorker::GetPublisherURL(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->url);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
PresearchRewardsNativeWorker::GetPublisherFavIconURL(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env,
      iter->second->favicon_url);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
PresearchRewardsNativeWorker::GetPublisherName(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->name);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
PresearchRewardsNativeWorker::GetPublisherId(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->id);
  }

  return res;
}

int PresearchRewardsNativeWorker::GetPublisherPercent(JNIEnv* env, uint64_t tabId) {
  int res = 0;

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = iter->second->percent;
  }

  return res;
}

bool PresearchRewardsNativeWorker::GetPublisherExcluded(JNIEnv* env,
                                                    uint64_t tabId) {
  bool res = false;

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = iter->second->excluded == ledger::type::PublisherExclude::EXCLUDED;
  }

  return res;
}

int PresearchRewardsNativeWorker::GetPublisherStatus(JNIEnv* env, uint64_t tabId) {
  int res = static_cast<int>(ledger::type::PublisherStatus::NOT_VERIFIED);
  PublishersInfoMap::const_iterator iter = map_publishers_info_.find(tabId);
  if (iter != map_publishers_info_.end()) {
    res = static_cast<int>(iter->second->status);
  }
  return res;
}

void PresearchRewardsNativeWorker::IncludeInAutoContribution(JNIEnv* env,
                                                         uint64_t tabId,
                                                         bool exclude) {
  PublishersInfoMap::iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    if (exclude) {
      iter->second->excluded = ledger::type::PublisherExclude::EXCLUDED;
    } else {
      iter->second->excluded = ledger::type::PublisherExclude::INCLUDED;
    }
    if (presearch_rewards_service_) {
      presearch_rewards_service_->SetPublisherExclude(iter->second->id, exclude);
    }
  }
}

void PresearchRewardsNativeWorker::RemovePublisherFromMap(JNIEnv* env,
                                                      uint64_t tabId) {
  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    map_publishers_info_.erase(iter);
  }
}

void PresearchRewardsNativeWorker::OnGetRewardsParameters(
    presearch_rewards::RewardsService* rewards_service,
    ledger::type::RewardsParametersPtr parameters) {
  if (parameters) {
    parameters_ = *parameters;
  }

  if (rewards_service) {
    rewards_service->FetchBalance(
      base::Bind(
        &PresearchRewardsNativeWorker::OnBalance,
        weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::OnBalance(
    const ledger::type::Result result,
    ledger::type::BalancePtr balance) {
  if (result == ledger::type::Result::LEDGER_OK && balance) {
    balance_ = *balance;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnRewardsParameters(
      env, weak_java_presearch_rewards_native_worker_.get(env), 0);
}

base::android::ScopedJavaLocalRef<jstring>
PresearchRewardsNativeWorker::GetWalletBalance(JNIEnv* env) {
  std::string json_balance;
  base::DictionaryValue json_root;
  json_root.SetDoubleKey("total", balance_.total);

  auto json_wallets = std::make_unique<base::DictionaryValue>();
  for (const auto & item : balance_.wallets) {
    json_wallets->SetDoubleKey(item.first, item.second);
  }
  json_root.SetDictionary("wallets", std::move(json_wallets));
  base::JSONWriter::Write(json_root, &json_balance);

  return base::android::ConvertUTF8ToJavaString(env, json_balance);
}

double PresearchRewardsNativeWorker::GetWalletRate(JNIEnv* env) {
  return parameters_.rate;
}

void PresearchRewardsNativeWorker::FetchGrants(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->FetchPromotions();
  }
}

void PresearchRewardsNativeWorker::StartProcess(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->StartProcess(base::Bind(
          &PresearchRewardsNativeWorker::OnStartProcess,
          weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::OnStartProcess() {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnStartProcess(
      env, weak_java_presearch_rewards_native_worker_.get(env));
}

void PresearchRewardsNativeWorker::GetCurrentBalanceReport(JNIEnv* env) {
  if (presearch_rewards_service_) {
    auto now = base::Time::Now();
    base::Time::Exploded exploded;
    now.LocalExplode(&exploded);

    presearch_rewards_service_->GetBalanceReport(
        exploded.month, exploded.year,
        base::BindOnce(&PresearchRewardsNativeWorker::OnGetCurrentBalanceReport,
                       weak_factory_.GetWeakPtr(), presearch_rewards_service_));
  }
}

void PresearchRewardsNativeWorker::OnGetCurrentBalanceReport(
        presearch_rewards::RewardsService* rewards_service,
        const ledger::type::Result result,
        ledger::type::BalanceReportInfoPtr report) {
  base::android::ScopedJavaLocalRef<jdoubleArray> java_array;
  JNIEnv* env = base::android::AttachCurrentThread();
  if (report) {
    std::vector<double> values;
    values.push_back(report->grants);
    values.push_back(report->earning_from_ads);
    values.push_back(report->auto_contribute);
    values.push_back(report->recurring_donation);
    values.push_back(report->one_time_donation);
    java_array = base::android::ToJavaDoubleArray(env, values);
  }
  Java_PresearchRewardsNativeWorker_OnGetCurrentBalanceReport(env,
        weak_java_presearch_rewards_native_worker_.get(env), java_array);
}

void PresearchRewardsNativeWorker::Donate(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& publisher_key,
        int amount, bool recurring) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->OnTip(
      base::android::ConvertJavaStringToUTF8(env, publisher_key), amount,
        recurring);
    if (!recurring) {
      Java_PresearchRewardsNativeWorker_OnOneTimeTip(env,
        weak_java_presearch_rewards_native_worker_.get(env));
    }
  }
}

void PresearchRewardsNativeWorker::GetAllNotifications(JNIEnv* env) {
  if (!presearch_rewards_service_) {
    return;
  }
  presearch_rewards::RewardsNotificationService* notification_service =
    presearch_rewards_service_->GetNotificationService();
  if (notification_service) {
    notification_service->GetNotifications();
  }
}

void PresearchRewardsNativeWorker::DeleteNotification(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& notification_id) {
  if (!presearch_rewards_service_) {
    return;
  }
  presearch_rewards::RewardsNotificationService* notification_service =
    presearch_rewards_service_->GetNotificationService();
  if (notification_service) {
    notification_service->DeleteNotification(
      base::android::ConvertJavaStringToUTF8(env, notification_id));
  }
}

void PresearchRewardsNativeWorker::GetGrant(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& promotionId) {
  if (presearch_rewards_service_) {
    std::string promotion_id =
      base::android::ConvertJavaStringToUTF8(env, promotionId);
    presearch_rewards_service_->ClaimPromotion(
        promotion_id,
        base::BindOnce(&PresearchRewardsNativeWorker::OnClaimPromotion,
                       weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::OnClaimPromotion(
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnClaimPromotion(env,
      weak_java_presearch_rewards_native_worker_.get(env),
      static_cast<int>(result));
}

base::android::ScopedJavaLocalRef<jobjectArray>
    PresearchRewardsNativeWorker::GetCurrentGrant(JNIEnv* env,
      int position) {
  if ((size_t)position > promotions_.size() - 1) {
    return base::android::ScopedJavaLocalRef<jobjectArray>();
  }
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2) <<
      (promotions_[position])->approximate_value;
  std::vector<std::string> values;
  values.push_back(stream.str());
  values.push_back(
    std::to_string((promotions_[position])->expires_at));
  values.push_back(
      std::to_string(static_cast<int>((promotions_[position])->type)));

  return base::android::ToJavaArrayOfStrings(env, values);
}

void PresearchRewardsNativeWorker::GetPendingContributionsTotal(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetPendingContributionsTotal(base::Bind(
          &PresearchRewardsNativeWorker::OnGetPendingContributionsTotal,
          weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::GetRecurringDonations(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetRecurringTips(base::Bind(
          &PresearchRewardsNativeWorker::OnGetRecurringTips,
          weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::OnGetRecurringTips(
    ledger::type::PublisherInfoList list) {
  map_recurrent_publishers_.clear();
  for (const auto& item : list) {
    map_recurrent_publishers_[item->id] = item->Clone();
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnRecurringDonationUpdated(env,
        weak_java_presearch_rewards_native_worker_.get(env));
}

bool PresearchRewardsNativeWorker::IsCurrentPublisherInRecurrentDonations(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  return map_recurrent_publishers_.find(
    base::android::ConvertJavaStringToUTF8(env, publisher)) !=
      map_recurrent_publishers_.end();
}

void PresearchRewardsNativeWorker::GetAutoContributeProperties(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetAutoContributeProperties(
        base::Bind(&PresearchRewardsNativeWorker::OnGetAutoContributeProperties,
                   weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::OnGetAutoContributeProperties(
    ledger::type::AutoContributePropertiesPtr properties) {
  if (properties) {
    auto_contrib_properties_ = std::move(properties);
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnGetAutoContributeProperties(
      env, weak_java_presearch_rewards_native_worker_.get(env));
}

bool PresearchRewardsNativeWorker::IsAutoContributeEnabled(JNIEnv* env) {
  if (!auto_contrib_properties_) {
    return false;
  }

  return auto_contrib_properties_->enabled_contribute;
}

void PresearchRewardsNativeWorker::GetReconcileStamp(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetReconcileStamp(base::Bind(
            &PresearchRewardsNativeWorker::OnGetGetReconcileStamp,
            weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::ResetTheWholeState(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->CompleteReset(base::Bind(
           &PresearchRewardsNativeWorker::OnResetTheWholeState,
           weak_factory_.GetWeakPtr()));
  } else {
    JNIEnv* env = base::android::AttachCurrentThread();

    Java_PresearchRewardsNativeWorker_OnResetTheWholeState(env,
            weak_java_presearch_rewards_native_worker_.get(env), false);
  }
}

void PresearchRewardsNativeWorker::OnResetTheWholeState(const bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_PresearchRewardsNativeWorker_OnResetTheWholeState(env,
          weak_java_presearch_rewards_native_worker_.get(env), success);
}

double PresearchRewardsNativeWorker::GetPublisherRecurrentDonationAmount(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  double amount(0.0);
  auto it = map_recurrent_publishers_.find(
    base::android::ConvertJavaStringToUTF8(env, publisher));
  if (it != map_recurrent_publishers_.end()) {
    // for Recurrent Donations, the amount is stored in ContentSite::percentage
    amount = it->second->percent;
  }
  return  amount;
}

void PresearchRewardsNativeWorker::RemoveRecurring(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  if (presearch_rewards_service_) {
      presearch_rewards_service_->RemoveRecurringTip(
        base::android::ConvertJavaStringToUTF8(env, publisher));
  }
}

void PresearchRewardsNativeWorker::OnGetGetReconcileStamp(uint64_t timestamp) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_PresearchRewardsNativeWorker_OnGetReconcileStamp(env,
          weak_java_presearch_rewards_native_worker_.get(env), timestamp);
}

void PresearchRewardsNativeWorker::OnGetPendingContributionsTotal(double amount) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_PresearchRewardsNativeWorker_OnGetPendingContributionsTotal(env,
        weak_java_presearch_rewards_native_worker_.get(env), amount);
}

void PresearchRewardsNativeWorker::OnNotificationAdded(
    presearch_rewards::RewardsNotificationService* rewards_notification_service,
    const presearch_rewards::RewardsNotificationService::RewardsNotification&
      notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_PresearchRewardsNativeWorker_OnNotificationAdded(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, notification.id_),
        notification.type_,
        notification.timestamp_,
        base::android::ToJavaArrayOfStrings(env, notification.args_));
}

void PresearchRewardsNativeWorker::OnGetAllNotifications(
    presearch_rewards::RewardsNotificationService* rewards_notification_service,
    const presearch_rewards::RewardsNotificationService::RewardsNotificationsList&
      notifications_list) {
  JNIEnv* env = base::android::AttachCurrentThread();

  // Notify about notifications count
  Java_PresearchRewardsNativeWorker_OnNotificationsCount(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        notifications_list.size());

  presearch_rewards::RewardsNotificationService::RewardsNotificationsList::
    const_iterator iter =
      std::max_element(notifications_list.begin(), notifications_list.end(),
        [](const presearch_rewards::RewardsNotificationService::
            RewardsNotification& notification_a,
          const presearch_rewards::RewardsNotificationService::
            RewardsNotification& notification_b) {
        return notification_a.timestamp_ > notification_b.timestamp_;
      });

  if (iter != notifications_list.end()) {
    Java_PresearchRewardsNativeWorker_OnGetLatestNotification(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, iter->id_),
        iter->type_,
        iter->timestamp_,
        base::android::ToJavaArrayOfStrings(env, iter->args_));
  }
}

void PresearchRewardsNativeWorker::OnNotificationDeleted(
      presearch_rewards::RewardsNotificationService* rewards_notification_service,
      const presearch_rewards::RewardsNotificationService::RewardsNotification&
        notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_PresearchRewardsNativeWorker_OnNotificationDeleted(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, notification.id_));
}

void PresearchRewardsNativeWorker::OnPromotionFinished(
    presearch_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_PresearchRewardsNativeWorker_OnGrantFinish(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        static_cast<int>(result));
}

int PresearchRewardsNativeWorker::GetAdsPerHour(JNIEnv* env) {
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (!ads_service_) {
    return DEFAULT_ADS_PER_HOUR;
  }
  return ads_service_->GetAdsPerHour();
}

void PresearchRewardsNativeWorker::SetAdsPerHour(JNIEnv* env, jint value) {
  auto* ads_service_ = presearch_ads::AdsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (!ads_service_) {
    return;
  }
  ads_service_->SetAdsPerHour(value);
}

void PresearchRewardsNativeWorker::SetAutoContributionAmount(JNIEnv* env,
                                                         jdouble value) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->SetAutoContributionAmount(value);
  }
}

bool PresearchRewardsNativeWorker::IsAnonWallet(JNIEnv* env) {
  if (presearch_rewards_service_) {
    return presearch_rewards_service_->OnlyAnonWallet();
  }
  return false;
}

bool PresearchRewardsNativeWorker::IsRewardsEnabled(JNIEnv* env) {
  if (presearch_rewards_service_) {
    return presearch_rewards_service_->IsRewardsEnabled();
  }
  return false;
}

void PresearchRewardsNativeWorker::GetExternalWallet(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->GetExternalWallet(
        base::BindOnce(&PresearchRewardsNativeWorker::OnGetExternalWallet,
                       weak_factory_.GetWeakPtr()));
  }
}

void PresearchRewardsNativeWorker::OnGetExternalWallet(
    const ledger::type::Result result,
    ledger::type::ExternalWalletPtr wallet) {
  std::string json_wallet;
  if (!wallet) {
    json_wallet = "";
  } else {
    base::Value dict(base::Value::Type::DICTIONARY);
    dict.SetStringKey("token", wallet->token);
    dict.SetStringKey("address", wallet->address);

    // enum class WalletStatus : int32_t
    dict.SetIntKey("status", static_cast<int32_t>(wallet->status));
    dict.SetStringKey("verify_url", wallet->verify_url);
    dict.SetStringKey("add_url", wallet->add_url);
    dict.SetStringKey("type", wallet->type);
    dict.SetStringKey("withdraw_url", wallet->withdraw_url);
    dict.SetStringKey("user_name", wallet->user_name);
    dict.SetStringKey("account_url", wallet->account_url);
    dict.SetStringKey("login_url", wallet->login_url);
    base::JSONWriter::Write(dict, &json_wallet);
  }
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnGetExternalWallet(env,
      weak_java_presearch_rewards_native_worker_.get(env),
      static_cast<int>(result),
      base::android::ConvertUTF8ToJavaString(env, json_wallet));
}

void PresearchRewardsNativeWorker::DisconnectWallet(JNIEnv* env) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->DisconnectWallet();
  }
}

void PresearchRewardsNativeWorker::OnDisconnectWallet(
    presearch_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    const std::string& wallet_type) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnDisconnectWallet(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        static_cast<int>(result),
        base::android::ConvertUTF8ToJavaString(env, wallet_type));
}

void PresearchRewardsNativeWorker::ProcessRewardsPageUrl(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& path,
        const base::android::JavaParamRef<jstring>& query) {
  if (presearch_rewards_service_) {
    std::string cpath = base::android::ConvertJavaStringToUTF8(env, path);
    std::string cquery = base::android::ConvertJavaStringToUTF8(env, query);
    auto callback =
        base::Bind(&PresearchRewardsNativeWorker::OnProcessRewardsPageUrl,
                   weak_factory_.GetWeakPtr());
    presearch_rewards_service_->ProcessRewardsPageUrl(cpath, cquery, callback);
  }
}

void PresearchRewardsNativeWorker::OnProcessRewardsPageUrl(
    const ledger::type::Result result,
    const std::string& wallet_type,
    const std::string& action,
    const base::flat_map<std::string, std::string>& args) {
  std::string json_args = StdStrStrMapToJsonString(args);
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnProcessRewardsPageUrl(env,
        weak_java_presearch_rewards_native_worker_.get(env),
        static_cast<int>(result),
        base::android::ConvertUTF8ToJavaString(env, wallet_type),
        base::android::ConvertUTF8ToJavaString(env, action),
        base::android::ConvertUTF8ToJavaString(env, json_args));
}

std::string PresearchRewardsNativeWorker::StdStrStrMapToJsonString(
    const base::flat_map<std::string, std::string>& args) {
    std::string json_args;
    base::Value dict(base::Value::Type::DICTIONARY);
    for (const auto & item : args) {
      dict.SetStringKey(item.first, item.second);
    }
    base::JSONWriter::Write(dict, &json_args);
    return json_args;
}

void PresearchRewardsNativeWorker::RecoverWallet(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& pass_phrase) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->RecoverWallet(
        base::android::ConvertJavaStringToUTF8(env, pass_phrase));
  }
}

void PresearchRewardsNativeWorker::OnRecoverWallet(
    presearch_rewards::RewardsService* rewards_service,
    const ledger::type::Result result) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnRecoverWallet(
      env, weak_java_presearch_rewards_native_worker_.get(env),
      static_cast<int>(result));
}

void PresearchRewardsNativeWorker::RefreshPublisher(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher_key) {
  if (!presearch_rewards_service_) {
    NOTREACHED();
    return;
  }
  presearch_rewards_service_->RefreshPublisher(
      base::android::ConvertJavaStringToUTF8(env, publisher_key),
      base::BindOnce(&PresearchRewardsNativeWorker::OnRefreshPublisher,
                     weak_factory_.GetWeakPtr()));
}

void PresearchRewardsNativeWorker::OnRefreshPublisher(
    const ledger::type::PublisherStatus status,
    const std::string& publisher_key) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_PresearchRewardsNativeWorker_OnRefreshPublisher(
      env, weak_java_presearch_rewards_native_worker_.get(env),
      static_cast<int>(status),
      base::android::ConvertUTF8ToJavaString(env, publisher_key));
}

void PresearchRewardsNativeWorker::SetAutoContributeEnabled(
    JNIEnv* env,
    bool isAutoContributeEnabled) {
  if (presearch_rewards_service_) {
    presearch_rewards_service_->SetAutoContributeEnabled(isAutoContributeEnabled);
  }
}

static void JNI_PresearchRewardsNativeWorker_Init(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  new PresearchRewardsNativeWorker(env, jcaller);
}

}  // namespace android
}  // namespace chrome
