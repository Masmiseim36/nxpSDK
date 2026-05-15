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

/* Ethernet configuration. */
#define EXAMPLE_PHY_ADDRESS  BOARD_SWT_PORT0_PHY_ADDR
#define EXAMPLE_PHY_OPS      &g_app_phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_yt8521_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

#define EXAMPLE_PHY_INT_PORT BOARD_INITNETPINS_ETH0_INT_B_PERIPHERAL
#define EXAMPLE_PHY_INT_PIN  BOARD_INITNETPINS_ETH0_INT_B_CHANNEL
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
