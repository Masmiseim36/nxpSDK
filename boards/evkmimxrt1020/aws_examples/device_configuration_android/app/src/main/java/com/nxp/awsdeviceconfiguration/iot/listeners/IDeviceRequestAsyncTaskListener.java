/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot.listeners;

import com.nxp.awsdeviceconfiguration.iot.DeviceRequestCommand;

public interface IDeviceRequestAsyncTaskListener {
    void onRequestStart(DeviceRequestCommand command);
    void onDone(String payload);
    void onError(String errMsg);
}
