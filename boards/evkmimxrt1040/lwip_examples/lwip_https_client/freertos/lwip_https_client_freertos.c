/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/opt.h"

#if LWIP_SOCKET
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "app_https_client_task.h"
#include "ethernetif.h"
#include "board.h"
#include "app.h"
#include "fsl_phy.h"

#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/dns.h"
#include "lwip/api.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"

#ifdef MBEDTLS_MCUX_ELE_S400_API
#include "ele_mbedtls.h"
#else
#include "ksdk_mbedtls.h"
#endif /* MBEDTLS_MCUX_ELE_S400_API */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Must be after include of app.h */
#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif

#ifndef EXAMPLE_HOST
/*! @brief Host name of the HTTPS server to download data from. */
#define EXAMPLE_HOST "httpbin.org"
#endif /* EXAMPLE_HOST */

#ifndef EXAMPLE_PORT
/*! @brief Port number of the HTTPS server to download data from. */
#define EXAMPLE_PORT 443
#endif /* EXAMPLE_PORT */

#ifndef EXAMPLE_PATH
/*! @brief HTTP path of the resource to download from the HTTPS server. */
#define EXAMPLE_PATH "/get"
#endif /* EXAMPLE_PATH */

#ifndef EXAMPLE_MAX_REQUEST_LENGTH
/*! @brief Maximum length of HTTP request. */
#define EXAMPLE_MAX_REQUEST_LENGTH 256
#endif /* EXAMPLE_MAX_REQUEST_LENGTH */

#ifndef EXAMPLE_MAX_RESPONSE_LENGTH
/*! @brief Maximum length of HTTP response. */
#define EXAMPLE_MAX_RESPONSE_LENGTH 1024
#endif /* EXAMPLE_MAX_RESPONSE_LENGTH */

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#ifndef INIT_THREAD_STACKSIZE
/*! @brief Stack size of the temporary initialization thread in words. */
#define INIT_THREAD_STACKSIZE 1024
#endif /* INIT_THREAD_STACKSIZE */

#ifndef INIT_THREAD_PRIO
/*! @brief Priority of the temporary initialization thread. */
#define INIT_THREAD_PRIO 5
#endif /* INIT_THREAD_PRIO */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if LWIP_IPV6
static void print_ipv6_addresses(struct netif *netif);
static void netif_ipv6_callback(struct netif *cb_netif);
#endif /* LWIP_IPV6 */

static void init_task(void *arg);
static void init_task_init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static phy_handle_t phyHandle;
static struct netif netif;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitHardware();

    init_task_init();

    /* run RTOS */
    vTaskStartScheduler();

    /* should not reach this statement */
    for (;;)
        ;
}

/*!
 * @brief Starts initialization task.
 */
static void init_task_init(void)
{
    /* create server thread in RTOS */
    if (sys_thread_new("init_task", init_task, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("init_task creation failed", 0);
    }
}

/*!
 * @brief Initializes crypto hw, lwIP stack and starts application task.
 */
static void init_task(void *arg)
{
    const ip_addr_t *dns_address;
    const char *str_ip;
    status_t status;
    uint8_t numdns;

    LWIP_UNUSED_ARG(arg);

    ethernetif_config_t enet_config = {.phyHandle   = &phyHandle,
                                       .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                       .phyOps      = EXAMPLE_PHY_OPS,
                                       .phyResource = EXAMPLE_PHY_RESOURCE,
                                       .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
                                       .macAddress = configMAC_ADDR
#endif
    };

    https_client_task_context_t client_context = {
        .host             = EXAMPLE_HOST,
        .port             = EXAMPLE_PORT,
        .path             = EXAMPLE_PATH,
        .max_request_len  = EXAMPLE_MAX_REQUEST_LENGTH,
        .max_response_len = EXAMPLE_MAX_RESPONSE_LENGTH,
    };

    LWIP_PLATFORM_DIAG(("***********************************************************\r\n"));
    LWIP_PLATFORM_DIAG(("lwIP HTTPS client example\r\n"));
    LWIP_PLATFORM_DIAG(("***********************************************************\r\n"));

    status = CRYPTO_InitHardware();
    LWIP_ASSERT("CRYPTO_InitHardware() has failed\r\n", status == kStatus_Success);

    tcpip_init(NULL, NULL);

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

#if LWIP_IPV4
    netifapi_netif_add(&netif, NULL, NULL, NULL, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
#else
    netifapi_netif_add(&netif, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
#endif /* LWIP_IPV4 */
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);
#if LWIP_IPV4
    netifapi_dhcp_start(&netif);
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
    LOCK_TCPIP_CORE();
    netif_create_ip6_linklocal_address(&netif, 1);
    UNLOCK_TCPIP_CORE();
#endif /* LWIP_IPV6 */

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        LWIP_PLATFORM_DIAG(
            ("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n"));
    }

#if LWIP_IPV4
    LWIP_PLATFORM_DIAG(("Getting IPv4 address from DHCP...\r\n"));
    (void)ethernetif_wait_ipv4_valid(&netif, ETHERNETIF_WAIT_FOREVER);
#endif /* LWIP_IPV4 */

    LOCK_TCPIP_CORE();
#if LWIP_IPV4
    LWIP_PLATFORM_DIAG((" IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif.ip_addr)));
    LWIP_PLATFORM_DIAG((" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif.netmask)));
    LWIP_PLATFORM_DIAG((" IPv4 Gateway     : %s\r\n", ipaddr_ntoa(&netif.gw)));
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    set_ipv6_valid_state_cb(netif_ipv6_callback);
    print_ipv6_addresses(&netif);
#endif /* LWIP_IPV6 */

    for (numdns = 0U; numdns < DNS_MAX_SERVERS; numdns++)
    {
        dns_address = dns_getserver(numdns);
        str_ip      = "-";
        if (!ip_addr_isany(dns_address))
        {
            str_ip = ipaddr_ntoa(dns_address);
        }
        LWIP_PLATFORM_DIAG((" DNS Server%u      : %s\r\n", numdns, str_ip));
    }

    LWIP_PLATFORM_DIAG(("***********************************************************\r\n\r\n"));
    UNLOCK_TCPIP_CORE();

    /* Start application task */
    app_https_client_task_init(&client_context);

    vTaskDelete(NULL);
}

#if LWIP_IPV6
static void print_ipv6_addresses(struct netif *netif)
{
    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++)
    {
        const char *str_ip = "-";
        if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)))
        {
            str_ip = ip6addr_ntoa(netif_ip6_addr(netif, i));
        }
        LWIP_PLATFORM_DIAG((" IPv6 Address%d    : %s\r\n", i, str_ip));
    }
}

static void netif_ipv6_callback(struct netif *cb_netif)
{
    LWIP_PLATFORM_DIAG(("\r\nIPv6 address update, valid addresses:\r\n"));
    print_ipv6_addresses(cb_netif);
    LWIP_PLATFORM_DIAG(("\r\n"));
}
#endif /* LWIP_IPV6 */

#endif /* LWIP_SOCKET */
