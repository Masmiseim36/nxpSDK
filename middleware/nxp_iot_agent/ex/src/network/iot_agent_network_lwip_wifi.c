/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_status.h>
#include <nxp_iot_agent_log.h>

#include "wpl.h"

#ifndef AP_SSID
#define AP_SSID "WiFi SSID"
#endif

#ifndef AP_PASSWORD
#define AP_PASSWORD "WiFi Password"
#endif


#define WIFI_NETWORK_LABEL "el2go_agent_wifi"

/* Link lost callback */
static void LinkStatusChangeCallback(bool linkState)
{
    if (linkState == false)
    {
        /* -------- LINK LOST -------- */
        /* DO SOMETHING */
        IOT_AGENT_INFO("-------- LINK LOST --------\r\n");
    }
    else
    {
        /* -------- LINK REESTABLISHED -------- */
        /* DO SOMETHING */
        IOT_AGENT_INFO("-------- LINK REESTABLISHED --------\r\n");
    }
}

/*!
 * @brief The main task function
 */
iot_agent_status_t network_init(void)
{
    wpl_ret_t result = WPLRET_SUCCESS;

    /* Initialize Wi-Fi board */
    IOT_AGENT_INFO("Initializing Wi-Fi connection... \r\n");

    result = WPL_Init();
    if (result != WPLRET_SUCCESS)
    {
        IOT_AGENT_INFO("[!] WPL Init failed: %d\r\n", (uint32_t)result);
        return IOT_AGENT_FAILURE;
    }

    result = WPL_Start(LinkStatusChangeCallback);
    if (result != WPLRET_SUCCESS)
    {
        IOT_AGENT_INFO("[!] WPL Start failed %d\r\n", (uint32_t)result);
        return IOT_AGENT_FAILURE;
    }

    IOT_AGENT_INFO("[i] Successfully initialized Wi-Fi module\r\n");

    /* Add Wi-Fi network */
    result = WPL_AddNetwork(AP_SSID, AP_PASSWORD, WIFI_NETWORK_LABEL);
    if (result == WPLRET_SUCCESS)
    {
        IOT_AGENT_INFO("Connecting as client to ssid: %s\r\n", AP_SSID);
        result = WPL_Join(WIFI_NETWORK_LABEL);
    }

    if (result != WPLRET_SUCCESS)
    {
	    IOT_AGENT_INFO("[!] Cannot connect to Wi-Fi\r\n[!]ssid: %s\r\n", AP_SSID);
        return IOT_AGENT_FAILURE;
    }
    else
    {
	  	IOT_AGENT_INFO("[i] Connected to Wi-Fi\r\nssid: %s\r\n", AP_SSID);
        char ip[16];
        WPL_GetIP(ip, 1);
    }

    return IOT_AGENT_SUCCESS;

}
