/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyyt8521.h"
#include "fsl_msgintr.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* @TEST_ANCHOR */

/* IP address configuration. */
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

/* Netmask configuration. */
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

/* Gateway address configuration. */
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

/* Ethernet configuration. */
#define EXAMPLE_PHY_ADDRESS  BOARD_SWT_PORT0_PHY_ADDR
#define EXAMPLE_PHY_OPS      &g_app_phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_yt8521_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)
/*${macro:end}*/

extern const phy_operations_t g_app_phyyt8521_ops;
extern phy_yt8521_resource_t g_phy_yt8521_resource;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
