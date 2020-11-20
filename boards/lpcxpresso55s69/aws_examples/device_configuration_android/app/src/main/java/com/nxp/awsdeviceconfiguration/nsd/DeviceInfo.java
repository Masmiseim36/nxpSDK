/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.awsdeviceconfiguration.nsd;

import android.net.nsd.NsdServiceInfo;
import android.support.annotation.NonNull;
import android.util.Log;

import com.nxp.awsdeviceconfiguration.iot.WiFiSecurity;

import java.net.InetAddress;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

// NOTE: NSD txt record is only available since Android 7.0
public class DeviceInfo {
    private static final String TAG = "AWS: Device Info";

    private static final String WIFI_SSID_KEY = "wifi_ssid";
    private static final String WIFI_SECURITY_KEY = "wifi_security";
    private static final String CONNECTION_KEY = "connection";
    private static final String AWS_THING_NAME_KEY = "aws_thing_name";
    private static final String AWS_CREDENTIALS_KEY = "aws_credentials";

    private String serviceName;
    private String serviceType;
    private InetAddress ip;
    private int port;

    public enum ConnectionType {
        ENET("Ethernet"),
        WIFI_AP("WiFi AP"),
        WIFI("WiFi");

        private String txt;

        ConnectionType(String txt) {
            this.txt = txt;
        }

        public String getText() {
            return txt;
        }
    }

    private ConnectionType connectionType;
    private String wifiSSID;
    private WiFiSecurity.Type wiFiSecurity;
    private String thingName;
    private boolean areAWSCredentialsSet;

    private String loginPassword;

    public DeviceInfo(NsdServiceInfo info) {
        this.serviceName = info.getServiceName();
        this.serviceType = info.getServiceType();
        this.ip = info.getHost();
        this.port = info.getPort();

        this.connectionType = ConnectionType.ENET;

    }

    @Override
    public String toString() {
        return "name: " + serviceName +
                ", type: " + serviceType +
                ", IP address: " + ip +
                ", port: " + port +
                ", connection: " + connectionType.toString() +
                (connectionType != ConnectionType.ENET ? ", SSID: " + wifiSSID + " (" + wiFiSecurity.toString() + "}" : "") +
                ", AWS thing name: " + thingName +
                ", AWS credentials set: " + String.valueOf(areAWSCredentialsSet);
    }

    @Override
    public int hashCode() {
        return Objects.hash(serviceName, serviceType, ip, port, connectionType);
    }

    /**
     * Get enum value from string with enum ordinal
     * @param value String with enum ordinal
     * @param values Array of enum type values
     * @param <T> Enum type
     * @return Enum value
     */
    private <T extends Enum<T>> T getEnumFromString(String value, T[] values) {
        if (!value.isEmpty()) {
            try {
                final int type = Integer.parseInt(value, 10);
                if (type < values.length) {
                    return values[type];
                }
            } catch (NumberFormatException e) {
                Log.e(TAG, "Exception during getting enum from string", e);
            }
        }
        return values[0];
    }

    /**
     * Find attribute in payload string and return value.
     * @param payload String payload
     * @param attr Attribute to find value for
     * @return Value for specified attribute, if found.
     */
    @NonNull
    private String findAttributeInPayload(String payload, String attr) {
        final Matcher matcher = Pattern.compile(attr + "=((?:)[^,]+)").matcher(payload);
        if (matcher.find()) {
            return matcher.group(1);
        }
        return "";
    }

    /**
     * Parse device information from payload string (attribute=value format).
     * @param payload String payload
     */
    public void parseInfo(String payload) {
        this.connectionType = getEnumFromString(findAttributeInPayload(payload, CONNECTION_KEY), ConnectionType.values());

        if (this.connectionType != ConnectionType.ENET) {
            this.wifiSSID = findAttributeInPayload(payload, WIFI_SSID_KEY);
            this.wiFiSecurity = getEnumFromString(findAttributeInPayload(payload, WIFI_SECURITY_KEY), WiFiSecurity.Type.values());
        }

        this.thingName = findAttributeInPayload(payload, AWS_THING_NAME_KEY);
        this.areAWSCredentialsSet = findAttributeInPayload(payload, AWS_CREDENTIALS_KEY).equals("true");
    }


    /** mDNS host name */
    public String getServiceName() {
        return serviceName;
    }

    /** IP address of device */
    public InetAddress getIp() {
        return ip;
    }

    /** Port of device */
    public int getPort() {
        return port;
    }

    /** Connection type of device */
    public ConnectionType getConnectionType() {
        return connectionType;
    }

    /** WiFi SSID */
    public String getWifiSSID() {
        return wifiSSID;
    }

    /** WiFi security */
    public WiFiSecurity.Type getWiFiSecurity() {
        return wiFiSecurity;
    }

    /** Is WiFi in AP mode */
    public boolean isWiFiInAPMode() {
        return connectionType == ConnectionType.WIFI_AP;
    }

    /** Is WiFi used */
    public boolean isWiFi() {
        return (connectionType == ConnectionType.WIFI || connectionType == ConnectionType.WIFI_AP);
    }

    /** AWS thing name of device */
    public String getThingName() {
        return thingName;
    }

    /** If AWS credentials are set on device */
    public boolean areAWSCredentialsSet() {
        return areAWSCredentialsSet;
    }

    public void setLoginPassword(String pass) {
        loginPassword = pass;
    }

    public String getLoginPassword() {
        return loginPassword;
    }
}
