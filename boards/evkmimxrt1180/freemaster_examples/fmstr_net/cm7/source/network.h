/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Declarations
 */

#ifndef __FMSTR_NETWORK_H
#define __FMSTR_NETWORK_H

#include "board.h"
#include "network_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* PHY */
#ifdef PHY_RTL8201

#include "fsl_phyrtl8201.h"
#ifndef EXAMPLE_PHY_RES
#define EXAMPLE_PHY_RES       phy_rtl8201_resource_t
#endif
#ifndef EXAMPLE_PHY_PREFIX
#define EXAMPLE_PHY_PREFIX(m) PHY_RTL8201_##m
#endif

#elif defined(PHY_KSZ8081)

#include "fsl_phyksz8081.h"
#ifndef EXAMPLE_PHY_RES
#define EXAMPLE_PHY_RES       phy_ksz8081_resource_t
#endif
#ifndef EXAMPLE_PHY_PREFIX
#define EXAMPLE_PHY_PREFIX(m) PHY_KSZ8081_##m
#endif

#elif defined(PHY_RTL8211F)

#include "fsl_phyrtl8211f.h"
#ifndef EXAMPLE_PHY_RES
#define EXAMPLE_PHY_RES       phy_rtl8211f_resource_t
#endif
#ifndef EXAMPLE_PHY_PREFIX
#define EXAMPLE_PHY_PREFIX(m) PHY_RTL8211F_##m
#endif

#elif defined(PHY_LAN8741)

#include "fsl_phylan8741.h"
#ifndef EXAMPLE_PHY_RES
#define EXAMPLE_PHY_RES       phy_lan8741_resource_t
#endif
#ifndef EXAMPLE_PHY_PREFIX
#define EXAMPLE_PHY_PREFIX(m) PHY_LAN8741_##m
#endif

#else
#error Undefined PHY
#endif

/* NET speed */
#ifndef EXAMPLE_NET_SPEED_100M

#if (!defined(BOARD_NETWORK_USE_100M_ENET_PORT) || (BOARD_NETWORK_USE_100M_ENET_PORT == 1))
#define EXAMPLE_NET_SPEED_100M 1
#else
#define EXAMPLE_NET_SPEED_100M 0
#endif

#endif

/* PHY address and init function based on speed */
#ifndef EXAMPLE_PHY_ADDRESS

#if !(EXAMPLE_NET_SPEED_100M) && \
    ((defined(FSL_FEATURE_SOC_ENET_COUNT) && (FSL_FEATURE_SOC_ENET_COUNT > 1)) || \
    (defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 1)))

#define EXAMPLE_PHY_ADDRESS   BOARD_ENET1_PHY_ADDRESS
#define EXAMPLE_ETHIF_INIT    ethernetif1_init
#else
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_ETHIF_INIT    ethernetif0_init
#endif

#endif

/******************************************************************************
 * Functions definitions
 ******************************************************************************/

/* Network example application functions */
void Network_Init(uint32_t csrClock_Hz);
void Network_Poll(void);

/* Network init internal function */
void Network_PhyInit(EXAMPLE_PHY_RES *phy_res, phy_operations_t *phy_ops, uint32_t csrClock_Hz);

#ifdef __cplusplus
}
#endif

#endif /* __FMSTR_NETWORK_H */
