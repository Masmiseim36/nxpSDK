/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class WiFiSecurity {
    private static final String TAG = "AWS: WiFi Security";

    public enum Type {
        /** 0. Open - No Security */
        OPEN,

        /** 1. WEP Security */
        WEP,

        /** 2. WPA Security */
        WPA,

        /** 3. WPA2 Security */
        WPA2
    }

    private byte[] payload;

    /**
     * Create payload from WiFi SSID, password and security type.
     * Payload: 1B int security type, SSID + \0, password + \0
     * @param securityType Security type as a string
     * @param ssid WiFi SSID
     * @param password WiFi password
     */
    public WiFiSecurity(String securityType, String ssid, String password) {
        try {
            // WiFi security type
            int typeNum = Type.OPEN.ordinal();
            try {
                typeNum = Type.valueOf(securityType).ordinal();
            } catch(IllegalArgumentException e) {
                Log.e(TAG, "Error during parsing WiFi security type.", e);
            }
            byte[] typeId = ByteBuffer.allocate(1).put((byte) typeNum).array();

            // SSID
            ssid += "\0";
            byte[] ssidArr = ssid.getBytes(StandardCharsets.UTF_8);

            // password
            byte[] passwordArr;
            if (password != null && password.length() > 0) {
                password += "\0";
                passwordArr = password.getBytes(StandardCharsets.UTF_8);
            } else {
                 passwordArr = new byte[0];
            }

            ByteArrayOutputStream outputStream = new ByteArrayOutputStream(typeId.length + ssidArr.length + passwordArr.length);
            outputStream.write(typeId);
            outputStream.write(ssidArr);
            if (passwordArr.length > 0) {
                outputStream.write(passwordArr);
            }
            this.payload = outputStream.toByteArray();
        } catch (IOException e) {
            this.payload = new byte[0];
        } catch (BufferOverflowException e) {
            Log.e(TAG, "Error during allocating header byte array.", e);
        }
    }

    public byte[] getPayload() {
        return payload;
    }

    public static List<String> getListOfWiFiSecurityTypes() {
        List<String> list = new ArrayList<>(Type.values().length);
        for (Type value : Type.values()) {
            list.add(value.toString());
        }
        return list;
    }
}
