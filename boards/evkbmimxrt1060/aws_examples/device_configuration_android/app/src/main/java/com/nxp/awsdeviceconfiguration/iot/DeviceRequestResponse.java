/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

public enum DeviceRequestResponse {
    /** Request has been successful */
    OK,

    /** Request has failed */
    FAIL,

    /** Device does not know this request */
    UNKNOWN_CMD,

    /** SSL connection has failed */
    SSL_ERROR,

    /** User has cancelled request */
    CANCELLED;

    private String status;
    private String payload;

    public DeviceRequestResponse setReason(String status) {
        this.status = status;
        return this;
    }

    public void setPayload(String payload) {
        this.payload = payload;
    }

    public String getReason() {
        return (status == null ? "Unknown" : status);
    }

    public String getPayload() {
        return payload;
    }
}