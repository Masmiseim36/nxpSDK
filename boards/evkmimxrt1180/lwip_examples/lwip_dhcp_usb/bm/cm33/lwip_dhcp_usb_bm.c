/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_DHCP && LWIP_RAW
#include "lwip/priv/tcpip_priv.h"
#include "ping.h"
#include "lwip/timeouts.h"
#include "lwip/ip_addr.h"
#include "lwip/init.h"
#include "lwip/api.h"
#include "lwip/dhcp.h"
#include "lwip/priv/tcpip_priv.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "usb_ethernetif.h"
#include "lwip/dns.h"
#include "netif/etharp.h"

#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN USB_EthernetIfInIt
#endif /* EXAMPLE_NETIF_INIT_FN */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*set when dhcp get ip address*/
uint32_t dhcpReady;
/*if URL's ip addrss is found, then ping_init is called, and app set this variable, only set once*/
uint8_t pingReady;
/*set when app get the URL's ip addrss*/
uint8_t dnsReady;
ip4_addr_t addrBuffer;
ip4_addr_t currentaddr;

uint8_t website[40] = {
    'w', 'w', 'w', '.', 'n', 'x', 'p', '.', 'c', 'o', 'm',
};

struct netif netif;
ethernetifConfig_t ethernetConfig;
ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Interrupt service for SysTick timer.
 */
void SysTick_Handler(void)
{
    time_isr();
}
void lwip_dns_dns_found(const char *name, const ip_addr_t *ipaddr, void *arg)
{
    if (ipaddr != NULL)
    {
        dnsReady = 1;

        currentaddr.addr = ipaddr->addr;
    }
    else
    {
        PRINTF("\r\n error in dns request\r\n");
    }
}
/*!
 * @brief Prints DHCP status of the interface when it has changed from last status.
 *
 * @param netif network interface structure
 */
static void print_dhcp_state(struct netif *netif)
{
    static u8_t dhcp_last_state = DHCP_STATE_OFF;
    struct dhcp *dhcp           = netif_dhcp_data(netif);

    if ((dhcpReady) && (!dnsReady))
    {
        err_t err;
        dhcpReady = 0;
        PRINTF("\r\n  waiting for getting the IP Address....\r\n");
        err = dns_gethostbyname((char *)&website[0], &addrBuffer, lwip_dns_dns_found, (void *)&netif);
        if (ERR_INPROGRESS == err)
        {
            /* PRINTF("\r\n error in dns get\r\n");*/
        }
        else if (ERR_OK == err)
        {
            dnsReady         = 1;
            currentaddr.addr = addrBuffer.addr;
        }
    }
    if (dnsReady)
    {
        if (!pingReady)
        {
            pingReady = 1;

            ping_init(&currentaddr);
        }
        dnsReady = 0;
        PRINTF("\r\n the IP Address of nxp.com is   : %s\r\n", ipaddr_ntoa(&currentaddr));
    }
    if (dhcp == NULL)
    {
        dhcp_last_state = DHCP_STATE_OFF;
    }
    else if (dhcp_last_state != dhcp->state)
    {
        dhcp_last_state = dhcp->state;

        PRINTF(" DHCP state       : ");
        switch (dhcp_last_state)
        {
            case DHCP_STATE_OFF:
                PRINTF("OFF");
                break;
            case DHCP_STATE_REQUESTING:
                PRINTF("REQUESTING");
                break;
            case DHCP_STATE_INIT:
                PRINTF("INIT");
                break;
            case DHCP_STATE_REBOOTING:
                PRINTF("REBOOTING");
                break;
            case DHCP_STATE_REBINDING:
                PRINTF("REBINDING");
                break;
            case DHCP_STATE_RENEWING:
                PRINTF("RENEWING");
                break;
            case DHCP_STATE_SELECTING:
                PRINTF("SELECTING");
                break;
            case DHCP_STATE_INFORMING:
                PRINTF("INFORMING");
                break;
            case DHCP_STATE_CHECKING:
                PRINTF("CHECKING");
                break;
            case DHCP_STATE_BOUND:
                PRINTF("BOUND");
                break;
            case DHCP_STATE_BACKING_OFF:
                PRINTF("BACKING_OFF");
                break;
            default:
                PRINTF("%u", dhcp_last_state);
                assert(0);
                break;
        }
        PRINTF("\r\n");

        if (dhcp_last_state == DHCP_STATE_BOUND)
        {
            PRINTF("\r\n IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
            PRINTF(" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
            PRINTF(" IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));

            dhcpReady = 1;
        }
    }
}

/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitHardware();

    time_init();

    IP4_ADDR(&netif_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_gw, 0U, 0U, 0U, 0U);
    ethernetConfig.controllerId = CONTROLLER_ID;
    ethernetConfig.privateData  = NULL;
    lwip_init();

    netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &ethernetConfig, EXAMPLE_NETIF_INIT_FN, ethernet_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    dhcp_start(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" DHCP example\r\n");
    PRINTF("************************************************\r\n");

    while (1)
    {
        /* Poll the driver, get any outstanding frames */
        USB_EthernetIfInput(&netif);

        /* Handle all system timeouts for all core protocols */
        sys_check_timeouts();

        /* Print DHCP progress */
        print_dhcp_state(&netif);
    }
}
#endif
