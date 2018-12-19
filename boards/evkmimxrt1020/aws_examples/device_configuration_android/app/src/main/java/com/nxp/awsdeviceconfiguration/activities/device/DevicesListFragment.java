/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.activities.device;

import android.arch.lifecycle.ViewModelProviders;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.TextInputLayout;
import android.support.v4.app.FragmentActivity;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import com.nxp.awsdeviceconfiguration.R;
import com.nxp.awsdeviceconfiguration.activities.about.AboutActivity;
import com.nxp.awsdeviceconfiguration.activities.example.ShadowExampleActivity;
import com.nxp.awsdeviceconfiguration.iot.AwsConstants;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequest;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequestAsyncTask;
import com.nxp.awsdeviceconfiguration.iot.DeviceRequestCommand;
import com.nxp.awsdeviceconfiguration.iot.listeners.IDeviceRequestAsyncTaskListener;
import com.nxp.awsdeviceconfiguration.nsd.DeviceInfo;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;

public class DevicesListFragment extends DeviceFragment {
    private static final String TAG = "AWS: Devices List";

    public static final String STACK_TAG = "devices_list_back_stack_tag";

    private static final int FILE_DIALOG_ID = 5;

    private FragmentActivity activity;
    private DevicesListViewModel devicesListViewModel;

