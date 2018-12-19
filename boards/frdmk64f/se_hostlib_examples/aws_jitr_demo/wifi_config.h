/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

/* If using WIFI based examples, change these to applicable values */

/*
 * Wi-Fi network to join.
 */
#define clientcredentialWIFI_SSID       "iot_wifi_ssid"

/*
 * Password needed to join Wi-Fi network.
 */
#define clientcredentialWIFI_PASSWORD   "password1234"

/**
 * @brief Security type
 * WPA2 Security, @see WIFISecurity_t
 * other values are - eWiFiSecurityOpen, eWiFiSecurityWEP, eWiFiSecurityWPA
 */
#define clientcredentialWIFI_SECURITY   eWiFiSecurityWPA2

#endif
