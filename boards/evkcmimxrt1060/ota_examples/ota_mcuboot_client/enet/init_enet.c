/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/*${header:start}*/
#include "board.h"

#include "ethernetif.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "ethernetif.h"

#include "network_cfg.h"

#include "fsl_silicon_id.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define INIT_SUCCESS 0
#define INIT_FAIL    1

/* @TEST_ANCHOR */

#ifndef EXAMPLE_BANNER
#define EXAMPLE_BANNER "OTA HTTPS Client Example"
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
static phy_handle_t phyHandle;
static struct netif netif;

/*${variable:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*${function:start}*/
int initNetwork(void)
{
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = EXAMPLE_PHY_ADDRESS,
        .phyOps      = EXAMPLE_PHY_OPS,
        .phyResource = EXAMPLE_PHY_RESOURCE,
        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
        .macAddress = configMAC_ADDR,
#endif
    };

#ifdef IP_USE_DHCP
#ifndef configMAC_ADDR
    /* Set special address for each chip. */
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    IP4_ADDR(&netif_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netif_netmask, 0, 0, 0, 0);
    IP4_ADDR(&netif_gw, 0, 0, 0, 0);
#else
    ip4addr_aton(IP_ADDR, &netif_ipaddr);
    ip4addr_aton(IP_MASK, &netif_netmask);
    ip4addr_aton(GW_ADDR, &netif_gw);
#endif

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

#ifdef IP_USE_DHCP
    PRINTF("Obtaining IP address from DHCP...\n");
    netifapi_dhcp_start(&netif);

    struct dhcp *dhcp;
    dhcp = netif_dhcp_data(&netif);

    while (dhcp->state != DHCP_STATE_BOUND)
    {
        vTaskDelay(100);
    }
#endif

    PRINTF("\r\n************************************************\r\n");
    PRINTF(EXAMPLE_BANNER "\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif.ip_addr.addr)[0], ((u8_t *)&netif.ip_addr.addr)[1],
           ((u8_t *)&netif.ip_addr.addr)[2], ((u8_t *)&netif.ip_addr.addr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif.netmask.addr)[0], ((u8_t *)&netif.netmask.addr)[1],
           ((u8_t *)&netif.netmask.addr)[2], ((u8_t *)&netif.netmask.addr)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&netif.gw.addr)[0], ((u8_t *)&netif.gw.addr)[1],
           ((u8_t *)&netif.gw.addr)[2], ((u8_t *)&netif.gw.addr)[3]);
    PRINTF("************************************************\r\n");

    return INIT_SUCCESS;
}
/*${function:end}*/
