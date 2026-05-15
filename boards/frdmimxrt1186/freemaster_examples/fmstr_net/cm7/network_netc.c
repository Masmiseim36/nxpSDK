/*
 * Copyright 2021-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Code
 */

#include "network.h"
#include "fsl_netc_mdio.h"
#include "fsl_debug_console.h"
#include "fsl_netc_soc.h"
#include "fsl_netc_ierb.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static netc_mdio_handle_t mdio_handle;

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#ifdef NETC_MII_MODE

#define EXAMPLE_MII_MODE       NETC_MII_MODE

#else // Set MII mode based on speed definition

#if EXAMPLE_NET_SPEED_100M
#define EXAMPLE_MII_MODE       kNETC_RmiiMode
#else
#define EXAMPLE_MII_MODE       kNETC_RgmiiMode
#endif

#endif // NETC_MII_MODE

#ifndef EXAMPLE_NETC_LINK_PORT
#define EXAMPLE_NETC_LINK_PORT 4
#endif

#define PHY_PAGE_SELECT_REG    0x1FU /*!< The PHY page select register. */

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/* This does initialization and then reconfigures CRS/DV pin to RXDV signal. */
static status_t PHY_InitConf(phy_handle_t *handle, const phy_config_t *config)
{
    status_t result;
    uint16_t data;

    result = EXAMPLE_PHY_PREFIX(Init)(handle, config);
    if (result != kStatus_Success)
    {
        PRINTF("ERROR: PHY Init returned %d\n", result);
        return result;
    }

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
    if (result != kStatus_Success)
    {
        PRINTF("ERROR: PHY Write (7) page select register returned %d\n", result);
        return result;
    }
    result = PHY_Read(handle, 16, &data);
    if (result != kStatus_Success)
    {
        PRINTF("ERROR: PHY Read returned %d\n", result);
        return result;
    }

    /* CRS/DV pin is RXDV signal. */
    data |= (1U << 2);
    result = PHY_Write(handle, 16, data);
    if (result != kStatus_Success)
    {
        PRINTF("ERROR: PHY Write data returned %d\n", result);
        return result;
    }

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);
    if (result != kStatus_Success)
    {
        PRINTF("ERROR: PHY Write (0) page select register returned %d\n", result);
    }

    return result;
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&mdio_handle, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&mdio_handle, phyAddr, regAddr, pData);
}

void Network_PhyInit(EXAMPLE_PHY_RES *phy_res, phy_operations_t *phy_ops, uint32_t csrClock_Hz)
{
    /* MDIO init*/
    phy_res->write = MDIO_Write;
    phy_res->read = MDIO_Read;

    netc_mdio_config_t mdioConfig = {
        .mdio =
            {
                .type = kNETC_EMdio,
            },
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = csrClock_Hz,
    };

    mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)kNETC_ENETC0EthPort;
    status_t status = NETC_MDIOInit(&mdio_handle, &mdioConfig);
    if (status != kStatus_Success)
    {
        PRINTF("ERROR: NETC_MDIOInit returned %d\n", status);
    }

    /* PHY init*/
    phy_ops->phyInit            = PHY_InitConf;
    phy_ops->phyWrite           = EXAMPLE_PHY_PREFIX(Write);
    phy_ops->phyRead            = EXAMPLE_PHY_PREFIX(Read);
    phy_ops->getAutoNegoStatus  = EXAMPLE_PHY_PREFIX(GetAutoNegotiationStatus);
    phy_ops->getLinkStatus      = EXAMPLE_PHY_PREFIX(GetLinkStatus);
    phy_ops->getLinkSpeedDuplex = EXAMPLE_PHY_PREFIX(GetLinkSpeedDuplex);
    phy_ops->setLinkSpeedDuplex = EXAMPLE_PHY_PREFIX(SetLinkSpeedDuplex);
    phy_ops->enableLoopback     = EXAMPLE_PHY_PREFIX(EnableLoopback);

    if (EXAMPLE_NETC_LINK_PORT < BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_COUNT)
    {
        /* Set RMII or RGMII mode*/
        status = NETC_SocSetMiiMode((netc_soc_link_t)EXAMPLE_NETC_LINK_PORT, EXAMPLE_MII_MODE);
        if (status != kStatus_Success)
        {
            PRINTF("ERROR: Cannot set the MII mode, returned %d\n", status);
        }

        /* Output reference clock for RMII */
        status = NETC_SocSetRmiiRefClk((netc_soc_link_t)EXAMPLE_NETC_LINK_PORT, true);
        if (status != kStatus_Success)
        {
            PRINTF("ERROR: Cannot set RMII clock reference, returned %d\n", status);
        }

        /* Unlock the IERB. It will warm reset whole NETC. */
        if (!NETC_IERBIsLockOver())
        {
            status = NETC_IERBUnlock();
            if (status != kStatus_Success)
            {
                PRINTF("ERROR: Cannot unlock the IERB, returned %d\n", status);
            }
        }

        /* Set link port PHY address */
        status = NETC_SocSetLinkAddr((netc_soc_link_t)EXAMPLE_NETC_LINK_PORT, EXAMPLE_PHY_ADDRESS);
        if (status != kStatus_Success)
        {
            PRINTF("ERROR: Cannot set link address, returned %d\n", status);
        }

        /* Set the access attribute, otherwise MSIX access will be blocked. */
        NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
        NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

        /* Lock the IERB. */
        status = NETC_IERBLock();
        if (status != kStatus_Success)
        {
            PRINTF("ERROR: Cannot lock the IERB, returned %d\n", status);
        }
    }
}