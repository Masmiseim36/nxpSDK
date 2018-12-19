/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.nxp.awsremotecontrol.mqtt;

import com.amazonaws.AmazonClientException;
import com.amazonaws.auth.CognitoCachingCredentialsProvider;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttManager;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttMessageDeliveryCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback.AWSIotMqttClientStatus;
import com.amazonaws.regions.Region;
import android.content.Context;
import android.util.Log;
import com.nxp.awsremotecontrol.iot.AwsConstants;
import java.util.UUID;

public class Connection {

    public static final String LOG_TAG = Connection.class.getCanonicalName();

    private AWSIotMqttManager mqttManager;
    private AwsConstants awsConstants;
    private CognitoCachingCredentialsProvider credentialsProvider;

    public Connection(Context appContext, AwsConstants awsConstants) {
        Log.d(LOG_TAG, String.format("Cognito pool ID: %s, region: %s, endpoint: %s", awsConstants.getCognitoPoolId(), awsConstants.getMyRegion(), awsConstants.getCustomerSpecificEndpoint()));

        // Initialize the AWS Cognito credentials provider
        credentialsProvider = new CognitoCachingCredentialsProvider(
            appContext,
            awsConstants.getCognitoPoolId(),
            awsConstants.getMyRegion()
        );

        this.awsConstants = awsConstants;
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
            Log.e(LOG_TAG, "Connection error.", e);
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
            Log.e(LOG_TAG, "Disconnect error.", e);
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
            mqttManager.publishString(message, topic, AwsConstants.QOS, new AWSIotMqttMessageDeliveryCallback() {
                @Override
                public void statusChanged(MessageDeliveryStatus status, Object userData) {
                    switch (status) {
                        case Success:
                            Log.d(LOG_TAG, String.format("Message \"%s\" for topic \"%s\" has been delivered successfully.", message, topic));
                            break;
                        case Fail:
                            Log.e(LOG_TAG, String.format("Message \"%s\" for topic \"%s\" delivery failure.", message, topic));
                    }
                }
            }, null);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
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
            Log.d(LOG_TAG, String.format("Successfully subscribed for topic \"%s\".", topic));
        } catch (AmazonClientException e) {
            Log.e(LOG_TAG, "Cannot subscribe to topic.", e);
        }
    }
}
