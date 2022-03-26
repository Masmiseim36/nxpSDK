/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NETWORK_BOARD_CFG_H_
#define _NETWORK_BOARD_CFG_H_

/*${header:start}*/
#if BOARD_NETWORK_USE_100M_ENET_PORT
#include "fsl_phyksz8081.h"
#else
#include "fsl_phyrtl8211f.h"
#endif
#include "fsl_enet_mdio.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* @TEST_ANCHOR */

#if BOARD_NETWORK_USE_100M_ENET_PORT
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
#endif

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)

/*${macro:end}*/
#endif /* _NETWORK_BOARD_CFG_H_ */