    // GUI elements
    private ListView devicesListView;
    private SwipeRefreshLayout devicesListSwipeRefresh;
    private TextView noDevicesFound;
    private MenuItem refreshButton;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_devices_list, container, false);
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "on create");
        setHasOptionsMenu(true);

        activity = getActivity();
        if (activity != null) {
            // get view model
            devicesListViewModel = ViewModelProviders.of(activity).get(DevicesListViewModel.class);
        }
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        // GUI elements
        devicesListView = view.findViewById(R.id.devicesListView);
        devicesListSwipeRefresh = view.findViewById(R.id.devicesListSwipeRefresh);
        noDevicesFound = view.findViewById(R.id.noDevicesFound);

        // check AWS preferences
        if (!aws.checkPreferencesAndValidate()) {
            // preferences not loaded yet
            new AlertDialog.Builder(activity).setTitle("Load preferences")
                    .setMessage("Please locate properties file with AWS IoT preferences (e.g. Customer specific endpoint, Cognito pool ID, etc.).")
                    .setPositiveButton("Load", (dialog, which) -> preferencesLoadFileDialog())
                    .setCancelable(false)
                    .show();
        } else {
            // preferences are correct, no need to load it so prepare UI
            prepareUILayout();
        }
    }

    /**
     * Prepare UI layout after successful loading of AWS preferences either from file or from preferences storage.
     */
    private void prepareUILayout() {
        final DevicesListAdapter adapter = new DevicesListAdapter(getActivity(), R.layout.item_devices_list, new ArrayList<>());
        // assign adapter to ListView
        devicesListView.setAdapter(adapter);

        // observe device list (filled by view model)
        devicesListViewModel.getDevicesList().observe(this, devicesList -> {
            if (devicesList != null) {
                noDevicesFound.setVisibility(View.INVISIBLE);
                devicesListView.setVisibility(View.VISIBLE);

                // update UI
                adapter.clear();
                adapter.addAll(devicesList);
                adapter.notifyDataSetChanged();
            } else {
                noDevicesFound.setVisibility(View.VISIBLE);
                new AlertDialog.Builder(activity)
                        .setTitle("Network Service Discovery has timed out.")
                        .setMessage("No device has been found!\nMake sure you are connected to same network as searched device/s.")
                        .setPositiveButton("Search again", (dialog, which) -> {
                            dialog.dismiss();

                            devicesListSwipeRefresh.setRefreshing(true);
                            refreshDevicesList();
                        })
                        .setNegativeButton("Cancel", (dialog, which) -> dialog.cancel())
                        .show();
            }

            // indicate stop of list refreshing
            devicesListSwipeRefresh.setRefreshing(false);

            // enable refresh button
            if (refreshButton != null) {
                refreshButton.setEnabled(true);
            }
        });

        // devices list on click
        devicesListView.setOnItemClickListener((parent, view1, position, id) -> {
            // stop NSD
            devicesListViewModel.stopDiscovery();

            // selected device
            final DeviceInfo device = (DeviceInfo) parent.getAdapter().getItem(position);
            devicesListViewModel.select(device);

            // create device login dialog
            final View loginDialogView = activity.getLayoutInflater().inflate(R.layout.dialog_device_login, null);
            final EditText loginDialogPassword = loginDialogView.findViewById(R.id.deviceLogin);
            final TextInputLayout loginDialogPassword_label = loginDialogView.findViewById(R.id.deviceLogin_label);
            loginDialogPassword.addTextChangedListener(new MyTextWatcher(activity, loginDialogPassword, loginDialogPassword_label));

            final AlertDialog loginDialog = new AlertDialog.Builder(activity)
                    .setTitle("Login to device")
                    .setView(loginDialogView)
                    .setCancelable(false)
                    .setPositiveButton(R.string.device_login, (dialog, which) -> {
                        // is handled via on click listener
                    })
                    .setNegativeButton("Cancel", ((dialog, which) -> {
                        dialog.cancel();

                        // go back to NSD
                        activity.getSupportFragmentManager()
                                .beginTransaction()
                                .add(R.id.fragmentWrapper, new DevicesListFragment())
                                .addToBackStack(null)
                                .commit();
                    }))
                    .show();
            loginDialog.getButton(AlertDialog.BUTTON_POSITIVE).setOnClickListener((view) -> {
                if (!validateEditText(activity, loginDialogPassword, loginDialogPassword_label)) {
                    return;
                }
                // hide login dialog
                loginDialog.cancel();

                // get device information
                Queue<DeviceRequest> deviceRequestQueue = new LinkedList<>();
                deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.LOGIN, loginDialogPassword.getText().toString()));
                deviceRequestQueue.add(new DeviceRequest(DeviceRequestCommand.GET_DEVICE_INFO));

                setStatusMessage("Establishing an SSL connection to device...");

                new DeviceRequestAsyncTask(device.getIp(), device.getPort(), deviceRequestQueue, new IDeviceRequestAsyncTaskListener() {
                    @Override
                    public void onRequestStart(DeviceRequestCommand command) {
                        setStatusMessage((command == DeviceRequestCommand.LOGIN ? "Logging into device..." : "Getting device information..."));
                    }

                    @Override
                    public void onDone(String payload) {
                        progressDialog.dismiss();

                        // save device password
                        // this should be saved into keystore to be more secure
                        device.setLoginPassword(loginDialogPassword.getText().toString());

                        // parse payload to device info
                        device.parseInfo(payload);
                        Log.d(TAG, String.format("Device information: %s", device.toString()));

                        // device has AWS already set, reconfigure or go to demo dialog
                        if (!device.areAWSCredentialsSet()) {
                            goToDeviceSetup();
                        } else {
                            new AlertDialog.Builder(activity)
                                    .setTitle("Device is already configured")
                                    .setMessage("Do you want to reconfigure it?")
                                    .setPositiveButton("Reconfigure", (dialog, which) -> {
                                        dialog.cancel();
                                        goToDeviceSetup();
                                    })
                                    .setNegativeButton("Go to demo", (dialog, which) -> {
                                        // go to demo activity
                                        dialog.cancel();
                                        DevicesListFragment.this.startActivity(new Intent(DevicesListFragment.this.getActivity(), ShadowExampleActivity.class));
                                    })
                                    .show();
                        }
                    }

                    @Override
                    public void onError(String errMsg) {
                        showErrorDialog(errMsg);
                    }
                }).execute();
            });
        });

        // refresh device list
        devicesListSwipeRefresh.setRefreshing(true);

        // enable drag list to refresh
        devicesListSwipeRefresh.setOnRefreshListener(this::refreshDevicesList);
    }

    @Override
    public void onResume() {
        if (devicesListViewModel != null) {
            devicesListSwipeRefresh.setRefreshing(true);
            refreshDevicesList();
        }
        super.onResume();
    }

    /**
     * Go to device setup fragment
     */
    private void goToDeviceSetup() {
        activity.getSupportFragmentManager()
                .beginTransaction()
                .add(R.id.fragmentWrapper, new DeviceSetupFragment())
                .addToBackStack(DeviceSetupFragment.STACK_TAG)
                .commit();
    }

    /**
     * Refresh list of devices
     */
    private void refreshDevicesList() {
        Log.d(TAG, "Refreshing device list");

        // show progress bar and hide list view
        devicesListView.setVisibility(View.INVISIBLE);
        noDevicesFound.setVisibility(View.INVISIBLE);

        if (refreshButton != null) {
            refreshButton.setEnabled(false);
        }

        devicesListViewModel.refreshDeviceList();
    }

    /**
     * Dialog for loading preferences from file
     */
    private void preferencesLoadFileDialog() {
        startActivityForResult(Intent.createChooser(new Intent()
                    .setType("*/*")
                    .addCategory(Intent.CATEGORY_OPENABLE)
                    .setAction(Intent.ACTION_GET_CONTENT)
                    .putExtra("android.content.extra.SHOW_ADVANCED", true)
                    .putExtra("android.content.extra.FANCY", true)
                    .putExtra("android.content.extra.SHOW_FILESIZE", true),
                "Select a preferences file"),
                FILE_DIALOG_ID
        );
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);

        // file dialog result
        if ((requestCode == FILE_DIALOG_ID) && (resultCode == DeviceActivity.RESULT_OK) && (intent != null)) {
            aws.loadPreferencesFromFile(activity, intent.getData(), new AwsConstants.IPreferencesListener() {
                @Override
                public void onPreferencesLoadSuccess() {
                    // preferences are loaded, prepare UI and start NSD
                    prepareUILayout();

                    // display success dialog
                    new AlertDialog.Builder(activity)
                            .setTitle("Preferences loaded successfully.")
                            .setMessage(String.format("AWS endpoint: \"%s\"\nCognito pool ID: \"%s\"\nRegion: \"%s\"",
                                    aws.getCustomerSpecificEndpoint(), aws.getCognitoPoolId(), aws.getMyRegion().getName()))
                            .setPositiveButton(android.R.string.ok, (dialog, which) -> dialog.dismiss())
                            .show();
                }

                @Override
                public void onPreferencesLoadError(String errMsg) {
                    new AlertDialog.Builder(activity).setTitle("Could not load preferences")
                            .setMessage("Could not load properties file with AWS IoT preferences.\nReason: " +
                                    errMsg + "\nPlease select correct *.properties file.")
                            .setPositiveButton(android.R.string.yes, (dialog, which) -> preferencesLoadFileDialog())
                            .show();
                }
            });
        }
    }

    @Override
    public void onPrepareOptionsMenu(Menu menu) {
        // refresh button
        refreshButton = menu.findItem(R.id.actionRefresh);
        refreshButton.setVisible(true);

        showBackButton(false);

        // display load preferences
        menu.findItem(R.id.menuItemLoadPreferences).setVisible(true);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.actionRefresh:
                if (devicesListViewModel != null) {
                    devicesListSwipeRefresh.setRefreshing(true);
                    refreshDevicesList();
                }
                break;

            case R.id.menuItemAbout:
                // load About activity
                DevicesListFragment.this.startActivity(new Intent(DevicesListFragment.this.getActivity(), AboutActivity.class));
                return true;

            case R.id.menuItemLoadPreferences:
                // open file dialog for loading preferences
                preferencesLoadFileDialog();
                return true;

            default:
                break;
        }

        return super.onOptionsItemSelected(item);
    }
}
