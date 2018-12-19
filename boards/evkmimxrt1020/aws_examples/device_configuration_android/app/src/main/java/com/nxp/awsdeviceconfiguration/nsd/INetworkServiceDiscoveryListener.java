/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.nsd;

import android.net.nsd.NsdServiceInfo;

public interface INetworkServiceDiscoveryListener {
    /** Notify that device discovery has started */
    void onDeviceDiscoveryStarted();

    /** Notify that device discovery has found one device
     * @param info Information about found device
     */
    void onDeviceDiscoveredSuccess(NsdServiceInfo info);

    /** Notify that device discovery has encountered some error */
    void onError(String errMsg);

    /** Notify that device discovery has timed out
     * @param foundSomeDevices True if discovery timed out but found some devices
     */
    void onTimeout(boolean foundSomeDevices);
}
