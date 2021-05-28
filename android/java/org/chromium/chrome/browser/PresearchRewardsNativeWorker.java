/** Copyright (c) 2019 The Presearch Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser;

import android.os.Handler;

import androidx.annotation.Nullable;

import org.json.JSONException;

import org.chromium.base.Log;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.PresearchRewardsBalance;
import org.chromium.chrome.browser.PresearchRewardsHelper;
import org.chromium.chrome.browser.PresearchRewardsObserver;
import org.chromium.chrome.browser.PresearchRewardsPublisher.PublisherStatus;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.components.embedder_support.util.UrlConstants;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class PresearchRewardsNativeWorker {
    /**
     * Allows to monitor a front tab publisher changes.
     */
    public interface PublisherObserver { void onFrontTabPublisherChanged(boolean verified); }

    // Rewards notifications
    // Taken from components/presearch_rewards/browser/rewards_notification_service.h
    public static final int REWARDS_NOTIFICATION_INVALID = 0;
    public static final int REWARDS_NOTIFICATION_AUTO_CONTRIBUTE = 1;
    public static final int REWARDS_NOTIFICATION_GRANT = 2;
    public static final int REWARDS_NOTIFICATION_GRANT_ADS = 3;
    public static final int REWARDS_NOTIFICATION_FAILED_CONTRIBUTION = 4;
    public static final int REWARDS_NOTIFICATION_IMPENDING_CONTRIBUTION = 5;
    public static final int REWARDS_NOTIFICATION_INSUFFICIENT_FUNDS = 6;
    public static final int REWARDS_NOTIFICATION_BACKUP_WALLET = 7;
    public static final int REWARDS_NOTIFICATION_TIPS_PROCESSED = 8;
    public static final int REWARDS_NOTIFICATION_ADS_ONBOARDING = 9;
    public static final int REWARDS_NOTIFICATION_VERIFIED_PUBLISHER = 10;

    public static final int LEDGER_OK = 0;
    public static final int LEDGER_ERROR = 1;
    public static final int WALLET_CREATED = 12;
    public static final int BAT_NOT_ALLOWED = 25;
    public static final int SAFETYNET_ATTESTATION_FAILED = 27;

    private static final int REWARDS_UNKNOWN = 0;
    private static final int REWARDS_DISABLED = 1;
    private static final int REWARDS_ENABLED = 2;
    private static int rewardsStatus = REWARDS_UNKNOWN;
    private String frontTabUrl;
    private static final Handler mHandler = new Handler();

    private List<PresearchRewardsObserver> mObservers;
    private List<PublisherObserver> mFrontTabPublisherObservers;
    private long mNativePresearchRewardsNativeWorker;

    private static PresearchRewardsNativeWorker instance;
    private static final Object lock = new Object();
    private boolean grantClaimInProcess;  // flag: wallet is being created

    public static  PresearchRewardsNativeWorker getInstance() {
        synchronized(lock) {
          if(instance == null) {
              instance = new PresearchRewardsNativeWorker();
              instance.Init();
          }
        }
        return instance;
    }

    private PresearchRewardsNativeWorker() {
        mObservers = new ArrayList<PresearchRewardsObserver>();
        mFrontTabPublisherObservers = new ArrayList<PublisherObserver>();
    }

    private void Init() {
      if (mNativePresearchRewardsNativeWorker == 0) {
          PresearchRewardsNativeWorkerJni.get().init(PresearchRewardsNativeWorker.this);
      }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativePresearchRewardsNativeWorker != 0) {
            PresearchRewardsNativeWorkerJni.get().destroy(mNativePresearchRewardsNativeWorker);
            mNativePresearchRewardsNativeWorker = 0;
        }
    }

    public void AddObserver(PresearchRewardsObserver observer) {
        synchronized(lock) {
            mObservers.add(observer);
        }
    }

    public void RemoveObserver(PresearchRewardsObserver observer) {
        synchronized(lock) {
            mObservers.remove(observer);
        }
    }

    public void AddPublisherObserver(PublisherObserver observer) {
        synchronized (lock) {
            mFrontTabPublisherObservers.add(observer);
        }
    }

    public void RemovePublisherObserver(PublisherObserver observer) {
        synchronized (lock) {
            mFrontTabPublisherObservers.remove(observer);
        }
    }

    public void OnNotifyFrontTabUrlChanged(int tabId, String url) {
        boolean chromeUrl = url.startsWith(UrlConstants.CHROME_SCHEME);
        boolean newUrl = (frontTabUrl == null || !frontTabUrl.equals(url));
        if (chromeUrl) {
            // Don't query 'GetPublisherInfo' and post response now.
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    NotifyPublisherObservers(false);
                }
            });
        } else if (newUrl) {
            GetPublisherInfo(tabId, url);
        }

        frontTabUrl = url;
    }

    private void NotifyPublisherObservers(boolean verified) {
        for (PublisherObserver observer : mFrontTabPublisherObservers) {
            observer.onFrontTabPublisherChanged(verified);
        }
    }

    public void TriggerOnNotifyFrontTabUrlChanged() {
        // Clear frontTabUrl so that all observers are updated.
        frontTabUrl = "";
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                Tab tab = PresearchRewardsHelper.currentActiveChromeTabbedActivityTab();
                if (tab != null && !tab.isIncognito()) {
                    OnNotifyFrontTabUrlChanged(tab.getId(), tab.getUrlString());
                }
            }
        });
    }

    public boolean IsGrantClaimInProcess() {
        synchronized(lock) {
          return grantClaimInProcess;
        }
    }

    public void GetRewardsParameters() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getRewardsParameters(mNativePresearchRewardsNativeWorker);
        }
    }

    @Nullable
    public PresearchRewardsBalance GetWalletBalance() {
        synchronized(lock) {
            String json = PresearchRewardsNativeWorkerJni.get().getWalletBalance(
                    mNativePresearchRewardsNativeWorker);
            PresearchRewardsBalance balance = null;
            try{
                balance = new PresearchRewardsBalance (json);
            }
            catch (JSONException e) {
                balance = null;
            }
            return balance;
        }
    }

    public double GetWalletRate() {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getWalletRate(mNativePresearchRewardsNativeWorker);
        }
    }

    public void GetPublisherInfo(int tabId, String host) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getPublisherInfo(
                    mNativePresearchRewardsNativeWorker, tabId, host);
        }
    }

    public String GetPublisherURL(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherURL(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherFavIconURL(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherFavIconURL(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherName(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherName(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public String GetPublisherId(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherId(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public int GetPublisherPercent(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherPercent(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public boolean GetPublisherExcluded(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherExcluded(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public @PublisherStatus int GetPublisherStatus(int tabId) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherStatus(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public void IncludeInAutoContribution(int tabId, boolean exclude) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().includeInAutoContribution(
                    mNativePresearchRewardsNativeWorker, tabId, exclude);
        }
    }

    public void RemovePublisherFromMap(int tabId) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().removePublisherFromMap(
                    mNativePresearchRewardsNativeWorker, tabId);
        }
    }

    public void GetCurrentBalanceReport() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getCurrentBalanceReport(
                    mNativePresearchRewardsNativeWorker);
        }
    }

    public void Donate(String publisher_key, int amount, boolean recurring) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().donate(
                    mNativePresearchRewardsNativeWorker, publisher_key, amount, recurring);
        }
    }

    public void GetAllNotifications() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getAllNotifications(mNativePresearchRewardsNativeWorker);
        }
    }

    public void DeleteNotification(String notification_id) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().deleteNotification(
                    mNativePresearchRewardsNativeWorker, notification_id);
        }
    }

    public void GetGrant(String promotionId) {
        synchronized(lock) {
            if (grantClaimInProcess) {
                return;
            }
            grantClaimInProcess = true;
            PresearchRewardsNativeWorkerJni.get().getGrant(
                    mNativePresearchRewardsNativeWorker, promotionId);
        }
    }

    public String[] GetCurrentGrant(int position) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getCurrentGrant(
                    mNativePresearchRewardsNativeWorker, position);
        }
    }

    public void GetPendingContributionsTotal() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getPendingContributionsTotal(
                    mNativePresearchRewardsNativeWorker);
        }
    }

    public void GetRecurringDonations() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getRecurringDonations(
                    mNativePresearchRewardsNativeWorker);
        }
    }

    public boolean IsCurrentPublisherInRecurrentDonations(String publisher) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().isCurrentPublisherInRecurrentDonations(
                    mNativePresearchRewardsNativeWorker, publisher);
        }
    }

    public double GetPublisherRecurrentDonationAmount(String publisher) {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().getPublisherRecurrentDonationAmount(
                    mNativePresearchRewardsNativeWorker, publisher);
        }
    }

    public void GetAutoContributeProperties() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getAutoContributeProperties(
                    mNativePresearchRewardsNativeWorker);
        }
    }

    public boolean IsAutoContributeEnabled() {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().isAutoContributeEnabled(
                    mNativePresearchRewardsNativeWorker);
        }
    }

    public void GetReconcileStamp() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().getReconcileStamp(mNativePresearchRewardsNativeWorker);
        }
    }

    public void RemoveRecurring(String publisher) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().removeRecurring(
                    mNativePresearchRewardsNativeWorker, publisher);
        }
    }

    public void ResetTheWholeState() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().resetTheWholeState(mNativePresearchRewardsNativeWorker);
        }
    }

    public void FetchGrants() {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().fetchGrants(mNativePresearchRewardsNativeWorker);
        }
    }

    public int GetAdsPerHour() {
        synchronized (lock) {
            return PresearchRewardsNativeWorkerJni.get().getAdsPerHour(mNativePresearchRewardsNativeWorker);
        }
    }

    public void SetAdsPerHour(int value) {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().setAdsPerHour(mNativePresearchRewardsNativeWorker, value);
        }
    }

    public boolean IsAnonWallet() {
        synchronized(lock) {
            return PresearchRewardsNativeWorkerJni.get().isAnonWallet(mNativePresearchRewardsNativeWorker);
        }
    }

    public boolean isRewardsEnabled() {
        synchronized (lock) {
            return PresearchRewardsNativeWorkerJni.get().isRewardsEnabled(
                    mNativePresearchRewardsNativeWorker);
        }
    }

    public void GetExternalWallet() {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().getExternalWallet(mNativePresearchRewardsNativeWorker);
        }
    }

    public void DisconnectWallet() {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().disconnectWallet(mNativePresearchRewardsNativeWorker);
        }
    }

    public void ProcessRewardsPageUrl(String path, String query) {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().processRewardsPageUrl(
                    mNativePresearchRewardsNativeWorker, path, query);
        }
    }

    public void RecoverWallet(String passPhrase) {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().recoverWallet(
                    mNativePresearchRewardsNativeWorker, passPhrase);
        }
    }

    @CalledByNative
    public void OnRecoverWallet(int errorCode) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnRecoverWallet(errorCode);
        }
    }

    public void RefreshPublisher(String publisherKey) {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().refreshPublisher(
                    mNativePresearchRewardsNativeWorker, publisherKey);
        }
    }

    public void SetAutoContributeEnabled(boolean isSetAutoContributeEnabled) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().setAutoContributeEnabled(
                    mNativePresearchRewardsNativeWorker, isSetAutoContributeEnabled);
        }
    }

    public void SetAutoContributionAmount(double amount) {
        synchronized(lock) {
            PresearchRewardsNativeWorkerJni.get().setAutoContributionAmount(
                    mNativePresearchRewardsNativeWorker, amount);
        }
    }

    public void StartProcess() {
        synchronized (lock) {
            PresearchRewardsNativeWorkerJni.get().startProcess(mNativePresearchRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void OnStartProcess() {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnStartProcess();
        }
    }

    @CalledByNative
    public void OnRefreshPublisher(int status, String publisherKey) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnRefreshPublisher(status, publisherKey);
        }
    }

    @CalledByNative
    public void OnRewardsParameters(int errorCode) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnRewardsParameters(errorCode);
        }
    }

    @CalledByNative
    public void OnGetCurrentBalanceReport(double[] report) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnGetCurrentBalanceReport(report);
        }
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativePresearchRewardsNativeWorker == 0;
        mNativePresearchRewardsNativeWorker = nativePtr;
    }

    @CalledByNative
    public void OnPublisherInfo(int tabId) {
        @PublisherStatus int pubStatus = GetPublisherStatus(tabId);
        boolean verified = (pubStatus == PresearchRewardsPublisher.CONNECTED
                                   || pubStatus == PresearchRewardsPublisher.UPHOLD_VERIFIED)
                ? true
                : false;
        NotifyPublisherObservers(verified);

        // Notify PresearchRewardsObserver (panel).
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnPublisherInfo(tabId);
        }
    }

    @CalledByNative
    public void OnNotificationAdded(String id, int type, long timestamp,
            String[] args) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnNotificationAdded(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void OnNotificationsCount(int count) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnNotificationsCount(count);
        }
    }

    @CalledByNative
    public void OnGetLatestNotification(String id, int type, long timestamp,
            String[] args) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnGetLatestNotification(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void OnNotificationDeleted(String id) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnNotificationDeleted(id);
        }
    }

    @CalledByNative
    public void OnGetPendingContributionsTotal(double amount) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnGetPendingContributionsTotal(amount);
        }
    }

    @CalledByNative
    public void OnGetAutoContributeProperties() {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnGetAutoContributeProperties();
        }
    }

    @CalledByNative
    public void OnGetReconcileStamp(long timestamp) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnGetReconcileStamp(timestamp);
        }
    }

    @CalledByNative
    public void OnRecurringDonationUpdated() {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnRecurringDonationUpdated();
        }
    }

    @CalledByNative
    public void OnGrantFinish(int result) {
        grantClaimInProcess = false;
        for(PresearchRewardsObserver observer : mObservers) {
            observer.OnGrantFinish(result);
        }
    }

    @CalledByNative
    public void OnResetTheWholeState(boolean success) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnResetTheWholeState(success);
        }
    }

    @CalledByNative
    public void OnGetExternalWallet(int error_code, String external_wallet) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnGetExternalWallet(error_code, external_wallet);
        }
    }

    @CalledByNative
    public void OnDisconnectWallet(int error_code, String external_wallet) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnDisconnectWallet(error_code, external_wallet);
        }
    }

    @CalledByNative
    public void OnProcessRewardsPageUrl(int error_code, String wallet_type,
            String action, String json_args) {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnProcessRewardsPageUrl(error_code, wallet_type,
                    action, json_args);
        }
    }

    @CalledByNative
    public void OnClaimPromotion(int error_code) {
        grantClaimInProcess = false;
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnClaimPromotion(error_code);
        }
    }

    @CalledByNative
    public void OnOneTimeTip() {
        for (PresearchRewardsObserver observer : mObservers) {
            observer.OnOneTimeTip();
        }
    }

    @NativeMethods
    interface Natives {
        void init(PresearchRewardsNativeWorker caller);
        void destroy(long nativePresearchRewardsNativeWorker);
        String getWalletBalance(long nativePresearchRewardsNativeWorker);
        double getWalletRate(long nativePresearchRewardsNativeWorker);
        void getPublisherInfo(long nativePresearchRewardsNativeWorker, int tabId, String host);
        String getPublisherURL(long nativePresearchRewardsNativeWorker, int tabId);
        String getPublisherFavIconURL(long nativePresearchRewardsNativeWorker, int tabId);
        String getPublisherName(long nativePresearchRewardsNativeWorker, int tabId);
        String getPublisherId(long nativePresearchRewardsNativeWorker, int tabId);
        int getPublisherPercent(long nativePresearchRewardsNativeWorker, int tabId);
        boolean getPublisherExcluded(long nativePresearchRewardsNativeWorker, int tabId);
        int getPublisherStatus(long nativePresearchRewardsNativeWorker, int tabId);
        void includeInAutoContribution(
                long nativePresearchRewardsNativeWorker, int tabId, boolean exclude);
        void removePublisherFromMap(long nativePresearchRewardsNativeWorker, int tabId);
        void getCurrentBalanceReport(long nativePresearchRewardsNativeWorker);
        void donate(long nativePresearchRewardsNativeWorker, String publisher_key, int amount,
                boolean recurring);
        void getAllNotifications(long nativePresearchRewardsNativeWorker);
        void deleteNotification(long nativePresearchRewardsNativeWorker, String notification_id);
        void getGrant(long nativePresearchRewardsNativeWorker, String promotionId);
        String[] getCurrentGrant(long nativePresearchRewardsNativeWorker, int position);
        void getPendingContributionsTotal(long nativePresearchRewardsNativeWorker);
        void getRecurringDonations(long nativePresearchRewardsNativeWorker);
        boolean isCurrentPublisherInRecurrentDonations(
                long nativePresearchRewardsNativeWorker, String publisher);
        void getAutoContributeProperties(long nativePresearchRewardsNativeWorker);
        boolean isAutoContributeEnabled(long nativePresearchRewardsNativeWorker);
        void getReconcileStamp(long nativePresearchRewardsNativeWorker);
        double getPublisherRecurrentDonationAmount(
                long nativePresearchRewardsNativeWorker, String publisher);
        void removeRecurring(long nativePresearchRewardsNativeWorker, String publisher);
        void resetTheWholeState(long nativePresearchRewardsNativeWorker);
        void fetchGrants(long nativePresearchRewardsNativeWorker);
        int getAdsPerHour(long nativePresearchRewardsNativeWorker);
        void setAdsPerHour(long nativePresearchRewardsNativeWorker, int value);
        boolean isAnonWallet(long nativePresearchRewardsNativeWorker);
        boolean isRewardsEnabled(long nativePresearchRewardsNativeWorker);
        void getExternalWallet(long nativePresearchRewardsNativeWorker);
        void disconnectWallet(long nativePresearchRewardsNativeWorker);
        void processRewardsPageUrl(long nativePresearchRewardsNativeWorker, String path, String query);
        void recoverWallet(long nativePresearchRewardsNativeWorker, String passPhrase);
        void refreshPublisher(long nativePresearchRewardsNativeWorker, String publisherKey);
        void getRewardsParameters(long nativePresearchRewardsNativeWorker);
        void setAutoContributeEnabled(
                long nativePresearchRewardsNativeWorker, boolean isSetAutoContributeEnabled);
        void setAutoContributionAmount(long nativePresearchRewardsNativeWorker, double amount);
        void startProcess(long nativePresearchRewardsNativeWorker);
    }
}
