/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.activities.example;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.google.gson.JsonSyntaxException;
import com.nxp.awsdeviceconfiguration.R;
import com.nxp.awsdeviceconfiguration.activities.about.AboutActivity;
import com.nxp.awsdeviceconfiguration.activities.device.DeviceActivity;
import com.nxp.awsdeviceconfiguration.iot.AwsConstants;
import com.nxp.awsdeviceconfiguration.iot.MQTTConnection;

import java.io.UnsupportedEncodingException;

import android.os.Handler;
import android.widget.Toast;

public class ShadowExampleActivity extends AppCompatActivity {

    /** MQTT connection */
    private MQTTConnection mqttConnection;

    /** AWS related constants (endpoint, cognito, etc. */
    private AwsConstants aws;

    /** GUI items */
    private TextView tvConnectionStatus;
    private ProgressBar progressBarConnection;
    private ProgressBar progressBarShadow;
    private TextView awsShadowText;

    /** variables */
    private Handler timeoutHandler;
    private Runnable displayTimeoutToast;
    private Runnable mqttTryToConnect;
    private AWSIotMqttClientStatusCallback mqttClientStatusCallback;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shadow_example);

        // initialize toolbar
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        // add back arrow to toolbar
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setDisplayShowHomeEnabled(true);
        }

        // GUI items instances
        tvConnectionStatus = findViewById(R.id.tvConnectionStatus);
        progressBarConnection =  findViewById(R.id.progressBarConnection);
        progressBarShadow = findViewById(R.id.progressBarShadow);
        awsShadowText = findViewById(R.id.awsShadowText);

        // default variable values
        timeoutHandler = new Handler();
        final AppCompatActivity activity = this;

        awsShadowText.setText(getString(R.string.empty_shadow));

        displayTimeoutToast = () -> {
            Toast.makeText(
                getApplicationContext(),
                "AWS IoT server did not respond in time.",
                Toast.LENGTH_LONG
            ).show();

            runOnUiThread(() -> progressBarShadow.setVisibility(View.INVISIBLE));
        };

        mqttTryToConnect = new Runnable() {
            public void run() {
                Log.d(MQTTConnection.TAG, "Trying to connect to MQTT");

                if (!isNetworkAvailable()) {
                    runOnUiThread(() -> {
                        tvConnectionStatus.setText(getString(R.string.network_not_available));
                        progressBarConnection.setVisibility(View.INVISIBLE);
                    });

                    // run timer again
                    timeoutHandler.postDelayed(this, AwsConstants.CONNECT_TIMEOUT);
                } else {
                    // network available, try to connect
                    runOnUiThread(() -> {
                        tvConnectionStatus.setText(getString(R.string.connecting));
                        progressBarConnection.setVisibility(View.VISIBLE);
                    });

                    final boolean isConnected = mqttConnection.connect(mqttClientStatusCallback);

                    if (isConnected) {
                        // cancel timeout
                        timeoutHandler.removeCallbacks(this);
                    } else {
                        new AlertDialog.Builder(activity).setTitle("Could not connect to AWS IoT")
                                .setMessage("Error during creation of AWS IoT client.\nPlease try again.")
                                .setPositiveButton(android.R.string.yes, (dialog, which) -> {
                                    // try to establish MQTT connection
                                    timeoutHandler.postDelayed(mqttTryToConnect, 1);
                                })
                                .show();
                    }
                }
            }
        };

        // get saved preferences
        aws = new AwsConstants(getApplicationContext());
        aws.refreshShadowUrls();

        // create MQTT client status callback
        mqttClientStatusCallback = (status, throwable) -> {
            Log.d(MQTTConnection.TAG, "MQTT connection status: " + String.valueOf(status));

            // display connection status on dashboard
            runOnUiThread(() -> {
                switch (status) {
                    case Connecting:
                        tvConnectionStatus.setText(getString(R.string.connecting));
                        progressBarConnection.setVisibility(View.VISIBLE);
                        break;

                    case Connected:
                        // MQTT connection is ready, can do publish and subscribe
                        tvConnectionStatus.setText(String.format("Connected to \"%s\" thing", aws.getThingName()));
                        progressBarConnection.setVisibility(View.INVISIBLE);
                        break;

                    case Reconnecting:
                        if (throwable != null) {
                            Log.e(MQTTConnection.TAG, "MQTTConnection error.", throwable);
                        }
                        if (isNetworkAvailable()) {
                            tvConnectionStatus.setText(getString(R.string.reconnecting));
                            progressBarConnection.setVisibility(View.VISIBLE);
                        } else {
                            tvConnectionStatus.setText(getString(R.string.network_not_available));
                            progressBarConnection.setVisibility(View.INVISIBLE);
                        }
                        break;

                    case ConnectionLost:
                        if (throwable != null) {
                            Log.e(MQTTConnection.TAG, "MQTTConnection error.", throwable);
                            throwable.printStackTrace();
                        }
                        tvConnectionStatus.setText(getString(R.string.mqtt_error));
                        progressBarConnection.setVisibility(View.INVISIBLE);
                        break;

                    default:
                        tvConnectionStatus.setText(getString(R.string.disconnected));
                        progressBarConnection.setVisibility(View.INVISIBLE);
                        break;
                }
            });

            // when MQTT manager is connected to AWS server
            // subscribe for accepted updates from AWS remote control example shadow
            // and download shadow after successful connection
            if (status == AWSIotMqttClientStatusCallback.AWSIotMqttClientStatus.Connected) {
                // subscription callback for updating GUI values
                final AWSIotMqttNewMessageCallback subscriptionCallback = (topic, data) -> {
                    try {
                        String message = new String(data, "UTF-8");
                        Log.d(MQTTConnection.TAG, String.format("Received message \"%s\" from topic \"%s\"", message, topic));

                        if (topic.equals(aws.SHADOW_GET_ACCEPTED)) {
                            timeoutHandler.removeCallbacks(displayTimeoutToast);
                        }

                        final String formatted = formatJSON(message);
                        runOnUiThread(() -> awsShadowText.setText(formatted));
                    } catch (UnsupportedEncodingException e) {
                        Log.e(MQTTConnection.TAG, "Message encoding error.", e);
                    } catch (JsonSyntaxException e) {
                        Log.e(MQTTConnection.TAG, "Could not parse message JSON.", e);
                    }
                };

                // subscribe for all accepted changes
                mqttConnection.subscribe(aws.SHADOW_UPDATE_ACCEPTED, subscriptionCallback);

                // get device shadow after successful connection
                mqttConnection.subscribe(aws.SHADOW_GET_ACCEPTED, subscriptionCallback);

                // wait one second for subscribe to be done before sending GET request
                timeoutHandler.postDelayed(() -> {
                    // make publish
                    mqttConnection.publish(aws.SHADOW_GET, AwsConstants.EMPTY_MESSAGE);

                    // wait for certain time for device response
                    timeoutHandler.postDelayed(displayTimeoutToast, AwsConstants.TIMEOUT);
                }, 1000);
            }
        };

        // Create new MQTT connection instance
        mqttConnection = new MQTTConnection(getApplicationContext(), aws);


        // try to establish MQTT connection
        timeoutHandler.postDelayed(mqttTryToConnect, 1);
    }

    /**
     * Format JSON string to human readable format
     * @param txt JSON string to be formatted
     * @return Formatted JSON string
     */
    private String formatJSON(String txt){
        StringBuilder json = new StringBuilder();
        int numOfTabs = 0;

        for (char ch : txt.toCharArray()) {
            switch (ch) {
                case '{':
                    numOfTabs++;
                    json.append(ch).append("\n");

                    for (int i=0; i<numOfTabs; i++) {
                        json.append("    ");
                    }
                    break;

                case '}':
                    numOfTabs--;
                    json.append("\n");

                    for (int i=0; i<numOfTabs; i++) {
                        json.append("    ");
                    }

                    json.append(ch);
                    break;

                case ',':
                    json.append(ch).append("\n");

                    for (int i=0; i<numOfTabs; i++) {
                        json.append("    ");
                    }
                    break;

                default:
                    json.append(ch);
                    break;
            }
        }

        return json.toString();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (mqttConnection != null && !mqttConnection.disconnect()) {
            Log.i(MQTTConnection.TAG, "Could not disconnect from AWS IoT.");
        }
    }

    /**
     * Check if current network (WiFi or mobile) connection is connected
     */
    private boolean isNetworkAvailable() {
        final ConnectivityManager connectivityManager = (ConnectivityManager) getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);

        boolean isConnected = false;
        if (connectivityManager != null) {
            NetworkInfo activeNetwork = connectivityManager.getActiveNetworkInfo();
            isConnected = (activeNetwork != null) && (activeNetwork.isConnectedOrConnecting());
        }
        return isConnected;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        // hide refresh and load preferences buttons
        menu.findItem(R.id.actionRefresh).setVisible(false);
        menu.findItem(R.id.menuItemLoadPreferences).setVisible(false);
        return true;
    }

    /**
     * Remove all fragments from back stack and go to first activity, i.e. start the application from scratch
     */
    private void goToDevicesList() {
        finish();

        startActivity(new Intent(ShadowExampleActivity.this, DeviceActivity.class)
                .setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK)
        );
    }

    @Override
    public void onBackPressed() {
        goToDevicesList();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                goToDevicesList();
            break;

            case R.id.menuItemAbout:
                // load About activity
                startActivity(new Intent(ShadowExampleActivity.this, AboutActivity.class));
                return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
