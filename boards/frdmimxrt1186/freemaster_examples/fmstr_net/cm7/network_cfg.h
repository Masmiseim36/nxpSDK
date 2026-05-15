/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Definitions
 */

#ifndef __FMSTR_NETWORK_CFG_H
#define __FMSTR_NETWORK_CFG_H

/******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_PHY_ADDRESS     BOARD_SWT_PORT0_PHY_ADDR
#define EXAMPLE_ETHIF_INIT      ethernetif0_init
#define EXAMPLE_NETC_LINK_PORT  kNETC_SocLinkSwitchPort0

#define NETC_USE_SWT            1
#define NETC_MII_MODE           kNETC_RgmiiMode
#define NETC_MII_SPEED          kNETC_MiiSpeed1000M

#endif /* __FMSTR_NETWORK_CFG_H */