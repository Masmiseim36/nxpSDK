/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Code
 */

#include "network.h"
#include "fsl_netc_mdio.h"
#include "fsl_debug_console.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static netc_mdio_handle_t mdio_handle;

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define EXAMPLE_SWT_PORT0_PHY_ADDR BOARD_SWT_PORT0_PHY_ADDR
#define EXAMPLE_SWT_PORT1_PHY_ADDR BOARD_SWT_PORT1_PHY_ADDR

#define PHY_PAGE_SELECT_REG        0x1FU /*!< The PHY page select register. */

#define EXAMPLE_MDIO_HANDLE &mdio_handle

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
    return NETC_MDIOWrite(EXAMPLE_MDIO_HANDLE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(EXAMPLE_MDIO_HANDLE, phyAddr, regAddr, pData);
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
    status_t status = NETC_MDIOInit(EXAMPLE_MDIO_HANDLE, &mdioConfig);
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

    /* RMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[4] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);

    /* RGMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[1] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(2);

    /* Output reference clock for RMII */
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT0_RMII_REF_CLK_DIR_MASK |
                                              BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT4_RMII_REF_CLK_DIR_MASK;

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    if ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
        PRINTF("ERROR: Cannot unlock the IERB\n");
    }

    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked. */
    NETC_IERB->L0BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT0_PHY_ADDR);
    NETC_IERB->L1BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT1_PHY_ADDR);
    NETC_IERB->L4BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_PHY_ADDRESS);

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    if ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
        PRINTF("ERROR: Cannot lock the IERB\n");
    }
}