/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NETWORK_CFG_H_
#define _NETWORK_CFG_H_

/*${header:start}*/
#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* IP address configuration. */
#define IP_ADDR "192.168.0.102"

/* Netmask configuration. */
#define IP_MASK "255.255.255.0"

/* Gateway address configuration. */
#define GW_ADDR "192.168.0.100"

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops

/* Network interface initialization function. */
#ifndef EXAMPLE_NETIF_INIT_FN
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
/*${macro:end}*/

#endif /* _NETWORK_CFG_H_ */
