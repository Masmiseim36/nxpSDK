/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.activities.device;

import android.app.Application;
import android.arch.lifecycle.AndroidViewModel;
import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.net.nsd.NsdServiceInfo;
import android.support.annotation.NonNull;
import android.util.Log;
import android.widget.Toast;

import com.nxp.awsdeviceconfiguration.nsd.DeviceInfo;
import com.nxp.awsdeviceconfiguration.nsd.INetworkServiceDiscoveryListener;
import com.nxp.awsdeviceconfiguration.nsd.NetworkServiceDiscovery;

import java.util.ArrayList;
import java.util.List;

public class DevicesListViewModel extends AndroidViewModel {
    private String TAG = "AWS: Devices List ViewModel";

    private Application appContext;
    private NetworkServiceDiscovery nsd;

    // live devices list data
    private MutableLiveData<List<DeviceInfo>> devicesLiveData;

    // selected device to share between fragments
    private final MutableLiveData<DeviceInfo> selectedDevice = new MutableLiveData<>();

    public DevicesListViewModel(@NonNull Application application) {
        super(application);
        appContext = application;
    }

    LiveData<List<DeviceInfo>> getDevicesList() {
        if (devicesLiveData == null) {
            devicesLiveData = new MutableLiveData<>();

            List<DeviceInfo> discoveredDevicesList = new ArrayList<>();

            // start NSD
            nsd = new NetworkServiceDiscovery(appContext, new INetworkServiceDiscoveryListener() {
                @Override
                public void onDeviceDiscoveryStarted() {
                    discoveredDevicesList.clear();

                    Toast.makeText(appContext, "Network Service Discovery has started.", Toast.LENGTH_SHORT).show();
                }

                @Override
                public void onDeviceDiscoveredSuccess(NsdServiceInfo info) {
                    // add device to devices list
                    final DeviceInfo deviceInfo = new DeviceInfo(info);
                    if (discoveredDevicesList.contains(deviceInfo)) {
                        Log.d(TAG, "Device already discovered: " + deviceInfo);
                    } else {
                        discoveredDevicesList.add(deviceInfo);

                        Log.d(TAG, "Discovered device: " + deviceInfo);

                        // post data to list view's live data
                        devicesLiveData.postValue(discoveredDevicesList);
                    }
                }

                @Override
                public void onError(String errMsg) {
                    Log.d(TAG, "Error during Network Service Discovery: " + errMsg);
                }

                @Override
                public void onTimeout(boolean foundSomeDevices) {
                    if (!foundSomeDevices) {
                        // no device has been found
                        devicesLiveData.postValue(null);
                    }
                }
            });
            nsd.startDiscovery();
        }
        return devicesLiveData;
    }

    void refreshDeviceList() {
        Log.d(TAG, "Refresh device list...");
        if (nsd != null) {
            nsd.stopDiscovery(true);
        }
    }

    void stopDiscovery() {
        Log.d(TAG, "Want to stop network service discovery");
        if (nsd != null) {
            nsd.stopDiscovery(false);
        }
    }

    public void select(DeviceInfo item) {
        selectedDevice.setValue(item);
    }

    public LiveData<DeviceInfo> getSelected() {
        return selectedDevice;
    }

    @Override
    protected void onCleared() {
        super.onCleared();
        Log.d(TAG, "on cleared called");
    }

}
