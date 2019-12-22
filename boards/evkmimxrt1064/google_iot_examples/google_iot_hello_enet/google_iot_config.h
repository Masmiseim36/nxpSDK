/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GOOGLE_IOT_CONFIG_H
#define GOOGLE_IOT_CONFIG_H

/* Common WiFi parameters */
#ifndef WIFI_SSID
#define WIFI_SSID "your_wifi_ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "your_password"
#endif

#ifndef WIFI_SECURITY
#define WIFI_SECURITY WICED_SECURITY_WPA2_MIXED_PSK
#endif

#define keyCLIENT_PRIVATE_KEY_PEM      \
    "-----BEGIN EC PRIVATE KEY-----\n" \
    "Paste your private key here\n"    \
    "-----END EC PRIVATE KEY-----\n"
#define iotcPROJECT_ID "YOUR_PROJECT_ID"
#define iotcDEVICE_PATH \
    "projects/YOUR_PROJECT_ID/locations/YOUR_REGION/registries/YOUR_REGISTRY_ID/devices/YOUR_DEVICE_ID"
#define iotcPUBLISH_TOPIC "/devices/YOUR_DEVICE_ID/state"
#define iotcPUBLISH_MESSAGE "Hello From Your IoTC client!"

#endif /* GOOGLE_IOT_CONFIG_H */
