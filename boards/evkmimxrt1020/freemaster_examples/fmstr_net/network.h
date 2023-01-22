/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Declarations
 */

#ifndef __FMSTR_NETWORK_H
#define __FMSTR_NETWORK_H

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Definitions
 ******************************************************************************/
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

#else
#error Undefined PHY
#endif

// This constant is typically defined in board.h
#ifndef BOARD_NETWORK_USE_100M_ENET_PORT
#define BOARD_NETWORK_USE_100M_ENET_PORT 1
#endif

// Use 100M when only ENET0 exists
#if !((defined(FSL_FEATURE_SOC_ENET_COUNT) && (FSL_FEATURE_SOC_ENET_COUNT > 1)) || \
    (defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 1)))
#undef BOARD_NETWORK_USE_100M_ENET_PORT
#define BOARD_NETWORK_USE_100M_ENET_PORT 1
#endif

// Force use of ENET4 for RT1180 board
#if defined(CPU_MIMXRT1189CVM8A_cm33) || defined(CPU_MIMXRT1189CVM8A_cm7)
#define EXAMPLE_PHY_ADDRESS   0x03U
#endif

#if BOARD_NETWORK_USE_100M_ENET_PORT

#ifndef EXAMPLE_ETHIF_INIT
#define EXAMPLE_ETHIF_INIT    ethernetif0_init
#endif
#ifndef EXAMPLE_PHY_ADDRESS
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET0_PHY_ADDRESS
#endif

#else

#ifndef EXAMPLE_ETHIF_INIT
#define EXAMPLE_ETHIF_INIT    ethernetif1_init
#endif
#ifndef EXAMPLE_PHY_ADDRESS
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET1_PHY_ADDRESS
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
