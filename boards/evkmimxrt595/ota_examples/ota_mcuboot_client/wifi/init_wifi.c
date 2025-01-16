/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/*${header:start}*/
#include "fsl_debug_console.h"

#include "wpl.h"
#include "wifi.h"
#include "wlan.h"
#include "network_cfg.h"

/* depricated way to include wifi fw in application image */
#if defined(RW610) && defined(WPL_NO_WLAN_INIT)
#include "rw61x_wifi_bin.h"
#endif
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define WIFI_NETWORK_LABEL "demo"

/*${macro:end}*/

/* @TEST_ANCHOR */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern char wifi_ssid[];
extern char wifi_pass[];
/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
/* Set to true for AP mode, false for client mode */
bool wifi_ap_mode = WIFI_MODE;
/*${variable:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*${function:start}*/

/* Link lost callback */
static void LinkStatusChangeCallback(bool linkState)
{
    if (linkState == false)
    {
        /* -------- LINK LOST -------- */
        /* DO SOMETHING */
        PRINTF("-------- LINK LOST --------\n");
    }
    else
    {
        /* -------- LINK REESTABLISHED -------- */
        /* DO SOMETHING */
        PRINTF("-------- LINK REESTABLISHED --------\n");
    }
}

int initNetwork(void)
{
    int result;
    wifi_fw_version_ext_t ver;

    /* Initialize WiFi board */
    PRINTF("Initializing WiFi connection... \n");

    result = WPL_Init();
    
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("Failed to init wifi (%d)\n", result);
        return -1;
    }
    
/* depricated way to include wifi fw in application image */
#if defined(RW610) && defined(WPL_NO_WLAN_INIT)
    result = wlan_init(rw61x_wifi_bin, RW61X_WIFI_BIN_LEN);
    if (result != WM_SUCCESS)
    {
        PRINTF("wlan_init failed with %d\n", result);
        return -1;
    }
#endif

    result = WPL_Start(LinkStatusChangeCallback);
    if (result != WPLRET_SUCCESS)
    {
        PRINTF("Failed to start wifi (%d)\n", result);
        return -1;
    }

    PRINTF("Successfully initialized WiFi module\n");

    wifi_get_device_firmware_version_ext(&ver);
    PRINTF("WLAN FW Version: %s\n", ver.version_str);

    return 0;
}

/*${function:end}*/
