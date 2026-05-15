/*
 * Copyright 2018-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NETWORK_BOARD_CFG_H_
#define _NETWORK_BOARD_CFG_H_

#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyyt8521.h"
#include "fsl_msgintr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */

/* IP address configuration. */
#ifndef IP_ADDR
#define IP_ADDR "192.168.0.102"
#endif

/* Netmask configuration. */
#ifndef IP_MASK
#define IP_MASK "255.255.255.0"
#endif

/* Gateway address configuration. */
#ifndef GW_ADDR
#define GW_ADDR "192.168.0.100"
#endif

/* Ethernet configuration. */

#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x00, 0xfa, 0xfa, 0xdd, 0x05 \
    }


#define EXAMPLE_PHY_ADDRESS  BOARD_SWT_PORT0_PHY_ADDR
#define EXAMPLE_PHY_OPS      &g_app_phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_yt8521_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern const phy_operations_t g_app_phyyt8521_ops;
extern phy_yt8521_resource_t g_phy_yt8521_resource;

#endif /* _NETWORK_BOARD_CFG_H_ */
