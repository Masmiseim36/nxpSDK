/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

/**
 * Request command to be send to device.
 * - ID of command is enum ordinal.
 * - ID has to be same as in device part of example.
 * - Some of commands has to have a payload.
 */
public enum DeviceRequestCommand {
    /** 0. Log in the board */
    LOGIN(Type.REPLY),

    /** 1. Set login password */
    SET_LOGIN_PASSWORD(Type.SET),

    /** 2. Change mDNS hostname */
    SET_MDNS_HOSTNAME(Type.SET),

    /** 3. Reset the board */
    BOARD_RESET(Type.NOREPLY),

    /** 4.-5. Set AWS certificates */
    SET_CERTIFICATE(Type.SET),
    SET_PRIVATE_KEY(Type.SET),

    /** 6. Set AWS thing name */
    SET_AWS_THING_NAME(Type.SET),

    /** 7. Set AWS endpoint */
    SET_AWS_ENDPOINT(Type.SET),

    /** 8. Wifi settings */
    SET_WIFI_NETWORK(Type.SET),

    /** 9. Get device information */
    GET_DEVICE_INFO(Type.GET);

    enum Type {
        REPLY, NOREPLY, GET, SET
    }
    private Type type;

    DeviceRequestCommand(Type type) {
        this.type = type;
    }

    public boolean isGet() {
        return type == Type.GET;
    }
    public boolean isNoReply() {
        return type == Type.NOREPLY;
    }
}