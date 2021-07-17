/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Code
 */

#include <string.h>
#include <stdio.h>
#include "pin_mux.h"
#include "board.h"

#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "lwip/opt.h"
#include "lwip/netifapi.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_phy.h"

#include "freemaster.h"
#include "network.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* Use DHCP for obtaining IP address */
#define EXAMPLE_USE_DHCP 1

#if EXAMPLE_USE_DHCP
/* IP address configuration. */
#define FMSTR_IP_ADDR 0U
/* Netmask configuration. */
#define FMSTR_NET_MASK 0U
/* Gateway address configuration. */
#define FMSTR_GW_ADDR 0U
#else
/* IP address configuration. */
#define FMSTR_IP_ADDR  LWIP_MAKEU32(192, 168, 1, 20)
/* Netmask configuration. */
#define FMSTR_NET_MASK LWIP_MAKEU32(255, 255, 255, 0)
/* Gateway address configuration. */
#define FMSTR_GW_ADDR  LWIP_MAKEU32(192, 168, 1, 1)
#endif /* EXAMPLE_USE_DHCP */

static struct netif netif;
static mdio_handle_t mdioHandle = {.ops = &enet_ops};
static phy_handle_t phyHandle = {.phyAddr = BOARD_ENET0_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &phyksz8081_ops};

#if EXAMPLE_USE_DHCP
static dhcp_state_enum_t dhcp_last_state = DHCP_STATE_OFF;
#endif

static void print_ipaddr(struct netif *itf, unsigned short port)
{
    PRINTF("IPv4 Address     : %s\n", ipaddr_ntoa(&itf->ip_addr));
    PRINTF("IPv4 Subnet mask : %s\n", ipaddr_ntoa(&itf->netmask));
    PRINTF("IPv4 Gateway     : %s\n\n", ipaddr_ntoa(&itf->gw));

    if (port != 0U)
    {
        PRINTF(
            "Run the FreeMASTER PC Host application, select the network communication\n"
            "plug-in and connect to this board's IP Address specified above at port %d.\n\n",
            port);
    }
}

#if EXAMPLE_USE_DHCP

/* Print DHCP address when address is bound. */
static void print_dhcp_state(void)
{
    struct dhcp *data;

    if (netif_is_up(&netif) != 0)
    {
        data = netif_dhcp_data(&netif);

        if (data == NULL)
        {
            dhcp_last_state = DHCP_STATE_OFF;
        }
        else if (dhcp_last_state != data->state)
        {
            dhcp_last_state = (dhcp_state_enum_t)data->state;

            /* Obtained IP address from DHCP */
            if (dhcp_last_state == DHCP_STATE_BOUND)
            {
                PRINTF("\n======= DHCP IP address assigned =======\n");
                print_ipaddr(&netif, FMSTR_NET_PORT);
            }
        }
    }
}
#endif /* EXAMPLE_USE_DHCP */

void Network_Init(uint32_t csrClock_Hz)
{
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    err_t err;

    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = {0x02, 0x12, 0x13, 0x10, 0x15, 0x11},
    };

    mdioHandle.resource.csrClock_Hz = csrClock_Hz;

    /* Prepare static IP address */
    netif_ipaddr.addr  = (u32_t)PP_HTONL(FMSTR_IP_ADDR);
    netif_netmask.addr = (u32_t)PP_HTONL(FMSTR_NET_MASK);
    netif_gw.addr      = (u32_t)PP_HTONL(FMSTR_GW_ADDR);

    tcpip_init(NULL, NULL);

    err = netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, ethernetif0_init,
                             tcpip_input);

    if (err == ERR_OK)
    {
        err = netifapi_netif_set_default(&netif);

        if (err == ERR_OK)
        {
            err = netifapi_netif_set_up(&netif);

            if (err != ERR_OK)
            {
                PRINTF("ERROR: netifapi_netif_set_up returned %d\n", (int)err);
            }
        }
        else
        {
            PRINTF("ERROR: netifapi_netif_set_default returned %d\n", (int)err);
        }
    }
    else
    {
        PRINTF("ERROR: netifapi_netif_add returned %d\n", (int)err);
    }

    if (err == ERR_OK)
    {
#if EXAMPLE_USE_DHCP
        /* Start DHCP, the address will be printed when acquired */
        PRINTF("Probing DHCP...\n");
        err = netifapi_dhcp_start(&netif);

        if (err != ERR_OK)
        {
            PRINTF("ERROR: netifapi_dhcp_start returned %d\n", (int)err);
        }
#else
        /* Print static IP address */
        PRINTF("\n======= Static IP address =======\n");
        print_ipaddr(&netif, FMSTR_NET_PORT);
#endif
    }
}

void Network_Poll(void)
{
#if EXAMPLE_USE_DHCP
    /* Check DHCP state and print obtained IP address */
    print_dhcp_state();
#endif
}
