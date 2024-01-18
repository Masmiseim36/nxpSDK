/*
 * Copyright 2018-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "board.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include <nxp_iot_agent_status.h>
#include "fsl_debug_console.h"

#ifndef RW612_SERIES
#include "ax_reset.h"
#include "se_reset_config.h"
#include "sm_timer.h"
#endif

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
#include "ethernetif.h"
#include "lwip/netifapi.h"
#ifdef EXAMPLE_USE_100M_ENET_PORT
#include "fsl_phyksz8081.h"
#else
#include "fsl_phyrtl8211f.h"
#endif

#include <nxp_iot_agent_status.h>

#if defined (LPC_ENET)
/* ENET clock frequency. */
#if defined(CPU_MIMXRT1176DVMAA_cm7)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#elif defined(CPU_MIMXRT1062DVL6A)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
#elif defined(RW612_SERIES)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetMainClkFreq()
#else
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#endif // CPU_MIMXRT1176DVMAA_cm7

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

#if defined(CPU_MIMXRT1062DVL6A) || defined (RW612_SERIES)
#include "fsl_enet.h"
#include "fsl_phyksz8081.h"
phy_ksz8081_resource_t g_phy_resource_agent;
#endif

#if defined(CPU_MIMXRT1176DVMAA_cm7)
#include "fsl_enet.h"
#include "fsl_phyrtl8211f.h"
phy_rtl8211f_resource_t g_phy_resource_agent;
#endif

#ifdef EXAMPLE_USE_100M_ENET_PORT
#define EXAMPLE_ENET ENET
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

phy_ksz8081_resource_t g_phy_resource_agent;
/* PHY operations. */
#define EXAMPLE_PHY_OPS &phyksz8081_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource_agent

/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#else
#define EXAMPLE_ENET          ENET_1G
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET1_PHY_ADDRESS
/* PHY operations. */
#define EXAMPLE_PHY_OPS       &phyrtl8211f_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif1_init
phy_rtl8211f_resource_t g_phy_resource_agent;
#endif // EXAMPLE_USE_100M_ENET_PORT
#define EXAMPLE_PHY_RESOURCE &g_phy_resource_agent

#endif  // (LPC_ENET)

#if defined (RW612_SERIES)
#include "fsl_silicon_id.h"
#else
#define configMAC_ADDR {0x04, 0x12, 0x13, 0xB1, 0x11, 0x90}
#endif

/* Address of PHY interface. */

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static struct netif fsl_netif0;
static phy_handle_t phyHandle;

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

#if defined(CPU_MIMXRT1062DVL6A) || defined(CPU_MIMXRT1176DVMAA_cm7) || defined(RW612_SERIES)
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}
#endif

iot_agent_status_t network_init(void)
{
    ethernetif_config_t fsl_enet_config0 = {.phyHandle   = &phyHandle,
                                            .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                            .phyOps      = EXAMPLE_PHY_OPS,
                                            .phyResource = EXAMPLE_PHY_RESOURCE,
                                            .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
                                            .macAddress = configMAC_ADDR
#endif
    };
#if defined(CPU_MIMXRT1062DVL6A) || defined(CPU_MIMXRT1176DVMAA_cm7) || defined(RW612_SERIES)
    MDIO_Init();
    g_phy_resource_agent.read  = MDIO_Read;
    g_phy_resource_agent.write = MDIO_Write;
#endif
	    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&fsl_enet_config0.macAddress);
#endif

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&fsl_netif0, NULL, NULL, NULL, &fsl_enet_config0, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    netifapi_netif_set_default(&fsl_netif0);
    netifapi_netif_set_up(&fsl_netif0);

    while (ethernetif_wait_linkup(&fsl_netif0, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }
#if STATIC_IP_ADDRESS == 0
    if (netifapi_dhcp_start(&fsl_netif0) != ERR_OK) {
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

