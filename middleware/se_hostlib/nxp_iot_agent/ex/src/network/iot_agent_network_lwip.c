/*
 * Copyright 2018, 2019, 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "ax_reset.h"
#include "se_reset_config.h"
#include "sm_timer.h"

#include "board.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

#if defined(MBEDTLS)
#include "ksdk_mbedtls.h"
#endif

#ifndef INC_FREERTOS_H /* Header guard of FreeRTOS */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#endif /* INC_FREERTOS_H */
#include "task.h"

#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip/netifapi.h"
#ifdef EXAMPLE_USE_100M_ENET_PORT
#include "fsl_phyksz8081.h"
#else
#include "fsl_phyrtl8211f.h"
#endif
#include "fsl_enet_mdio.h"
#include <nxp_iot_agent_status.h>

#if defined (LPC_ENET)
/* ENET clock frequency. */
#if defined(CPU_MIMXRT1176DVMAA_cm7)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#elif defined(CPU_MIMXRT1062DVL6A)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
#else
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#endif // CPU_MIMXRT1176DVMAA_cm7

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

#ifdef EXAMPLE_USE_100M_ENET_PORT
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#else
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET1_PHY_ADDRESS
/* PHY operations. */
#define EXAMPLE_PHY_OPS       phyrtl8211f_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif1_init
#endif // EXAMPLE_USE_100M_ENET_PORT

#endif  // (LPC_ENET)


/* MAC address configuration. */
#define configMAC_ADDR {0x04, 0x12, 0x13, 0xB1, 0x11, 0x90}

/* Address of PHY interface. */

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static struct netif fsl_netif0;
static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};


/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/* Facilitate a simple hash for unique MAC Address based on an input 18 byte UID */
#define MAC_HASH(N)  \
    fsl_enet_config0.macAddress[N] = buffer[(N+2)+(5*0)] ^ buffer[(N+2)+(5*1)] ^ buffer[(N+2)+(5*2)]

#define STATIC_IP_ADDRESS 0

#if STATIC_IP_ADDRESS == 1

/* IP address configuration. */
#define configIP_ADDR0 170
#define configIP_ADDR1 253
#define configIP_ADDR2 47
#define configIP_ADDR3 143

#else

/* IP address configuration. */
#define configIP_ADDR0 0
#define configIP_ADDR1 0
#define configIP_ADDR2 0
#define configIP_ADDR3 0

#endif

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static struct netif fsl_netif0;

iot_agent_status_t network_init(void)
{
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
            .phyHandle  = &phyHandle,
            .macAddress = configMAC_ADDR,
    #if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
            .non_dma_memory = non_dma_memory,
    #endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

	mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    tcpip_init(NULL, NULL);

    IP4_ADDR(&fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&fsl_netif0_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&fsl_netif0_gw, 0U, 0U, 0U, 0U);

    if (netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0, EXAMPLE_NETIF_INIT_FN,
        tcpip_input) == NULL) {
        return IOT_AGENT_FAILURE;
    }
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);
#if STATIC_IP_ADDRESS == 0
    if (dhcp_start(&fsl_netif0) != ERR_OK) {
        return IOT_AGENT_FAILURE;
    }

    struct dhcp *dhcp;
    dhcp = (struct dhcp *)netif_get_client_data(&fsl_netif0, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    while (dhcp->state != DHCP_STATE_BOUND)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif
    return IOT_AGENT_SUCCESS;
}
