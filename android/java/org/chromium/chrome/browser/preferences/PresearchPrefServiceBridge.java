/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences;

import androidx.annotation.NonNull;

import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.profiles.Profile;

@JNINamespace("chrome::android")
public class PresearchPrefServiceBridge {
    private PresearchPrefServiceBridge() {
    }

    private static PresearchPrefServiceBridge sInstance;

    public static PresearchPrefServiceBridge getInstance() {
        ThreadUtils.assertOnUiThread();
        if (sInstance == null) {
            sInstance = new PresearchPrefServiceBridge();
        }
        return sInstance;
    }

    /**
     * @param whether HTTPSE should be enabled.
     */
    public void setHTTPSEEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setHTTPSEEnabled(enabled);
    }

    /**
     * @param whether the IPFS gateway should be enabled.
     */
    public void setIpfsGatewayEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setIpfsGatewayEnabled(enabled);
    }

    /**
     * @param whether google login is enabled on third party sites.
     */
    public void setThirdPartyGoogleLoginEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setThirdPartyGoogleLoginEnabled(enabled);
    }

    /**
     * @param whether facebook embeds are allowed on third party sites.
     */
    public void setThirdPartyFacebookEmbedEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setThirdPartyFacebookEmbedEnabled(enabled);
    }

    /**
     * @param whether twitter embeds are allowed on third party sites.
     */
    public void setThirdPartyTwitterEmbedEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setThirdPartyTwitterEmbedEnabled(enabled);
    }

    /**
     * @param whether linkedin embeds are allowed on third party sites.
     */
    public void setThirdPartyLinkedinEmbedEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setThirdPartyLinkedinEmbedEnabled(enabled);
    }

    /**
     * @param whether AdBlock should be enabled.
     */
    public void setAdBlockEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setAdBlockEnabled(enabled);
    }

    /**
     * @param whether Fingerprinting Protection should be enabled.
     */
    public void setFingerprintingProtectionEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setFingerprintingProtectionEnabled(enabled);
    }

    public void setPlayYTVideoInBrowserEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setPlayYTVideoInBrowserEnabled(enabled);
    }

    public boolean getPlayYTVideoInBrowserEnabled() {
        return PresearchPrefServiceBridgeJni.get().getPlayYTVideoInBrowserEnabled();
    }

    public void setBackgroundVideoPlaybackEnabled(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setBackgroundVideoPlaybackEnabled(enabled);
    }

    public boolean getBackgroundVideoPlaybackEnabled() {
        return PresearchPrefServiceBridgeJni.get().getBackgroundVideoPlaybackEnabled();
    }

    public long getTrackersBlockedCount(Profile profile) {
        return PresearchPrefServiceBridgeJni.get().getTrackersBlockedCount(profile);
    }

    public long getAdsBlockedCount(Profile profile) {
        return PresearchPrefServiceBridgeJni.get().getAdsBlockedCount(profile);
    }

    public long getDataSaved(Profile profile) {
        return PresearchPrefServiceBridgeJni.get().getDataSaved(profile);
    }

    /**
     * @param whether SafetyNet check failed.
     */
    public void setSafetynetCheckFailed(boolean value) {
        PresearchPrefServiceBridgeJni.get().setSafetynetCheckFailed(value);
    }

    public boolean getSafetynetCheckFailed() {
        return PresearchPrefServiceBridgeJni.get().getSafetynetCheckFailed();
    }

    public void setSafetynetStatus(String status) {
        PresearchPrefServiceBridgeJni.get().setSafetynetStatus(status);
    }

    public void setUseRewardsStagingServer(boolean enabled) {
        PresearchPrefServiceBridgeJni.get().setUseRewardsStagingServer(false);
    }

    public void resetPromotionLastFetchStamp() {
        PresearchPrefServiceBridgeJni.get().resetPromotionLastFetchStamp();
    }

    public boolean getUseRewardsStagingServer() {
        return PresearchPrefServiceBridgeJni.get().getUseRewardsStagingServer();
    }

    public void setOldTrackersBlockedCount(Profile profile, long count) {
        PresearchPrefServiceBridgeJni.get().setOldTrackersBlockedCount(profile, count);
    }

    public void setOldAdsBlockedCount(Profile profile, long count) {
        PresearchPrefServiceBridgeJni.get().setOldAdsBlockedCount(profile, count);
    }

    public void setOldHttpsUpgradesCount(Profile profile, long count) {
        PresearchPrefServiceBridgeJni.get().setOldHttpsUpgradesCount(profile, count);
    }

    public boolean GetBooleanForContentSetting(int content_type) {
        return PresearchPrefServiceBridgeJni.get().getBooleanForContentSetting(content_type);
    }

    public void setReferralAndroidFirstRunTimestamp(long time) {
        PresearchPrefServiceBridgeJni.get().setReferralAndroidFirstRunTimestamp(time);
    }

    public void setReferralCheckedForPromoCodeFile(boolean value) {
        PresearchPrefServiceBridgeJni.get().setReferralCheckedForPromoCodeFile(value);
    }

    public void setReferralInitialization(boolean value) {
        PresearchPrefServiceBridgeJni.get().setReferralInitialization(value);
    }

    public void setReferralPromoCode(String promoCode) {
        PresearchPrefServiceBridgeJni.get().setReferralPromoCode(promoCode);
    }

    public void setReferralDownloadId(String downloadId) {
        PresearchPrefServiceBridgeJni.get().setReferralDownloadId(downloadId);
    }

    public void setP3AEnabled(boolean value) {
        PresearchPrefServiceBridgeJni.get().setP3AEnabled(value);
    }

    public boolean getP3AEnabled() {
        return PresearchPrefServiceBridgeJni.get().getP3AEnabled();
    }

    public boolean hasPathP3AEnabled() {
        return PresearchPrefServiceBridgeJni.get().hasPathP3AEnabled();
    }

    public void setP3ANoticeAcknowledged(boolean value) {
        PresearchPrefServiceBridgeJni.get().setP3ANoticeAcknowledged(value);
    }

    public boolean getP3ANoticeAcknowledged() {
        return PresearchPrefServiceBridgeJni.get().getP3ANoticeAcknowledged();
    }

    public void setUnstoppableDomainsResolveMethod(int method) {
        PresearchPrefServiceBridgeJni.get().setUnstoppableDomainsResolveMethod(method);
    }

    public int getUnstoppableDomainsResolveMethod() {
        return PresearchPrefServiceBridgeJni.get().getUnstoppableDomainsResolveMethod();
    }

    public void setENSResolveMethod(int method) {
        PresearchPrefServiceBridgeJni.get().setENSResolveMethod(method);
    }

    public int getENSResolveMethod() {
        return PresearchPrefServiceBridgeJni.get().getENSResolveMethod();
    }

    public void setWebrtcPolicy(int policy) {
        PresearchPrefServiceBridgeJni.get().setWebrtcPolicy(policy);
    }

    public int getWebrtcPolicy() {
        return PresearchPrefServiceBridgeJni.get().getWebrtcPolicy();
    }

    @NativeMethods
    interface Natives {
        void setHTTPSEEnabled(boolean enabled);
        void setIpfsGatewayEnabled(boolean enabled);
        void setAdBlockEnabled(boolean enabled);
        void setFingerprintingProtectionEnabled(boolean enabled);

        void setThirdPartyGoogleLoginEnabled(boolean enabled);
        void setThirdPartyFacebookEmbedEnabled(boolean enabled);
        void setThirdPartyTwitterEmbedEnabled(boolean enabled);
        void setThirdPartyLinkedinEmbedEnabled(boolean enabled);

        void setPlayYTVideoInBrowserEnabled(boolean enabled);
        boolean getPlayYTVideoInBrowserEnabled();

        void setBackgroundVideoPlaybackEnabled(boolean enabled);
        boolean getBackgroundVideoPlaybackEnabled();

        long getTrackersBlockedCount(Profile profile);
        long getAdsBlockedCount(Profile profile);
        long getDataSaved(Profile profile);

        // Used to pass total stat from upgrading old tabs based browser
        // to a new core based
        void setOldTrackersBlockedCount(Profile profile, long count);
        void setOldAdsBlockedCount(Profile profile, long count);
        void setOldHttpsUpgradesCount(Profile profile, long count);

        void setSafetynetCheckFailed(boolean value);
        boolean getSafetynetCheckFailed();

        void setSafetynetStatus(String status);

        void setUseRewardsStagingServer(boolean enabled);
        void resetPromotionLastFetchStamp();
        boolean getUseRewardsStagingServer();
        boolean getBooleanForContentSetting(int content_type);

        void setReferralAndroidFirstRunTimestamp(long time);
        void setReferralCheckedForPromoCodeFile(boolean value);
        void setReferralInitialization(boolean value);
        void setReferralPromoCode(String promoCode);
        void setReferralDownloadId(String downloadId);

        void setP3AEnabled(boolean value);
        boolean getP3AEnabled();
        boolean hasPathP3AEnabled();
        void setP3ANoticeAcknowledged(boolean value);
        boolean getP3ANoticeAcknowledged();

        void setUnstoppableDomainsResolveMethod(int method);
        void setENSResolveMethod(int method);
        int getUnstoppableDomainsResolveMethod();
        int getENSResolveMethod();

        void setWebrtcPolicy(int policy);
        int getWebrtcPolicy();
    }
}
