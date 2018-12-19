/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.activities.device;


import android.arch.lifecycle.ViewModelProviders;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.CardView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.nxp.awsdeviceconfiguration.R;
import com.nxp.awsdeviceconfiguration.activities.about.AboutActivity;
import com.nxp.awsdeviceconfiguration.activities.example.ShadowExampleActivity;
import com.nxp.awsdeviceconfiguration.iot.AWSResult;
import com.nxp.awsdeviceconfiguration.iot.AwsIoTConnection;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequest;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequestCommand;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequestAsyncTask;
import com.nxp.awsdeviceconfiguration.iot.WiFiSecurity;
import com.nxp.awsdeviceconfiguration.iot.listeners.IAWSIoTClientListener;
import com.nxp.awsdeviceconfiguration.iot.listeners.IDeviceRequestAsyncTaskListener;
import com.nxp.awsdeviceconfiguration.nsd.DeviceInfo;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Queue;


public class DeviceSetupFragment extends DeviceFragment {
    private static final String TAG = "AWS: Device Setup";
    public static final String STACK_TAG = "device_setup_back_stack_tag";

    private final int NOT_FOUND = -1;

    // AWS IoT connection
    private AwsIoTConnection awsIoTConnection;

    // GUI items
    private EditText mDNSHostname;
    private TextInputLayout mDNSHostnameLabel;
    private TextView awsIoTEndpoint;
    private EditText deviceLoginPasswordChange;
    private CardView wifiView;
    private CardView viewAWSIoT;
    private EditText wifiSSID;
    private EditText wifiPassword;
    private TextInputLayout wifiSSIDLabel;
    private TextInputLayout wifiPasswordLabel;
    private Spinner wifiSpinner;
    private Spinner thingNameSpinner;
    private ArrayAdapter<String> thingNameAdapter;
    private ImageButton awsCreateThingName;
    private ImageButton awsLoadThingName;
    private FloatingActionButton deviceSetupSave;
    private TextView infoIpAddress;
    private TextView infoAwsThing;
    private ImageView infoAwsCredentials;
    private TextView infoConnectionType;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_device_setup, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        // GUI items
        infoIpAddress = view.findViewById(R.id.infoIpAddress);
        infoAwsThing = view.findViewById(R.id.infoAwsThing);
        infoAwsCredentials = view.findViewById(R.id.infoAwsCredentials);
        infoConnectionType = view.findViewById(R.id.infoConnectionType);
        wifiView = view.findViewById(R.id.cardViewWiFi);
        wifiSSID = view.findViewById(R.id.wifiSSID);
        wifiSSIDLabel = view.findViewById(R.id.wifiSSID_label);
        wifiPassword = view.findViewById(R.id.wifiPassword);
        wifiPasswordLabel = view.findViewById(R.id.wifiPassword_label);
        wifiSpinner = view.findViewById(R.id.wifiSecurity);
        viewAWSIoT = view.findViewById(R.id.cardViewAWSIoT);
        awsIoTEndpoint = view.findViewById(R.id.awsIoTEndpoint);
        thingNameSpinner = view.findViewById(R.id.awsIoTThingNameSpinner);
        awsCreateThingName = view.findViewById(R.id.awsIoTCreateThingName);
        awsLoadThingName = view.findViewById(R.id.awsIoTLoadThingName);
        mDNSHostname = view.findViewById(R.id.mDNSHostname);
        mDNSHostnameLabel = view.findViewById(R.id.mDNSHostname_label);
        deviceLoginPasswordChange = view.findViewById(R.id.deviceLoginPasswordChange);
        deviceSetupSave = view.findViewById(R.id.deviceSetupSave);

        // input fields validators
        wifiSSID.addTextChangedListener(new MyTextWatcher(activity, wifiSSID, wifiSSIDLabel));
        mDNSHostname.addTextChangedListener(new MyTextWatcher(activity, mDNSHostname, mDNSHostnameLabel));

        // create AWS IoT client
        awsIoTConnection = new AwsIoTConnection(getActivity(), aws);

