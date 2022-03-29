/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/*${header:start}*/
#include "board.h"

#include "enet_ethernetif.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"

#include "network_cfg.h"
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
static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};

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
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

#ifdef IP_USE_DHCP
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

    LWIP_PLATFORM_DIAG(("\r\n************************************************"));
    LWIP_PLATFORM_DIAG((EXAMPLE_BANNER));
    LWIP_PLATFORM_DIAG(("************************************************"));
    LWIP_PLATFORM_DIAG((" IPv4 Address     : %u.%u.%u.%u", ((u8_t *)&netif.ip_addr.addr)[0],
                        ((u8_t *)&netif.ip_addr.addr)[1], ((u8_t *)&netif.ip_addr.addr)[2],
                        ((u8_t *)&netif.ip_addr.addr)[3]));
    LWIP_PLATFORM_DIAG((" IPv4 Subnet mask : %u.%u.%u.%u", ((u8_t *)&netif.netmask.addr)[0],
                        ((u8_t *)&netif.netmask.addr)[1], ((u8_t *)&netif.netmask.addr)[2],
                        ((u8_t *)&netif.netmask.addr)[3]));
    LWIP_PLATFORM_DIAG((" IPv4 Gateway     : %u.%u.%u.%u", ((u8_t *)&netif.gw.addr)[0], ((u8_t *)&netif.gw.addr)[1],
                        ((u8_t *)&netif.gw.addr)[2], ((u8_t *)&netif.gw.addr)[3]));
    LWIP_PLATFORM_DIAG(("************************************************"));

    return INIT_SUCCESS;
}
/*${function:end}*/
