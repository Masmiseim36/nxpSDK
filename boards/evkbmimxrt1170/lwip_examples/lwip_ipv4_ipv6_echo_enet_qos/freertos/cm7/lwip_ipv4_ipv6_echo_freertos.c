/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020, 2022-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"

#include "tcpecho.h"
#include "lwip/netifapi.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

#include "board.h"
#include "app.h"
#include "fsl_phy.h"

#include "fsl_component_serial_manager.h"
#include "fsl_shell.h"

#include "lwip/sockets.h"

#include "shell_task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Must be after include of app.h */
#if !defined(configMAC_ADDR) || !defined(configMAC_ADDR1)
#include "fsl_silicon_id.h"
#endif

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 1024

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

#ifndef EXAMPLE_NETIF_INIT_FN
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif

#if defined(BOARD_NETWORK_USE_DUAL_ENET)
#define BOARD_PHY_COUNT 2
#else
#define BOARD_PHY_COUNT 1
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static phy_handle_t phyHandle;
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
static phy_handle_t phyHandle1;

#if LWIP_SINGLE_NETIF == 1
#error \
    "Single netif limitation in lwIP must be disabled if this example were to use both interfaces. (LWIP_SINGLE_NETIF = 0)"
#endif // LWIP_SINGLE_NETIF == 1
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initializes lwIP stack.
 *
 * @param arg unused
 */
static void stack_init(void *arg)
{
    LWIP_UNUSED_ARG(arg);

    ip4_addr_t netif0_ipaddr, netif0_netmask, netif0_gw;
    static struct netif s_netif0;
    ethernetif_config_t enet0_config = {.phyHandle   = &phyHandle,
                                        .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                        .phyOps      = EXAMPLE_PHY_OPS,
                                        .phyResource = EXAMPLE_PHY_RESOURCE,
                                        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
                                        .macAddress = configMAC_ADDR
#endif
    };
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet0_config.macAddress);
#endif

    tcpip_init(NULL, NULL);

    IP4_ADDR(&netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);
    netifapi_netif_add(&s_netif0, &netif0_ipaddr, &netif0_netmask, &netif0_gw, &enet0_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_up(&s_netif0);

#if defined(BOARD_NETWORK_USE_DUAL_ENET)
    ip4_addr_t netif1_ipaddr, netif1_netmask, netif1_gw;
    ethernetif_config_t enet1_config = {.phyHandle   = &phyHandle1,
                                        .phyAddr     = EXAMPLE_PHY1_ADDRESS,
                                        .phyOps      = EXAMPLE_PHY1_OPS,
                                        .phyResource = EXAMPLE_PHY1_RESOURCE,
                                        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR1
                                        .macAddress = configMAC_ADDR1
#endif
    };
    static struct netif s_netif1;
#ifndef configMAC_ADDR1
    (void)SILICONID_ConvertToMacAddr(&enet1_config.macAddress);
#endif
    IP4_ADDR(&netif1_ipaddr, configIP1_ADDR0, configIP1_ADDR1, configIP1_ADDR2, configIP1_ADDR3);
    IP4_ADDR(&netif1_netmask, configNET1_MASK0, configNET1_MASK1, configNET1_MASK2, configNET1_MASK3);
    IP4_ADDR(&netif1_gw, configGW1_ADDR0, configGW1_ADDR1, configGW1_ADDR2, configGW1_ADDR3);
    netifapi_netif_add(&s_netif1, &netif1_ipaddr, &netif1_netmask, &netif1_gw, &enet1_config, EXAMPLE_NETIF1_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_up(&s_netif1);
#else
    /*
     * Single netif is used, set is as default to avoid
     * the need to append zone indices to link-local IPv6 addresses.
     */
    netifapi_netif_set_default(&s_netif0);
#endif /* defined(BOARD_NETWORK_USE_DUAL_ENET) */

    LOCK_TCPIP_CORE();
    netif_create_ip6_linklocal_address(&s_netif0, 1);
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
    netif_create_ip6_linklocal_address(&s_netif1, 1);
#endif
    UNLOCK_TCPIP_CORE();

    struct netif *netif_array[BOARD_PHY_COUNT];
    netif_array[0] = &s_netif0;
#if defined(BOARD_NETWORK_USE_DUAL_ENET)
    netif_array[1] = &s_netif1;
#endif

    while (ethernetif_wait_linkup_array(netif_array, BOARD_PHY_COUNT, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    shell_task_init(NULL, 0);

    vTaskDelete(NULL);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    /* Initialize lwIP from thread */
    if (sys_thread_new("main", stack_init, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("main(): Task creation failed.", 0);
    }

    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    return 0;
}
