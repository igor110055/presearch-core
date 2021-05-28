/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.sync;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

@JNINamespace("chrome::android")
public class PresearchSyncDevices {
    public static final String TAG = "SYNC";
    private long mNativePresearchSyncDevicesAndroid;

    private static PresearchSyncDevices sPresearchSyncDevices;
    private static boolean sInitialized;

    public static PresearchSyncDevices get() {
        ThreadUtils.assertOnUiThread();
        if (!sInitialized) {
            sPresearchSyncDevices = new PresearchSyncDevices();
            sInitialized = true;
        }
        return sPresearchSyncDevices;
    }

    public PresearchSyncDevices() {
        Init();
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativePresearchSyncDevicesAndroid == 0;
        mNativePresearchSyncDevicesAndroid = nativePtr;
    }

    private void Init() {
        if (mNativePresearchSyncDevicesAndroid == 0) {
            PresearchSyncDevicesJni.get().init(PresearchSyncDevices.this);
        }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativePresearchSyncDevicesAndroid != 0) {
            PresearchSyncDevicesJni.get().destroy(mNativePresearchSyncDevicesAndroid);
            mNativePresearchSyncDevicesAndroid = 0;
        }
    }

    /**
     * Listener for the devices syncchain changes.
     */
    public interface DeviceInfoChangedListener {
        // Invoked when the device info has changed.
        public void deviceInfoChanged();
    }

    // Sync state changes more often than listeners are added/removed, so using CopyOnWrite.
    private final List<DeviceInfoChangedListener> mDeviceInfoListeners =
            new CopyOnWriteArrayList<DeviceInfoChangedListener>();

    public void addDeviceInfoChangedListener(DeviceInfoChangedListener listener) {
        ThreadUtils.assertOnUiThread();
        mDeviceInfoListeners.add(listener);
    }

    public void removeDeviceInfoChangedListener(DeviceInfoChangedListener listener) {
        ThreadUtils.assertOnUiThread();
        mDeviceInfoListeners.remove(listener);
    }

    /**
     * Called when the state of the native sync engine has changed, so various
     * UI elements can update themselves.
     */
    @CalledByNative
    protected void deviceInfoChanged() {
        for (DeviceInfoChangedListener listener : mDeviceInfoListeners) {
            listener.deviceInfoChanged();
        }
    }

    public class SyncDeviceInfo {
        public String mName;
        public boolean mIsCurrentDevice;
        public boolean mSupportsSelfDelete;
        public String mType;
        public Date mLastUpdatedTimestamp;
        public String mGuid;
    }

    public ArrayList<SyncDeviceInfo> GetSyncDeviceList() {
        ArrayList<SyncDeviceInfo> deviceList = new ArrayList<SyncDeviceInfo>();
        String json =
                PresearchSyncDevicesJni.get().getSyncDeviceListJson(mNativePresearchSyncDevicesAndroid);
        // Add root element to make it real JSON, otherwise getJSONArray cannot parse it
        json = "{\"devices\":" + json + "}";
        try {
            JSONObject result = new JSONObject(json);
            JSONArray devices = result.getJSONArray("devices");
            Log.i(TAG, "GetSyncDeviceList devices length is " + devices.length());
            for (int i = 0; i < devices.length(); i++) {
                SyncDeviceInfo deviceInfo = new SyncDeviceInfo();
                JSONObject device = devices.getJSONObject(i);
                deviceInfo.mName = device.getString("name");
                deviceInfo.mIsCurrentDevice = device.getBoolean("isCurrentDevice");
                deviceInfo.mType = device.getString("type");
                long lastUpdatedTimestamp = device.getLong("lastUpdatedTimestamp");
                deviceInfo.mLastUpdatedTimestamp = new Date(lastUpdatedTimestamp);
                deviceInfo.mGuid = device.getString("guid");
                deviceInfo.mSupportsSelfDelete = device.getBoolean("supportsSelfDelete");
                deviceList.add(deviceInfo);
            }
        } catch (JSONException e) {
            Log.e(TAG, "GetSyncDeviceList JSONException error " + e);
        } catch (IllegalStateException e) {
            Log.e(TAG, "GetSyncDeviceList IllegalStateException error " + e);
        }
        return deviceList;
    }

    public void DeleteDevice(String deviceGuid) {
        PresearchSyncDevicesJni.get().deleteDevice(mNativePresearchSyncDevicesAndroid, deviceGuid);
    }

    @NativeMethods
    interface Natives {
        void init(PresearchSyncDevices caller);
        void destroy(long nativePresearchSyncDevicesAndroid);

        String getSyncDeviceListJson(long nativePresearchSyncDevicesAndroid);
        void deleteDevice(long nativePresearchSyncDevicesAndroid, String deviceGuid);
    }
}
