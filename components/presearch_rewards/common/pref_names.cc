/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_rewards/common/pref_names.h"

namespace presearch_rewards {
namespace prefs {

const char kHideButton[] = "presearch.hide_presearch_rewards_button";
const char kEnabled[] = "presearch.rewards.enabled";
const char kNotifications[] = "presearch.rewards.notifications";
const char kNotificationTimerInterval[]=
    "presearch.rewards.notification_timer_interval";
const char kBackupNotificationInterval[] =
    "presearch.rewards.backup_notification_interval";
const char kBackupSucceeded[] = "presearch.rewards.backup_succeeded";
const char kUserHasFunded[] = "presearch.rewards.user_has_funded";
const char kAddFundsNotification[] =
    "presearch.rewards.add_funds_notification";
const char kNotificationStartupDelay[] =
    "presearch.rewards.notification_startup_delay";
const char kExternalWallets[] = "presearch.rewards.external_wallets";
const char kServerPublisherListStamp[] =
    "presearch.rewards.publisher_prefix_list_stamp";
const char kUpholdAnonAddress[] =
    "presearch.rewards.uphold_anon_address";
const char kBadgeText[] = "presearch.rewards.badge_text";
const char kUseRewardsStagingServer[] = "presearch.rewards.use_staging_server";
const char kPromotionLastFetchStamp[] =
    "presearch.rewards.promotion_last_fetch_stamp";
const char kPromotionCorruptedMigrated[] =
    "presearch.rewards.promotion_corrupted_migrated2";
const char kAnonTransferChecked[] =  "presearch.rewards.anon_transfer_checked";
const char kVersion[] =  "presearch.rewards.version";
const char kMinVisitTime[] =  "presearch.rewards.ac.min_visit_time";
const char kMinVisits[] =  "presearch.rewards.ac.min_visits";
const char kAllowNonVerified[] =  "presearch.rewards.ac.allow_non_verified";
const char kAllowVideoContribution[] =
    "presearch.rewards.ac.allow_video_contributions";
const char kScoreA[] = "presearch.rewards.ac.score.a";
const char kScoreB[] = "presearch.rewards.ac.score.b";
const char kAutoContributeEnabled[] = "presearch.rewards.ac.enabled";
const char kAutoContributeAmount[] = "presearch.rewards.ac.amount";
const char kNextReconcileStamp[] = "presearch.rewards.ac.next_reconcile_stamp";
const char kCreationStamp[] = "presearch.rewards.creation_stamp";
const char kRecoverySeed[] = "presearch.rewards.wallet.seed";
const char kPaymentId[] = "presearch.rewards.wallet.payment_id";
const char kInlineTipRedditEnabled[] = "presearch.rewards.inline_tip.reddit";
const char kInlineTipTwitterEnabled[] = "presearch.rewards.inline_tip.twitter";
const char kInlineTipGithubEnabled[] = "presearch.rewards.inline_tip.github";
const char kParametersRate[] = "presearch.rewards.parameters.rate";
const char kParametersAutoContributeChoice[] =
    "presearch.rewards.parameters.ac.choice";
const char kParametersAutoContributeChoices[] =
    "presearch.rewards.parameters.ac.choices";
const char kParametersTipChoices[] =
    "presearch.rewards.parameters.tip.choices";
const char kParametersMonthlyTipChoices[] =
    "presearch.rewards.parameters.tip.monthly_choices";
const char kFetchOldBalance[] =
    "presearch.rewards.fetch_old_balance";
const char kEmptyBalanceChecked[] =
    "presearch.rewards.empty_balance_checked";
const char kWalletPresearch[] =
    "presearch.rewards.wallets.presearch";
const char kWalletUphold[] =
    "presearch.rewards.wallets.uphold";
const char kWalletBitflyer[] = "presearch.rewards.wallets.bitflyer";

}  // namespace prefs
}  // namespace presearch_rewards
