/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCH_COMPONENTS_PRESEARCH_REFERRALS_BROWSER_PRESEARCH_REFERRALS_SERVICE_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_REFERRALS_BROWSER_PRESEARCH_REFERRALS_SERVICE_H_

#include <memory>
#include <string>

#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile_manager_observer.h"
#include "url/gurl.h"

#if defined(OS_ANDROID)
#include "presearch/components/safetynet/safetynet_check.h"
#endif

class PrefRegistrySimple;
class PrefService;
class Profile;

namespace network {
class SimpleURLLoader;
}

namespace presearch {

std::string GetAPIKey();

class PresearchReferralsService : public ProfileManagerObserver {
 public:
  explicit PresearchReferralsService(PrefService* pref_service,
                                 const std::string& platform,
                                 const std::string& api_key);
  ~PresearchReferralsService() override;

  void Start();
  void Stop();

  using ReferralInitializedCallback =
      base::RepeatingCallback<void(const std::string& download_id)>;

  static void SetReferralInitializedCallbackForTesting(
      ReferralInitializedCallback referral_initialized_callback);

  static void SetPromoFilePathForTesting(const base::FilePath& path);

  static bool GetMatchingReferralHeaders(
      const base::ListValue& referral_headers_list,
      const base::DictionaryValue** request_headers_dict,
      const GURL& url);

  static bool IsDefaultReferralCode(const std::string& code);

 private:
  // ProfileManagerObserver
  void OnProfileAdded(Profile* profile) override;

  void GetFirstRunTime();
  void SetFirstRunTime(const base::Time& first_run_timestamp);
  void PerformFinalizationChecks();
  base::FilePath GetPromoCodeFileName() const;
  void MaybeCheckForReferralFinalization();
  void MaybeDeletePromoCodePref() const;
  void InitReferral();
  std::string BuildReferralInitPayload() const;
  std::string BuildReferralFinalizationCheckPayload() const;
  void FetchReferralHeaders();
  void CheckForReferralFinalization();
  std::string FormatExtraHeaders(const base::Value* referral_headers,
                                 const GURL& url);

  // Invoked from RepeatingTimer when finalization checks timer
  // fires.
  void OnFinalizationChecksTimerFired();

  // Invoked from RepeatingTimer when referral headers timer fires.
  void OnFetchReferralHeadersTimerFired();

  // Invoked from SimpleURLLoader after download of referral headers
  // is complete.
  void OnReferralHeadersLoadComplete(
      std::unique_ptr<std::string> response_body);

  // Invoked from SimpleURLLoader after referral init load
  // completes.
  void OnReferralInitLoadComplete(std::unique_ptr<std::string> response_body);

  // Invoked from SimpleURLLoader after referral finalization check
  // load completes.
  void OnReferralFinalizationCheckLoadComplete(
      std::unique_ptr<std::string> response_body);

  // Invoked after reading contents of promo code file.
  void OnReadPromoCodeComplete(const std::string& promo_code);

#if defined(OS_ANDROID)
  void GetSafetynetStatusResult(const bool token_received,
                                const std::string& result_string,
                                const bool attestation_passed);
  safetynet_check::SafetyNetCheckRunner safetynet_check_runner_;
#endif

  bool initialized_;
  base::Time first_run_timestamp_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  std::unique_ptr<network::SimpleURLLoader> referral_headers_loader_;
  std::unique_ptr<network::SimpleURLLoader> referral_init_loader_;
  std::unique_ptr<network::SimpleURLLoader> referral_finalization_check_loader_;
  std::unique_ptr<base::OneShotTimer> initialization_timer_;
  std::unique_ptr<base::RepeatingTimer> fetch_referral_headers_timer_;
  std::unique_ptr<base::RepeatingTimer> finalization_checks_timer_;
  ReferralInitializedCallback referral_initialized_callback_;
  PrefService* pref_service_;
  const std::string api_key_;
  const std::string platform_;
  std::string promo_code_;

  base::WeakPtrFactory<PresearchReferralsService> weak_factory_;
};

// Registers the preferences used by PresearchReferralsService
void RegisterPrefsForPresearchReferralsService(PrefRegistrySimple* registry);

}  // namespace presearch

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_REFERRALS_BROWSER_PRESEARCH_REFERRALS_SERVICE_H_
