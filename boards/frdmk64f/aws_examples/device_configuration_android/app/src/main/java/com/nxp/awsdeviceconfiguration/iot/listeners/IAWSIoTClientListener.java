/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot.listeners;

import com.nxp.awsdeviceconfiguration.iot.AWSResult;

public interface IAWSIoTClientListener {
    void onSuccess(AWSResult result);
    void onError(String errMsg);
}