        // observe device list selected item change
        ViewModelProviders.of(activity).get(DevicesListViewModel.class).getSelected().observe(this, device -> {
            if (device == null) {
                Toast.makeText(getActivity(), "No device!", Toast.LENGTH_LONG).show();
                return;
            }

            // load list of AWS thing names
            if (!device.isWiFiInAPMode()) {
                listAWSThingNames(device.getThingName());

                // click listeners
                awsLoadThingName.setOnClickListener((view1) -> listAWSThingNames(device.getThingName()));
                awsCreateThingName.setOnClickListener((view1) -> createNewAWSThingDialog());
            }

            // device information
            infoIpAddress.setText(String.format(Locale.US, "%s:%d", device.getIp().getHostAddress(), device.getPort()));
            infoAwsThing.setText(device.getThingName().isEmpty() ? "Not set" : device.getThingName());
            infoConnectionType.setText(device.getConnectionType().getText());
            if (!device.areAWSCredentialsSet()) {
                // gray out AWS image if credentials are not set
                infoAwsCredentials.setColorFilter(Color.argb(150, 200, 200, 200));
            }

            // AWS endpoint
            awsIoTEndpoint.setText(aws.getCustomerSpecificEndpoint());

            // mDNS hostname
            mDNSHostname.setText(device.getServiceName());

            // change UI layout according to connection type
            switch (device.getConnectionType()) {
                default:
                case ENET:
                    // hide WiFi settings
                    wifiView.setVisibility(View.GONE);
                    break;

                case WIFI_AP:
                case WIFI:
                    // show WiFi settings
                    wifiView.setVisibility(View.VISIBLE);

                    // fill WiFi security spinner
                    final ArrayAdapter<String> wifiSecurityAdapter = new ArrayAdapter<>(activity, android.R.layout.simple_dropdown_item_1line, WiFiSecurity.getListOfWiFiSecurityTypes());
                    wifiSecurityAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                    wifiSpinner.setAdapter(wifiSecurityAdapter);

                    if (device.isWiFiInAPMode()) {
                        // if AP mode is active, show only WiFi settings, not AWS
                        viewAWSIoT.setVisibility(View.GONE);
                    } else {
                        viewAWSIoT.setVisibility(View.VISIBLE);

                        // fill WiFi details
                        wifiSSID.setText(device.getWifiSSID());
                        // select WiFi security
                        final int position = wifiSecurityAdapter.getPosition(device.getWiFiSecurity().toString());
                        if (position != NOT_FOUND) {
                            wifiSpinner.setSelection(position);
                        }
                    }

                    // hide WiFi password when security is open
                    wifiSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                        @Override
                        public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                            try {
                                final WiFiSecurity.Type selectedItem = WiFiSecurity.Type.valueOf(parent.getItemAtPosition(position).toString());
                                wifiPassword.setVisibility(selectedItem == WiFiSecurity.Type.OPEN ? View.GONE : View.VISIBLE);
                            } catch (IllegalArgumentException e) {
                                Log.e(TAG, "Could not get current wifi security item.", e);
                            }
                        }

                        @Override
                        public void onNothingSelected(AdapterView<?> parent) {}
                    });
                    break;
            }

            // save device configuration after click to save button
            deviceSetupSave.setOnClickListener((v) -> saveSetupToDevice(device));
        });
    }

    /**
     * Save setup to device
     * @param device Device information
     */
    private void saveSetupToDevice(DeviceInfo device) {
        // wifi settings has changed
        if (device.isWiFi()) {
            final boolean wifiSSIDHasChanged = !device.getWifiSSID().equals(wifiSSID.getText().toString());
            final boolean wifiSecurityHasChanged = !device.getWiFiSecurity().toString().equals(wifiSpinner.getSelectedItem().toString());

            if (wifiSSIDHasChanged || wifiSecurityHasChanged) {
                if (!validateEditText(activity, wifiPassword, wifiPasswordLabel)) {
                    // wifi password cannot be empty
                    return;
                }
            }
        }

        // check if values are not empty and are valid
        if (!validateEditText(activity, mDNSHostname, mDNSHostnameLabel) || (device.isWiFi() && !validateEditText(activity, wifiSSID, wifiSSIDLabel))) {
            return;
        }

        // display progress bar
        setStatusMessage("Saving configuration to device...");

        if (!device.isWiFiInAPMode()) {
            // AWS IoT settings shall be configured when device is not in WiFi AP mode
            final String thingName = thingNameSpinner.getSelectedItem().toString();

            // save AWS thing name to preferences
            aws.setThingName(thingName);

            // create AWS certificates if:
            // 1) AWS credentials on device are not set
            // 2) AWS credentials on device are set but thing name has changed
            if (!device.areAWSCredentialsSet() || (device.areAWSCredentialsSet() && !device.getThingName().equals(thingName))) {
                setStatusMessage("Creating and assigning new AWS certificate...");

                awsIoTConnection.configureAWSCredentials(thingName, new IAWSIoTClientListener() {
                    @Override
                    public void onSuccess(AWSResult result) {
                        runDeviceRequestQueue(device, (AwsIoTConnection.ConfigureAWSCredentialsResult) result);
                    }

                    @Override
                    public void onError(String errMsg) {
                        showErrorDialog(String.format("Error during AWS configuration: %s", errMsg));
                    }
                });

                return;
            }
        }

        // run device request queue without setting new AWS certificates
        runDeviceRequestQueue(device, null);
    }


    /**
     * Run device request queue
     * @param device Device information
     * @param result Result of AWS configuration
     */
    private void runDeviceRequestQueue(DeviceInfo device, AwsIoTConnection.ConfigureAWSCredentialsResult result) {
        // prepareUILayout device request queue
        Queue<DeviceRequest> deviceRequestQueue = new LinkedList<>();

        // login to device via saved password
        deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.LOGIN, device.getLoginPassword()));

        // set new login password
        if (!deviceLoginPasswordChange.getText().toString().isEmpty()) {
            deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_LOGIN_PASSWORD, deviceLoginPasswordChange.getText().toString()));
        }

        // AWS IoT settings shall be only visible when device is not in WiFi AP mode
        if (!device.isWiFiInAPMode()) {
            // AWS certificate and private key
            if (result != null) {
                deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_CERTIFICATE, result.getPemCertificate()));
                deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_PRIVATE_KEY, result.getKeyPair().getPrivateKey()));
            }

            // AWS endpoint
            deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_AWS_ENDPOINT, aws.getCustomerSpecificEndpoint()));

            // AWS thing name
            final String thingName = thingNameSpinner.getSelectedItem().toString();
            if (!device.getThingName().equals(thingName)) {
                deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_AWS_THING_NAME, thingName));
            }
        }

        // mDNS hostname
        if (!device.getServiceName().equals(mDNSHostname.getText().toString())) {
            deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_MDNS_HOSTNAME, mDNSHostname.getText().toString()));
        }

        // WiFi settings
        if (device.isWiFi()) {
            final boolean wifiSSIDHasChanged = !device.getWifiSSID().equals(wifiSSID.getText().toString());
            final boolean wifiSecurityHasChanged = !device.getWiFiSecurity().toString().equals(wifiSpinner.getSelectedItem().toString());

            if (wifiSSIDHasChanged || wifiSecurityHasChanged) {
                final WiFiSecurity wiFiSecurity = new WiFiSecurity(
                        wifiSpinner.getSelectedItem().toString(),
                        wifiSSID.getText().toString(),
                        wifiPassword.getText().toString()
                );

                deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.SET_WIFI_NETWORK, wiFiSecurity.getPayload()));
            }
        }

        // reset board to apply changes
        deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.BOARD_RESET));

        // create device request task, connect to device via SSL and execute commands
        setStatusMessage("Establishing an SSL connection to device...");

        new DeviceRequestAsyncTask(device.getIp(), device.getPort(), deviceRequestQueue, new IDeviceRequestAsyncTaskListener() {
            @Override
            public void onRequestStart(DeviceRequestCommand command) {
                setStatusMessage(String.format("Running device request command \"%s\"...", command.toString()));
            }

            @Override
            public void onDone(String payload) {
                // all requests have been sent successfully
                progressDialog.dismiss();

                if (device.isWiFiInAPMode()) {
                    new AlertDialog.Builder(activity)
                            .setTitle("Setting of device WiFi done")
                            .setMessage(String.format("Please connect phone to selected WiFi network: %s", wifiSSID.getText().toString()))
                            .setIcon(getResources().getDrawable(android.R.drawable.ic_dialog_info, activity.getTheme()))
                            .setPositiveButton("OK", (dialog, which) -> {
                                dialog.cancel();

                                // go to NSD
                                activity.getSupportFragmentManager().popBackStack();
                            })
                            .show();
                } else {
                    // go to demo
                    startActivity(new Intent(activity, ShadowExampleActivity.class));
                }
            }

            @Override
            public void onError(String errMsg) {
                showErrorDialog(errMsg);
            }
        }).execute();
    }

    /**
     * List AWS thing names
     * @param thingName Current device AWS thing name
     */
    private void listAWSThingNames(final String thingName) {
        setStatusMessage("Getting list of existing AWS things...");

        awsIoTConnection.listAWSThingNames(new IAWSIoTClientListener() {
            @Override
            public void onSuccess(AWSResult result) {
                progressDialog.dismiss();

                // get existing thing names
                final List<String> thingNames = new ArrayList<>(((AwsIoTConnection.ListAWSThingNamesResult) result).getThingNames());
                Log.d(TAG, String.format("Existing AWS things: %s", thingNames.toString()));

                // set spinner adapter
                thingNameAdapter = new ArrayAdapter<>(activity, android.R.layout.simple_dropdown_item_1line, thingNames);
                thingNameAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                thingNameSpinner.setAdapter(thingNameAdapter);

                // select current device AWS thing name
                if (thingName != null && thingName.length() > 0) {
                    final int position = thingNameAdapter.getPosition(thingName);
                    if (position != NOT_FOUND) {
                        thingNameSpinner.setSelection(position);
                    }
                }
            }

            @Override
            public void onError(String errMsg) {
                showErrorDialog(errMsg);
            }
        });
    }

    /**
     * Create new AWS thing dialog
     */
    private void createNewAWSThingDialog() {
        final View createAwsThingView = getLayoutInflater().inflate(R.layout.dialog_create_aws_thing, null);
        final EditText createAwsThingName = createAwsThingView.findViewById(R.id.createAwsThing);
        final TextInputLayout createAwsThingName_label = createAwsThingView.findViewById(R.id.createAwsThing_label);
        createAwsThingName.addTextChangedListener(new MyTextWatcher(activity, createAwsThingName, createAwsThingName_label));

        final AlertDialog thingDialog = new AlertDialog.Builder(activity)
                .setTitle("Create new AWS thing")
                .setView(createAwsThingView)
                .setPositiveButton("Create", (dialog, which) -> {})
                .setNegativeButton("Cancel", ((dialog, which) -> dialog.cancel()))
                .show();
        thingDialog.getButton(AlertDialog.BUTTON_POSITIVE).setOnClickListener((view) -> {
            if (!validateEditText(activity, createAwsThingName, createAwsThingName_label)) {
                return;
            }

            final String newThingName = createAwsThingName.getText().toString();

            // check if new thing name already exists or not
            final int position = thingNameAdapter.getPosition(newThingName);
            if (position != NOT_FOUND) {
                showErrorDialog(String.format("Thing with name \"%s\" already exists!", newThingName));
                thingDialog.cancel();
                return;
            }

            // assign new thing name to spinner and select it
            thingNameAdapter.add(newThingName);
            thingNameSpinner.setSelection(thingNameAdapter.getPosition(newThingName));
            thingNameAdapter.notifyDataSetChanged();

            // call create new thing async task
            setStatusMessage(String.format("Creating new AWS thing \"%s\"...", newThingName));

            awsIoTConnection.createNewAWSThing(newThingName, new IAWSIoTClientListener() {
                @Override
                public void onSuccess(AWSResult result) {
                    progressDialog.dismiss();
                }

                @Override
                public void onError(String errMsg) {
                    showErrorDialog(errMsg);
                }
            });

            // close dialog
            thingDialog.dismiss();
        });
    }

    @Override
    public void onPrepareOptionsMenu(Menu menu) {
        // hide refresh button
        menu.findItem(R.id.actionRefresh).setVisible(false);

        showBackButton(true);

        // hide load preferences
        menu.findItem(R.id.menuItemLoadPreferences).setVisible(false);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                activity.getSupportFragmentManager().popBackStack();
                return true;

            case R.id.menuItemAbout:
                // load About activity
                DeviceSetupFragment.this.startActivity(new Intent(DeviceSetupFragment.this.getActivity(), AboutActivity.class));
                return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
