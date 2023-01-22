/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NETWORK_CFG_H_
#define _NETWORK_CFG_H_

/*${header:start}*/
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* @TEST_ANCHOR */

/* Common WiFi parameters */
#ifndef WIFI_SSID
#define WIFI_SSID "nxp-ota"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "NXP0123456789"
#endif

#define WIFI_CLIENT_MODE 0
#define WIFI_AP_MODE     1

/* This demo runs in client mode */
#define WIFI_MODE WIFI_CLIENT_MODE

#ifndef WIFI_AP_CHANNEL
#define WIFI_AP_CHANNEL 1
#endif

/*${macro:end}*/

#endif /* _NETWORK_CFG_H_ */
