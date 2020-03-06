/*
 * Copyright 2017 NXP
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
package com.nxp.awsremotecontrol.iot;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.Uri;
import android.util.Log;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttQos;
import com.amazonaws.regions.Regions;
import com.nxp.awsremotecontrol.mqtt.Connection;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class AwsConstants {

    Context context;
    SharedPreferences preferences;

    /** Amazon AWS customer specific endpoint */
    public static final String CUSTOMER_SPECIFIC_ENDPOINT_KEY = "customer_specific_endpoint";

    /** Amazon Cognito user pool ID */
    public static final String COGNITO_POOL_ID_KEY = "cognito_pool_id";

    /** Name of AWS IoT user defined thing name */
    public static final String THING_NAME_KEY = "thing_name";

    /** Region of AWS IoT */
    public static final String MY_REGION_KEY = "region";


    /** Main topic for publishing used by AWS IoT device shadow */
    public String SHADOW_UPDATE;

    /** All accepted changes in main topic used by AWS IoT device shadow */
    public String SHADOW_UPDATE_ACCEPTED;

    /** Get AWS IoT device shadow */
    public String SHADOW_GET;

    /** All accepted get requests by AWS IoT device shadow */
    public String SHADOW_GET_ACCEPTED;


    /** Quality of service */
    public static final AWSIotMqttQos QOS = AWSIotMqttQos.QOS0;

    /** Empty message for requesting data from device shadow */
    public static final String EMPTY_MESSAGE = "";

    /** Timeout interval in miliseconds for message to arrive after publish */
    public static final int TIMEOUT = 7000;

    /** Timeout interval in miliseconds for establishing MQTT connection */
    public static final int CONNECT_TIMEOUT = 4000;


    public AwsConstants(Context context, String preferenceFileKey) {
        this.context = context;
        this.preferences = context.getSharedPreferences(preferenceFileKey, Context.MODE_PRIVATE);
    }

    /**
     * Check all preferences keys if they are valid
     * @return True if all preferences are valid
     */
    public boolean checkPreferencesAndValidate() {
        return checkKey(CUSTOMER_SPECIFIC_ENDPOINT_KEY) &&
                checkKey(COGNITO_POOL_ID_KEY) &&
                checkKey(MY_REGION_KEY) &&
                checkKey(THING_NAME_KEY);
    }

    /**
     * Check key from preferences
     * @param key Which key to check
     * @return True if key is not null and not empty
     */
    private boolean checkKey(String key) {
        final String value = preferences.getString(key, null);
        Log.d(Connection.LOG_TAG, String.format("check key %s=%s (%b)", key, value, value != null));
        return value != null && !value.trim().isEmpty();
    }

    /**
     * Get Customer specific endpoint from preferences
     * @return String with Customer specific endpoint
     */
    public String getCustomerSpecificEndpoint() {
        return preferences.getString(CUSTOMER_SPECIFIC_ENDPOINT_KEY, null);
    }

    /**
     * Get Cognito Pool ID from preferences
     * @return String with Cognito Pool ID
     */
    public String getCognitoPoolId() {
        return preferences.getString(COGNITO_POOL_ID_KEY, null);
    }

    /**
     * Get Region from preferences
     * @return Selected region
     */
    public Regions getMyRegion() {
        return Regions.fromName(preferences.getString(MY_REGION_KEY, "us-west-2"));
    }

    /**
     * Get Thing name from preferences
     * @return Thing name
     */
    public String getThingName() {
        return preferences.getString(THING_NAME_KEY, null);
    }

    /**
     * Refresh all shadow's URLs.
     * To be called after updating thing name.
     */
    public void refreshShadowUrls() {
        // fill AWS topics with loaded thing name
        final String thingName = preferences.getString(THING_NAME_KEY, "MyTestDemo");
        SHADOW_UPDATE = "$aws/things/" + thingName + "/shadow/update";
        SHADOW_UPDATE_ACCEPTED = SHADOW_UPDATE + "/accepted";
        SHADOW_GET = "$aws/things/" + thingName + "/shadow/get";
        SHADOW_GET_ACCEPTED = "$aws/things/" + thingName + "/shadow/get/accepted";
    }

    /**
     * Load preferences from .properties file and save them into shared preferences
     * @param uri Path to .properties file
     * @param listener Preferences listener to be notified after load ends in success or error
     */
    public void loadPreferencesFromFile(Uri uri, IPreferencesListener listener) {
        Log.d(Connection.LOG_TAG, "Loading preferences from file: " + uri +" , "  + uri.getPath());

        try {
            // load and parse file
            InputStream fis = context.getContentResolver().openInputStream(uri);
            Properties properties = new Properties();
            properties.load(fis);

            // save loaded values into shared preferences
            Editor editor = preferences.edit();

            editor.putString(CUSTOMER_SPECIFIC_ENDPOINT_KEY, properties.getProperty(CUSTOMER_SPECIFIC_ENDPOINT_KEY));
            editor.putString(COGNITO_POOL_ID_KEY, properties.getProperty(COGNITO_POOL_ID_KEY));
            editor.putString(THING_NAME_KEY, properties.getProperty(THING_NAME_KEY));

            // check region
            final String region = properties.getProperty(MY_REGION_KEY);
            try {
                editor.putString(MY_REGION_KEY, Regions.fromName(region).getName());

                if (!properties.getProperty(CUSTOMER_SPECIFIC_ENDPOINT_KEY).contains(region) && !properties.getProperty(COGNITO_POOL_ID_KEY).contains(region)) {
                    final String errMsg = String.format("Customer specific endpoint and/or Cognito pool ID have different region than defined \"%s\".", region);
                    Log.e(Connection.LOG_TAG, errMsg);
                    listener.onPreferencesLoadError(errMsg);
                    return;
                }
            } catch (IllegalArgumentException e) {
                final String errMsg = String.format("Region \"%s\" is not valid.", region);
                Log.e(Connection.LOG_TAG, errMsg, e);
                listener.onPreferencesLoadError(errMsg);
                return;
            }

            editor.apply();

            // refresh shadow's URLs with loaded values
            refreshShadowUrls();

            // notify listener
            if (checkPreferencesAndValidate()) {
                listener.onPreferencesLoadSuccess();
            } else {
                listener.onPreferencesLoadError("Preferences are missing or not valid.");
            }
        } catch (IOException e) {
            final String errMsg = String.format("Cannot read preferences file: \"%s\"", uri);
            Log.e(Connection.LOG_TAG, errMsg, e);
            listener.onPreferencesLoadError(errMsg);
        } catch (IllegalArgumentException e) {
            final String errMsg = "Wrong file format selected.";
            Log.e(Connection.LOG_TAG, errMsg, e);
            listener.onPreferencesLoadError(errMsg);
        }
    }
}
