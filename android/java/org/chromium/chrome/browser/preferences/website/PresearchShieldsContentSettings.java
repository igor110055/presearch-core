/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences.website;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.preferences.website.PresearchShieldsContentSettingsObserver;
import org.chromium.chrome.browser.profiles.Profile;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class PresearchShieldsContentSettings {
    static public final String RESOURCE_IDENTIFIER_ADS = "shieldsAds";
    static public final String RESOURCE_IDENTIFIER_TRACKERS = "trackers";
    static public final String RESOURCE_IDENTIFIER_ADS_TRACKERS = "ads_trackers";
    static public final String RESOURCE_IDENTIFIER_DATA_SAVED = "data_saved";
    static public final String RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES = "httpUpgradableResources";
    static public final String RESOURCE_IDENTIFIER_PRESEARCH_SHIELDS = "presearchShields";
    static public final String RESOURCE_IDENTIFIER_FINGERPRINTING = "fingerprinting";
    static public final String RESOURCE_IDENTIFIER_COOKIES = "shieldsCookies";
    static public final String RESOURCE_IDENTIFIER_REFERRERS = "referrers";
    static public final String RESOURCE_IDENTIFIER_JAVASCRIPTS = "javascript";

    static public final String BLOCK_RESOURCE = "block";
    static public final String BLOCK_THIRDPARTY_RESOURCE = "block_third_party";
    static public final String DEFAULT = "default";
    static public final String ALLOW_RESOURCE = "allow";

    private long mNativePresearchShieldsContentSettings;
    private List<PresearchShieldsContentSettingsObserver> mPresearchShieldsContentSettingsObservers;
    private static PresearchShieldsContentSettings sInstance;

    public static PresearchShieldsContentSettings getInstance() {
        if (sInstance == null) sInstance = new PresearchShieldsContentSettings();
        return sInstance;
    }

    private PresearchShieldsContentSettings() {
        mNativePresearchShieldsContentSettings = 0;
        mPresearchShieldsContentSettingsObservers =
            new ArrayList<PresearchShieldsContentSettingsObserver>();
        init();
    }

    public void addObserver(PresearchShieldsContentSettingsObserver observer) {
        mPresearchShieldsContentSettingsObservers.add(observer);
    }

    public void removeObserver(PresearchShieldsContentSettingsObserver observer) {
        mPresearchShieldsContentSettingsObservers.remove(observer);
    }

    private void init() {
        if (mNativePresearchShieldsContentSettings == 0) {
            PresearchShieldsContentSettingsJni.get().init(this);
        }
    }

    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativePresearchShieldsContentSettings == 0) {
            return;
        }
        PresearchShieldsContentSettingsJni.get().destroy(mNativePresearchShieldsContentSettings);
    }

    static public void setShields(Profile profile, String host, String resourceIndentifier, boolean value,
                                  boolean fromTopShields) {
        String setting_string = (value ? BLOCK_RESOURCE : ALLOW_RESOURCE);
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_PRESEARCH_SHIELDS)) {
            PresearchShieldsContentSettingsJni.get().setPresearchShieldsEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_ADS_TRACKERS)) {
            PresearchShieldsContentSettingsJni.get().setAdControlType(setting_string, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            PresearchShieldsContentSettingsJni.get().setHTTPSEverywhereEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            PresearchShieldsContentSettingsJni.get().setNoScriptControlType(setting_string, host, profile);
        }
    }

    public static void setShieldsValue(Profile profile, String host, String resourceIndentifier,
                                       String settingOption, boolean fromTopShields) {
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            PresearchShieldsContentSettingsJni.get().setFingerprintingControlType(settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            PresearchShieldsContentSettingsJni.get().setCookieControlType(settingOption, host, profile);
        }
    }

    public static boolean getShields(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_PRESEARCH_SHIELDS)) {
            return PresearchShieldsContentSettingsJni.get().getPresearchShieldsEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_ADS_TRACKERS)) {
            settings = PresearchShieldsContentSettingsJni.get().getAdControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            return PresearchShieldsContentSettingsJni.get().getHTTPSEverywhereEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            settings = PresearchShieldsContentSettingsJni.get().getNoScriptControlType(host, profile);
        }

        return !settings.equals(ALLOW_RESOURCE);
    }

    public static String getShieldsValue(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            settings = PresearchShieldsContentSettingsJni.get().getFingerprintingControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            settings = PresearchShieldsContentSettingsJni.get().getCookieControlType(host, profile);
        }
        return settings;
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativePresearchShieldsContentSettings == 0;
        mNativePresearchShieldsContentSettings = nativePtr;
    }

    @CalledByNative
    private void blockedEvent(int tabId, String block_type, String subresource) {
        for (PresearchShieldsContentSettingsObserver observer : mPresearchShieldsContentSettingsObservers) {
            observer.blockEvent(tabId, block_type, subresource);
        }
    }

    @CalledByNative
    private void savedBandwidth(long savings) {
        for (PresearchShieldsContentSettingsObserver observer : mPresearchShieldsContentSettingsObservers) {
            observer.savedBandwidth(savings);
        }
    }

    @NativeMethods
    interface Natives {
        void init(PresearchShieldsContentSettings self);
        void destroy(long nativePresearchShieldsContentSettings);

        void setPresearchShieldsEnabled(boolean enabled, String url, Profile profile);
        boolean getPresearchShieldsEnabled(String url, Profile profile);
        void setAdControlType(String type, String url, Profile profile);
        String getAdControlType(String url, Profile profile);
        void setCookieControlType(String type, String url, Profile profile);
        String getCookieControlType(String url, Profile profile);
        void setFingerprintingControlType(String type, String url, Profile profile);
        String getFingerprintingControlType(String url, Profile profile);
        void setHTTPSEverywhereEnabled(boolean enabled, String url, Profile profile);
        boolean getHTTPSEverywhereEnabled(String url, Profile profile);
        void setNoScriptControlType(String type, String url, Profile profile);
        String getNoScriptControlType(String url, Profile profile);
    }
}
