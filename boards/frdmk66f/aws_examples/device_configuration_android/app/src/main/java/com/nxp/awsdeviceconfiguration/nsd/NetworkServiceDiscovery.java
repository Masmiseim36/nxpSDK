/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.nsd;

import android.content.Context;
import android.net.nsd.NsdManager;
import android.net.nsd.NsdServiceInfo;
import android.util.Log;

import com.nxp.awsdeviceconfiguration.iot.AwsConfig;
import com.nxp.awsdeviceconfiguration.iot.AwsConstants;

import android.os.Handler;

import java.util.Locale;

public class NetworkServiceDiscovery {

    private static final String TAG = "AWS: NSD";

    enum DiscoveryStatus {
        RUNNING,
        STOPPED
    }

    private Handler timeoutHandler;
    private Runnable timeoutCallback;
    private boolean requestRefresh;
    private int numOfResolvedDevices;
    private int numOfResolveRetries;

    private final int DEFAULT_NUM_OF_RESOLVE_RETRIES = 10;

    private NsdManager nsdManager;
    private NsdManager.DiscoveryListener discoveryListener;
    private DiscoveryStatus discoveryStatus;

    public class AwsNsdResolveListener implements NsdManager.ResolveListener {
        INetworkServiceDiscoveryListener callback;

        AwsNsdResolveListener(final INetworkServiceDiscoveryListener callback) {
            this.callback = callback;
        }

        @Override
        public void onResolveFailed(NsdServiceInfo serviceInfo, int errorCode) {
            String reason;
            switch (errorCode) {
                case NsdManager.FAILURE_ALREADY_ACTIVE:
                    reason = "resolve already active";
                    if ((--numOfResolveRetries) > 0) {
                        reason += String.format(Locale.US, ", trying again (remaining tries: #%d)", numOfResolveRetries);
                        nsdManager.resolveService(serviceInfo, new AwsNsdResolveListener(callback));
                    }
                    break;
                case NsdManager.FAILURE_INTERNAL_ERROR:
                    reason = "internal error";
                    break;
                case NsdManager.FAILURE_MAX_LIMIT:
                    reason = "max limit of request has been reached";
                    break;
                default:
                    reason = "unknown";
            }
            callback.onError("Resolve failed: " + serviceInfo.getServiceName() + ", reason: " + reason);
        }

        @Override
        public void onServiceResolved(NsdServiceInfo serviceInfo) {
            // reset timeout after successful resolve
            timeoutHandler.removeCallbacks(timeoutCallback);
            timeoutHandler.postDelayed(timeoutCallback, AwsConstants.NSD_TIMEOUT);

            // increase counter of resolved devices
            numOfResolvedDevices++;

            // send info about resolved device back
            Log.d(TAG, "Service resolve success: " + serviceInfo);
            callback.onDeviceDiscoveredSuccess(serviceInfo);
        }
    }

    public NetworkServiceDiscovery(Context ctx, final INetworkServiceDiscoveryListener callback) {
        // create NSD manager
        nsdManager = (NsdManager) ctx.getSystemService(Context.NSD_SERVICE);

        numOfResolvedDevices = 0;

        // initialize timeout
        timeoutHandler = new Handler();
        timeoutCallback = () -> {
            // stop NSD discovery service
            stopDiscovery(false);

            // call timeout callback
            callback.onTimeout(numOfResolvedDevices > 0);
            Log.d(TAG, String.format("Service discovery has timed out (%ds) and found #%d devices.", (AwsConstants.NSD_TIMEOUT/1000), numOfResolvedDevices));

            numOfResolvedDevices = 0;
        };

        // create discovery listener
        discoveryStatus = DiscoveryStatus.STOPPED;
        discoveryListener = new NsdManager.DiscoveryListener() {
            @Override
            public void onDiscoveryStarted(String regType) {
                Log.d(TAG, "Service discovery started: " + regType);
                discoveryStatus = DiscoveryStatus.RUNNING;
                requestRefresh = false;
                numOfResolveRetries = DEFAULT_NUM_OF_RESOLVE_RETRIES;

                callback.onDeviceDiscoveryStarted();
            }

            public void onServiceFound(NsdServiceInfo serviceInfo) {
                Log.d(TAG, "Service discovery success: " + serviceInfo.getServiceName());

                if (!serviceInfo.getServiceType().equals(AwsConfig.MDNS_SERVICE_TYPE)) {
                    // ERR service type is different
                    callback.onError("Unknown Service Type: " + serviceInfo.getServiceType());
                } else {
                    // AWS IOT device found
                    nsdManager.resolveService(serviceInfo, new AwsNsdResolveListener(callback));
                }
            }

            @Override
            public void onServiceLost(NsdServiceInfo service) {
                Log.e(TAG, "Service lost: " + service.getServiceName());
            }

            @Override
            public void onDiscoveryStopped(String serviceType) {
                Log.d(TAG, "Discovery stopped: " + serviceType);
                discoveryStatus = DiscoveryStatus.STOPPED;
                timeoutHandler.removeCallbacks(timeoutCallback);
                numOfResolvedDevices = 0;

                // if refresh has been requested, start discovery again
                if (requestRefresh) {
                    Log.d(TAG, "Starting discovery again");
                    startDiscovery();
                }
            }

            @Override
            public void onStartDiscoveryFailed(String serviceType, int errorCode) {
                Log.e(TAG, "Discovery failed: Error code:" + errorCode);
                nsdManager.stopServiceDiscovery(this);
            }

            @Override
            public void onStopDiscoveryFailed(String serviceType, int errorCode) {
                Log.e(TAG, "Discovery failed: Error code:" + errorCode);
                nsdManager.stopServiceDiscovery(this);
            }
        };
    }

    public void startDiscovery() {
        try {
            // start discovery of AWS mDNS
            nsdManager.discoverServices(AwsConfig.MDNS_SERVICE_TYPE, NsdManager.PROTOCOL_DNS_SD, discoveryListener);

            // start timeout
            timeoutHandler.postDelayed(timeoutCallback, AwsConstants.NSD_TIMEOUT);
        } catch(IllegalArgumentException e) {
            Log.e(TAG, "Could not start NSD discovery!", e);
        }
    }

    public void stopDiscovery(boolean triggerRefresh) {
        Log.d(TAG, String.format("Stopping discovery %s triggering refresh.", (triggerRefresh ? "with" : "without")));

        switch (discoveryStatus) {
            case RUNNING:
                // set flag for refresh
                requestRefresh = triggerRefresh;
                try {
                    nsdManager.stopServiceDiscovery(discoveryListener);
                } catch (IllegalArgumentException e) {
                    Log.e(TAG, "NSD discovery is already stopped!", e);
                }
                break;

            case STOPPED:
                if (triggerRefresh) {
                    startDiscovery();
                }
                break;
        }
    }

}
