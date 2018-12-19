/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.iot;

import android.os.AsyncTask;
import android.util.Log;

import com.nxp.awsdeviceconfiguration.iot.listeners.IDeviceRequestAsyncTaskListener;
import com.nxp.awsdeviceconfiguration.iot.ssl.ISSLConnectionStatus;
import com.nxp.awsdeviceconfiguration.iot.ssl.SSLConnection;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.Locale;
import java.util.Queue;

import javax.net.ssl.SSLSocket;

public class DeviceRequestAsyncTask extends AsyncTask<Void, DeviceRequestCommand, DeviceRequestResponse> {
    private static final String TAG = "AWS: Device Req Task";

    private SSLConnection sslConnection;
    private InetAddress ipAddress;
    private int port;
    private Queue<DeviceRequest> deviceRequestQueue;
    private IDeviceRequestAsyncTaskListener listener;

    private DeviceRequestCommand currentRequest;

    public DeviceRequestAsyncTask(InetAddress ipAddress, int port, Queue<DeviceRequest> deviceRequestQueue, IDeviceRequestAsyncTaskListener listener) {
        this.ipAddress = ipAddress;
        this.port = port;
        this.deviceRequestQueue = deviceRequestQueue;
        this.listener = listener;

        // create SSL connection
        sslConnection = new SSLConnection(listener::onError);
    }

    @Override
    protected DeviceRequestResponse doInBackground(Void... params) {
        try {
            // initialize SSL connection
            if (!sslConnection.connect(ipAddress, port, AwsConstants.SSL_CONNECTION_TIMEOUT)) {
                final String msg = String.format(Locale.US, "Could not connect to %s:%d", ipAddress.getHostAddress(), port);
                Log.e(TAG, msg);
                return DeviceRequestResponse.SSL_ERROR.setReason(msg);
            }

            // go through requests
            DeviceRequestResponse response = DeviceRequestResponse.FAIL;
            for (DeviceRequest req : deviceRequestQueue) {
                // has task been cancelled?
                if (isCancelled()) {
                    return DeviceRequestResponse.CANCELLED.setReason("User has cancelled sending the requests.");
                }

                // update progress that command is running
                publishProgress(req.getCommand());

                // get current request
                currentRequest = req.getCommand();

                // send request
                response = sslConnection.sendBlocking(req.getCommand(), req.getPayload());
                if (response != DeviceRequestResponse.OK) {
                    Log.e(TAG, String.format("Request command %s has failed on sending data! Response: %s, Reason: %s", currentRequest.toString(), response.toString(), response.getReason()));
                    sslConnection.closeSocket();
                    return response;
                }
            }

            // all commands were sent successfully
            sslConnection.closeSocket();
            return response;
        } catch (Exception e) {
            Log.e(TAG,"An error occurred", e);
            sslConnection.closeSocket();
            return DeviceRequestResponse.FAIL.setReason(String.format("An error occurred: %s", e.getMessage()));
        }
    }

    @Override
    protected void onProgressUpdate(DeviceRequestCommand... values) {
        super.onProgressUpdate(values);

        listener.onRequestStart(values[0]);
    }

    @Override
    protected void onPostExecute(DeviceRequestResponse response) {
        switch (response) {
            case CANCELLED:
            case SSL_ERROR:
                listener.onError(String.format("Reason: %s", response.getReason()));
                break;

            case FAIL:
                listener.onError(String.format("Request command %s has failed! Reason: %s", currentRequest.toString(), response.getReason()));
                break;

            case UNKNOWN_CMD:
                listener.onError(String.format("Unknown request command %s!", currentRequest.toString()));
                break;

            case OK:
                listener.onDone(response.getPayload());
        }
    }
}
