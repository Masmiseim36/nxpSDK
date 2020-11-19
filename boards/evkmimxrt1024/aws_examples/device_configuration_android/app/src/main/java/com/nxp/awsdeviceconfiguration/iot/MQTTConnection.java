/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

import com.amazonaws.AmazonClientException;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttManager;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;

import android.content.Context;
import android.util.Log;

import java.util.UUID;

public class MQTTConnection extends AwsConnection {

    public static final String TAG = "AWS: MQTT connection";

    private AWSIotMqttManager mqttManager;

    public MQTTConnection(Context appContext, AwsConstants awsConstants) {
        super(appContext, awsConstants);

        Log.d(TAG, String.format("Cognito pool ID: %s, region: %s, endpoint: %s", awsConstants.getCognitoPoolId(), awsConstants.getMyRegion(), awsConstants.getCustomerSpecificEndpoint()));
    }

    /**
     * Connect to AWS IoT via MQTT client.
     * @param callback User callback called when MQTT client status has changed.
     */
    public boolean connect(AWSIotMqttClientStatusCallback callback) {
        try {
            if (mqttManager == null) {
                // Create MQTT Client
                String clientId = UUID.randomUUID().toString(); // MQTT client IDs are required to be unique per AWS IoT account
                mqttManager = new AWSIotMqttManager(clientId, awsConstants.getCustomerSpecificEndpoint());
            }
            mqttManager.connect(credentialsProvider, callback);
        } catch (final Exception e) {
            Log.e(TAG, "MQTTConnection error.", e);
            return false;
        }

        return true;
    }

    /**
     * Disconnect from AWS IoT via MQTT client.
     */
    public boolean disconnect() {
        try {
            return mqttManager.disconnect();
        } catch (Exception e) {
            Log.e(TAG, "Disconnect error.", e);
        }

        return false;
    }

    /**
     * Publish message to selected topic to AWS IoT device shadow via MQTT client.
     * @param topic AWS IoT topic to publish message into.
     * @param message Message either in form of string or as JSON formatted device shadow.
     */
    public void publish(final String topic, final String message) {
        try {
            mqttManager.publishString(message, topic, AwsConstants.QOS, (status, userData) -> {
                switch (status) {
                    case Success:
                        Log.d(TAG, String.format("Message \"%s\" for topic \"%s\" has been delivered successfully.", message, topic));
                        break;
                    case Fail:
                        Log.e(TAG, String.format("Message \"%s\" for topic \"%s\" delivery failure.", message, topic));
                }
            }, null);
        } catch (Exception e) {
            Log.e(TAG, "Publish error.", e);
        }
    }

    /**
     * Subscribe to selected topic of AWS IoT via MQTT client.
     * @param topic AWS IoT topic to receive messages from.
     * @param callback User callback for handling received message from selected topic.
     */
    public void subscribe(String topic, AWSIotMqttNewMessageCallback callback) {
        try {
            mqttManager.subscribeToTopic(topic, AwsConstants.QOS, callback);
            Log.d(TAG, String.format("Successfully subscribed for topic \"%s\".", topic));
        } catch (AmazonClientException e) {
            Log.e(TAG, "Cannot subscribe to topic.", e);
        }
    }
}
