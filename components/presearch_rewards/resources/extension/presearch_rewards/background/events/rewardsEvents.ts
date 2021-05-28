/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import rewardsPanelActions from '../actions/rewardsPanelActions'

chrome.presearchRewards.onPublisherData.addListener((windowId: number, publisher: RewardsExtension.Publisher) => {
  rewardsPanelActions.onPublisherData(windowId, publisher)

  // Get publisher amounts
  if (publisher && publisher.publisherKey && publisher.status !== 0) {
    chrome.presearchRewards.getPublisherBanner(publisher.publisherKey, ((banner: RewardsExtension.PublisherBanner) => {
      rewardsPanelActions.onPublisherBanner(banner)
    }))
  }
})

chrome.presearchRewards.onPromotions.addListener((result: number, promotions: RewardsExtension.Promotion[]) => {
  rewardsPanelActions.onPromotions(result, promotions)
})

chrome.rewardsNotifications.onNotificationAdded.addListener((id: string, type: number, timestamp: number, args: string[]) => {
  rewardsPanelActions.onNotificationAdded(id, type, timestamp, args)
})

chrome.rewardsNotifications.onNotificationDeleted.addListener((id: string, type: number, timestamp: number) => {
  chrome.windows.getAll({ populate: true }, (windows) => {
    rewardsPanelActions.onNotificationDeleted(id, type, timestamp, windows)
  })
})

chrome.rewardsNotifications.onAllNotificationsDeleted.addListener(() => {
  rewardsPanelActions.onAllNotificationsDeleted()
})

chrome.presearchRewards.onPendingContributionSaved.addListener((result: number) => {
  if (result === 0) {
    chrome.presearchRewards.getPendingContributionsTotal(((amount: number) => {
      rewardsPanelActions.OnPendingContributionsTotal(amount)
    }))
  }
})

chrome.presearchRewards.onPublisherListNormalized.addListener((properties: RewardsExtension.PublisherNormalized[]) => {
  rewardsPanelActions.onPublisherListNormalized(properties)
})

chrome.presearchRewards.onExcludedSitesChanged.addListener((properties: RewardsExtension.ExcludedSitesChanged) => {
  rewardsPanelActions.onExcludedSitesChanged(properties)
})

chrome.presearchRewards.onRecurringTipSaved.addListener((success: boolean) => {
  if (success) {
    chrome.presearchRewards.getRecurringTips((tips: RewardsExtension.RecurringTips) => {
      rewardsPanelActions.onRecurringTips(tips)
    })
  }
})

chrome.presearchRewards.onRecurringTipRemoved.addListener((success: boolean) => {
  if (success) {
    chrome.presearchRewards.getRecurringTips((tips: RewardsExtension.RecurringTips) => {
      rewardsPanelActions.onRecurringTips(tips)
    })
  }
})

chrome.presearchRewards.onReconcileComplete.addListener((result: number, type: number) => {
  if (result === 0) {
    chrome.presearchRewards.fetchBalance((balance: RewardsExtension.Balance) => {
      rewardsPanelActions.onBalance(balance)
    })

    chrome.presearchRewards.getBalanceReport(new Date().getMonth() + 1, new Date().getFullYear(),
    (report: RewardsExtension.BalanceReport) => {
      rewardsPanelActions.onBalanceReport(report)
    })
  }
})

chrome.presearchRewards.onDisconnectWallet.addListener((properties: {result: number, walletType: string}) => {
  if (properties.result === 0) {
    chrome.presearchRewards.getExternalWallet((result: number, wallet: RewardsExtension.ExternalWallet) => {
      rewardsPanelActions.onExternalWallet(wallet)
    })

    chrome.presearchRewards.fetchBalance((balance: RewardsExtension.Balance) => {
      rewardsPanelActions.onBalance(balance)
    })
  }
})

chrome.presearchRewards.onUnblindedTokensReady.addListener(() => {
  chrome.presearchRewards.fetchBalance((balance: RewardsExtension.Balance) => {
    rewardsPanelActions.onBalance(balance)
  })
})

chrome.presearchRewards.onPromotionFinish.addListener((result: RewardsExtension.Result, promotion: RewardsExtension.Promotion) => {
  rewardsPanelActions.promotionFinished(result, promotion)

  chrome.presearchRewards.getBalanceReport(new Date().getMonth() + 1, new Date().getFullYear(),
  (report: RewardsExtension.BalanceReport) => {
    rewardsPanelActions.onBalanceReport(report)
  })
})

chrome.presearchRewards.onCompleteReset.addListener((properties: { success: boolean }) => {
  rewardsPanelActions.onCompleteReset(properties.success)
})

chrome.presearchRewards.initialized.addListener((result: RewardsExtension.Result) => {
  rewardsPanelActions.initialized()
})
