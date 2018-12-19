/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.Uri;
import android.util.Log;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttQos;
import com.amazonaws.regions.Regions;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

public class AwsConstants {
    private static final String TAG = "AWS CONSTANTS";

    private SharedPreferences preferences;

    public interface IPreferencesListener {
        void onPreferencesLoadSuccess();
        void onPreferencesLoadError(String errMsg);
    }

    /** Amazon AWS customer specific endpoint */
    private static final String CUSTOMER_SPECIFIC_ENDPOINT_KEY = "customer_specific_endpoint";

    /** Amazon Cognito user pool ID */
    private static final String COGNITO_POOL_ID_KEY = "cognito_pool_id";

    /** Name of AWS IoT user defined thing name */
    private static final String THING_NAME_KEY = "thing_name";

    /** Region of AWS IoT */
    private static final String MY_REGION_KEY = "region";


    /** All accepted changes in main topic used by AWS IoT device shadow */
    public String SHADOW_UPDATE_ACCEPTED;

    /** Get AWS IoT device shadow */
    public String SHADOW_GET;

    /** All accepted get requests by AWS IoT device shadow */
    public String SHADOW_GET_ACCEPTED;


    /** Quality of service */
    static final AWSIotMqttQos QOS = AWSIotMqttQos.QOS0;

    /** Empty message for requesting data from device shadow */
    public static final String EMPTY_MESSAGE = "";

    /** Timeout interval in miliseconds for message to arrive after publish */
    public static final int TIMEOUT = 10000;

    /** Timeout interval in miliseconds for network service discovery */
    public static final int NSD_TIMEOUT = 15000;

    /** Timeout interval in miliseconds for SSL connection */
    static final int SSL_CONNECTION_TIMEOUT = 30000;

    /** Timeout interval in miliseconds for establishing MQTT connection */
    public static final int CONNECT_TIMEOUT = 4000;


    /** SSL connection reader buffer size */
    public static final int READER_BUFFER_SIZE = 128;


    public AwsConstants(Context context) {
        this.preferences = context.getSharedPreferences(AwsConfig.PREFERENCES_KEY, Context.MODE_PRIVATE);
    }

    /**
     * Check all preferences keys if they are valid
     * @return True if all preferences are valid
     */
    public boolean checkPreferencesAndValidate() {
        return checkKey(CUSTOMER_SPECIFIC_ENDPOINT_KEY) &&
                checkKey(COGNITO_POOL_ID_KEY) &&
                checkKey(MY_REGION_KEY);
    }

    /**
     * Check key from preferences
     * @param key Which key to check
     * @return True if key is not null and not empty
     */
    private boolean checkKey(String key) {
        final String value = preferences.getString(key, null);
        return value != null && !value.trim().isEmpty();
    }

    /**
     * Refresh all shadow's URLs.
     * To be called after updating thing name.
     */
    public void refreshShadowUrls() {
        // fill AWS topics with loaded thing name
        final String thingName = preferences.getString(THING_NAME_KEY, "MyTestDemo");
        SHADOW_UPDATE_ACCEPTED = "$aws/things/" + thingName + "/shadow/update/accepted";
        SHADOW_GET = "$aws/things/" + thingName + "/shadow/get";
        SHADOW_GET_ACCEPTED = "$aws/things/" + thingName + "/shadow/get/accepted";
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
     * Set Thing name
     * @param name thing name
     */
    public void setThingName(String name) {
        preferences.edit().putString(THING_NAME_KEY, name).apply();
    }

    /**
     * Load preferences from .properties file and save them into shared preferences
     * @param uri Path to .properties file
     * @param listener Preferences listener to be notified after load ends in success or error
     */
    public void loadPreferencesFromFile(Context context, Uri uri, IPreferencesListener listener) {
        Log.d(TAG, "Loading preferences from file: " + uri +" , "  + uri.getPath());

        try {
            // load and parse file
            InputStream fis = context.getContentResolver().openInputStream(uri);
            Properties properties = new Properties();
            properties.load(fis);

            // save loaded values into shared preferences
            Editor editor = preferences.edit();

            editor.putString(CUSTOMER_SPECIFIC_ENDPOINT_KEY, properties.getProperty(CUSTOMER_SPECIFIC_ENDPOINT_KEY));
            editor.putString(COGNITO_POOL_ID_KEY, properties.getProperty(COGNITO_POOL_ID_KEY));

            // check region
            final String region = properties.getProperty(MY_REGION_KEY);
            try {
                editor.putString(MY_REGION_KEY, Regions.fromName(region).getName());

                if (!properties.getProperty(CUSTOMER_SPECIFIC_ENDPOINT_KEY).contains(region) && !properties.getProperty(COGNITO_POOL_ID_KEY).contains(region)) {
                    final String errMsg = String.format("Customer specific endpoint and/or Cognito pool ID have different region than defined \"%s\".", region);
                    Log.e(TAG, errMsg);
                    listener.onPreferencesLoadError(errMsg);
                    return;
                }
            } catch (IllegalArgumentException e) {
                final String errMsg = String.format("Region \"%s\" is not valid.", region);
                Log.e(TAG, errMsg, e);
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
            Log.e(TAG, errMsg, e);
            listener.onPreferencesLoadError(errMsg);
        } catch (IllegalArgumentException e) {
            final String errMsg = "Wrong file format selected.";
            Log.e(TAG, errMsg, e);
            listener.onPreferencesLoadError(errMsg);
        }
    }
}
