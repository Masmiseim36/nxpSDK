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
#include "lwip/netifapi.h"
#include "wwd.h"
#include "wwd_wiced.h"
#include "wwd_network.h"
#include "dhcp_server.h"
#include "timers.h"

#include "fsl_debug_console.h"

#include "network_cfg.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void test_join(void);
extern wwd_result_t test_scan();
extern wiced_result_t wiced_wlan_connectivity_init(void);
extern void add_wlan_interface(void);
/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
/* Set to true for AP mode, false for client mode */
bool wifi_ap_mode = WIFI_AP_MODE;
static struct netif wiced_if;
/*${variable:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*${function:start}*/
int initNetwork()
{
    wiced_result_t wiced_result = WICED_SUCCESS;
    wwd_result_t wwd_result     = WWD_SUCCESS;

    wiced_ssid_t ap_ssid = {
        .length = sizeof(WIFI_SSID) - 1,
        .value  = WIFI_SSID,
    };

    tcpip_init(NULL, NULL);

    PRINTF("Initializing WiFi connection... \r\n");
    wiced_result = wiced_wlan_connectivity_init();
    if (wiced_result != WICED_SUCCESS)
    {
        PRINTF("Could not initialize WiFi module %d\r\n", (uint32_t)wiced_result);
        return -1;
    }
    else
    {
        PRINTF("Successfully initialized WiFi module\r\n");
    }

    if (wifi_ap_mode)
    {
        /* AP mode */
        ip4_addr_t ap_ipaddr;
        ip4_addr_t ap_netmask;
        uint8_t ap_channel = WIFI_AP_CHANNEL;

        if ((ip4addr_aton(WIFI_AP_IP_ADDR, &ap_ipaddr) == 0) || (ip4addr_aton(WIFI_AP_NET_MASK, &ap_netmask) == 0))
        {
            PRINTF("Invalid IP address\r\n");
            return -1;
        }

        PRINTF("Starting Access Point: SSID: %s, Chnl: %d\r\n", (char *)ap_ssid.value, ap_channel);

        /* Start the access point */
        wwd_result =
            wwd_wifi_start_ap(&ap_ssid, WIFI_SECURITY, (uint8_t *)WIFI_PASSWORD, sizeof(WIFI_PASSWORD) - 1, ap_channel);
        if (wwd_result != WWD_SUCCESS)
        {
            PRINTF("Failed to start access point\r\n");
            return -1;
        }

        /* Configure network interface */
        if (ERR_OK != netifapi_netif_add(&wiced_if, &ap_ipaddr, &ap_netmask, &ap_ipaddr, (void *)WWD_AP_INTERFACE,
                                         wlanif_init, tcpip_input))
        {
            PRINTF("Failed to start network interface\r\n");
            return -1;
        }
        netifapi_netif_set_default(&wiced_if);
        netifapi_netif_set_up(&wiced_if);

        PRINTF("Network ready IP: %u.%u.%u.%u\r\n", (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 24) & 0xff),
               (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 16) & 0xff),
               (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 8) & 0xff),
               (unsigned char)((htonl(wiced_if.ip_addr.addr) >> 0) & 0xff));
        /* Start DHCP server */
        start_dhcp_server(ap_ipaddr.addr);

        return 0;
    }

#if WIFI_SCAN_NETWORKS
    wwd_result = test_scan();
    if (wwd_result != WWD_SUCCESS)
    {
        PRINTF(" Scan Error\r\n");
        return -1;
    }
#endif

    /* Client mode */
    PRINTF("Joining: " WIFI_SSID "\r\n");
    (void)host_rtos_delay_milliseconds((uint32_t)1000);
    wwd_result = wwd_wifi_join(&ap_ssid, WIFI_SECURITY, (uint8_t *)WIFI_PASSWORD, sizeof(WIFI_PASSWORD) - 1, NULL,
                               WWD_STA_INTERFACE);
    if (wwd_result != WWD_SUCCESS)
    {
        PRINTF("Failed to join: " WIFI_SSID "\r\n");
        return -1;
    }
    else
    {
        PRINTF("Successfully joined: " WIFI_SSID "\r\n");
        (void)host_rtos_delay_milliseconds((uint32_t)1000);
        add_wlan_interface();
    }

    return 0;
}
/*${function:end}*/
