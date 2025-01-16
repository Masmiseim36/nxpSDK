/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Code
 */

#include "network.h"
#include "fsl_enet.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if EXAMPLE_NET_SPEED_100M
#define EXAMPLE_MDIO_HANDLE ENET
#else
#define EXAMPLE_MDIO_HANDLE ENET_1G
#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_MDIO_HANDLE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_MDIO_HANDLE, phyAddr, regAddr, pData);
}

void Network_PhyInit(EXAMPLE_PHY_RES *phy_res, phy_operations_t *phy_ops, uint32_t csrClock_Hz)
{
    /* MDIO init*/
    phy_res->write = MDIO_Write;
    phy_res->read = MDIO_Read;

    /* PHY init*/
    phy_ops->phyInit            = EXAMPLE_PHY_PREFIX(Init);
    phy_ops->phyWrite           = EXAMPLE_PHY_PREFIX(Write);
    phy_ops->phyRead            = EXAMPLE_PHY_PREFIX(Read);
    phy_ops->getAutoNegoStatus  = EXAMPLE_PHY_PREFIX(GetAutoNegotiationStatus);
    phy_ops->getLinkStatus      = EXAMPLE_PHY_PREFIX(GetLinkStatus);
    phy_ops->getLinkSpeedDuplex = EXAMPLE_PHY_PREFIX(GetLinkSpeedDuplex);
    phy_ops->setLinkSpeedDuplex = EXAMPLE_PHY_PREFIX(SetLinkSpeedDuplex);
    phy_ops->enableLoopback     = EXAMPLE_PHY_PREFIX(EnableLoopback);

    /* Make sure the Ethernet clock is enabled */
    CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_MDIO_HANDLE)]);

#if defined(FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE) && FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE
    /* Make sure the Ethernet extra clock is enabled */
    CLOCK_EnableClock(s_enetExtraClock[ENET_GetInstance(EXAMPLE_MDIO_HANDLE)]);
#endif

#if ENET_NO_PREAMBLE_PARAM
    ENET_SetSMI(EXAMPLE_MDIO_HANDLE, csrClock_Hz);
#else
    ENET_SetSMI(EXAMPLE_MDIO_HANDLE, csrClock_Hz, false);
#endif
}