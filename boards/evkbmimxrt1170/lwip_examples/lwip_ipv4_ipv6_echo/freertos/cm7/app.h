/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/

#include "fsl_phyrtl8201.h"
#include "fsl_phyrtl8211f.h"

/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* @TEST_ANCHOR */

/* IP address configuration port 0. */
#ifndef configIP_ADDR0
#define configIP_ADDR0 192
#endif
#ifndef configIP_ADDR1
#define configIP_ADDR1 168
#endif
#ifndef configIP_ADDR2
#define configIP_ADDR2 0
#endif
#ifndef configIP_ADDR3
#define configIP_ADDR3 102
#endif

/* Netmask configuration port 0. */
#ifndef configNET_MASK0
#define configNET_MASK0 255
#endif
#ifndef configNET_MASK1
#define configNET_MASK1 255
#endif
#ifndef configNET_MASK2
#define configNET_MASK2 255
#endif
#ifndef configNET_MASK3
#define configNET_MASK3 0
#endif

/* Gateway address configuration port 0. */
#ifndef configGW_ADDR0
#define configGW_ADDR0 192
#endif
#ifndef configGW_ADDR1
#define configGW_ADDR1 168
#endif
#ifndef configGW_ADDR2
#define configGW_ADDR2 0
#endif
#ifndef configGW_ADDR3
#define configGW_ADDR3 100
#endif

/* IP address configuration port 1. */
#ifndef configIP1_ADDR0
#define configIP1_ADDR0 192
#endif
#ifndef configIP1_ADDR1
#define configIP1_ADDR1 168
#endif
#ifndef configIP1_ADDR2
#define configIP1_ADDR2 1
#endif
#ifndef configIP1_ADDR3
#define configIP1_ADDR3 103
#endif

/* Netmask configuration port 1. */
#ifndef configNET1_MASK0
#define configNET1_MASK0 255
#endif
#ifndef configNET1_MASK1
#define configNET1_MASK1 255
#endif
#ifndef configNET1_MASK2
#define configNET1_MASK2 255
#endif
#ifndef configNET1_MASK3
#define configNET1_MASK3 0
#endif

/* Gateway address configuration port 1. */
#ifndef configGW1_ADDR0
#define configGW1_ADDR0 192
#endif
#ifndef configGW1_ADDR1
#define configGW1_ADDR1 168
#endif
#ifndef configGW1_ADDR2
#define configGW1_ADDR2 1
#endif
#ifndef configGW1_ADDR3
#define configGW1_ADDR3 200
#endif

#define EXAMPLE_ENET        ENET
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_PHY_OPS     &phyrtl8201_ops
extern phy_rtl8201_resource_t g_phy_resource;
#define EXAMPLE_PHY_RESOURCE  &g_phy_resource
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

#define EXAMPLE_ENET_1G      ENET_1G
#define EXAMPLE_PHY1_ADDRESS BOARD_ENET1_PHY_ADDRESS
#define EXAMPLE_PHY1_OPS     &phyrtl8211f_ops
extern phy_rtl8211f_resource_t g_phy1_resource;
#define EXAMPLE_PHY1_RESOURCE  &g_phy1_resource
#define EXAMPLE_NETIF1_INIT_FN ethernetif1_init
#define configMAC_ADDR1                    \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x12 \
    }

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)

/* Tell the app to build 2 network interface configurations */
#define BOARD_NETWORK_USE_DUAL_ENET

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
