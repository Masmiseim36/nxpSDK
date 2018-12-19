/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

public abstract class AWSResult {
    private String exception;

    AWSResult(String exception) {
        this.exception = exception;
    }

    String getException() {
        return exception;
    }
}
