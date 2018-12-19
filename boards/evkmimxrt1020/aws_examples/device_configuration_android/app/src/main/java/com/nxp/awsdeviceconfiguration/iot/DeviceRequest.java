/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

import android.support.annotation.NonNull;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

/**
 * Encode request command and payload to be send as byte array to device.
 * <br/>
 * Structure of header message:<br/>
 * - 2 bytes: request command ID<br/>
 * - 2 bytes: length of payload<br/><br/>
 * Structure of payload message:<br/>
 * - raw payload payload
 */
public class DeviceRequest {
    private static final String TAG = "AWS: Device Request";

    private DeviceRequestCommand command;
    private byte[] payload;

    public DeviceRequest(DeviceRequestCommand command, String payload) {
        if (!command.isNoReply() && payload == null) {
            Log.e(TAG, String.format("Request command %s has to have a payload!", command.toString()));
        }

        this.command = command;
        this.payload = preparePayload((payload != null ? payload.getBytes(StandardCharsets.UTF_8) : new byte[0]));
    }

    public DeviceRequest(DeviceRequestCommand command, @NonNull byte[] payload) {
        this.command = command;
        this.payload = preparePayload(payload);
    }

    public DeviceRequest(DeviceRequestCommand command) {
        this.command = command;
        this.payload = preparePayload(new byte[0]);
    }

    private byte[] preparePayload(byte[] payloadArr) {
        try {
            byte[] commandId = ByteBuffer.allocate(Short.SIZE / Byte.SIZE).putShort((short) command.ordinal()).array();
            byte[] payloadLength = ByteBuffer.allocate(Short.SIZE / Byte.SIZE).putShort((short)payloadArr.length).array();

            ByteArrayOutputStream outputStream = new ByteArrayOutputStream(commandId.length + payloadLength.length + payloadArr.length);
            outputStream.write(commandId);
            outputStream.write(payloadLength);
            if (payloadArr.length > 0) {
                outputStream.write(payloadArr);
            }
            return outputStream.toByteArray();
        } catch (IOException e) {
            Log.e(TAG, "Error during preparing payload byte array.", e);
        } catch (BufferOverflowException e) {
            Log.e(TAG, "Error during allocating byte array.", e);
        }

        return new byte[0];
    }


    byte[] getPayload() {
        return payload;
    }

    DeviceRequestCommand getCommand() {
        return command;
    }
}
