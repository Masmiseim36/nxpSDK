/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_phyksz8081.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the PHY KSZ8081 vendor defined registers. */
#define PHY_CONTROL1_REG 0x1EU /*!< The PHY control one register. */
#define PHY_CONTROL2_REG 0x1FU /*!< The PHY control two register. */

/*! @brief Defines the PHY KSZ8081 ID number. */
#define PHY_CONTROL_ID1 0x22U /*!< The PHY ID1*/

/*!@brief Defines the mask flag of operation mode in control two register*/
#define PHY_CTL2_REMOTELOOP_MASK    0x0004U /*!< The PHY remote loopback mask. */
#define PHY_CTL2_REFCLK_SELECT_MASK 0x0080U /*!< The PHY RMII reference clock select. */
#define PHY_CTL1_10HALFDUPLEX_MASK  0x0001U /*!< The PHY 10M half duplex mask. */
#define PHY_CTL1_100HALFDUPLEX_MASK 0x0002U /*!< The PHY 100M half duplex mask. */
#define PHY_CTL1_10FULLDUPLEX_MASK  0x0005U /*!< The PHY 10M full duplex mask. */
#define PHY_CTL1_100FULLDUPLEX_MASK 0x0006U /*!< The PHY 100M full duplex mask. */
#define PHY_CTL1_SPEEDUPLX_MASK     0x0007U /*!< The PHY speed and duplex mask. */
#define PHY_CTL1_ENERGYDETECT_MASK  0x10U   /*!< The PHY signal present on rx differential pair. */
#define PHY_CTL1_LINKUP_MASK        0x100U  /*!< The PHY link up. */
#define PHY_LINK_READY_MASK         (PHY_CTL1_ENERGYDETECT_MASK | PHY_CTL1_LINKUP_MASK)

/*! @brief Defines the timeout macro. */
#define PHY_READID_TIMEOUT_COUNT 1000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
const phy_operations_t phyksz8081_ops = {.phyInit            = PHY_KSZ8081_Init,
                                         .phyWrite           = PHY_KSZ8081_Write,
                                         .phyRead            = PHY_KSZ8081_Read,
                                         .getLinkStatus      = PHY_KSZ8081_GetLinkStatus,
                                         .getLinkSpeedDuplex = PHY_KSZ8081_GetLinkSpeedDuplex,
                                         .getAutoNegoStatus  = PHY_KSZ8081_GetAutoNegotiationStatus,
                                         .enableLoopback     = PHY_KSZ8081_EnableLoopback};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t PHY_KSZ8081_Init(phy_handle_t *handle, const phy_config_t *config)
{
    uint32_t counter = PHY_READID_TIMEOUT_COUNT;
    uint32_t idReg   = 0;
    status_t result  = kStatus_Success;

    /* Init MDIO interface. */
    MDIO_Init(handle->mdioHandle);

    /* Assign phy address. */
    handle->phyAddr = config->phyAddr;

    /* Initialization after PHY stars to work. */
    while ((idReg != PHY_CONTROL_ID1) && (counter != 0U))
    {
        MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_ID1_REG, &idReg);
        counter--;
    }

    if (!counter)
    {
        return kStatus_Fail;
    }

    /* Reset PHY. */
    result = MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_BASICCONTROL_REG, PHY_BCTL_RESET_MASK);
    if (result == kStatus_Success)
    {
#if defined(FSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE)
        uint32_t data = 0;
        result        = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_CONTROL2_REG, &data);
        if (result != kStatus_Success)
        {
            return result;
        }
        result =
            MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_CONTROL2_REG, (data | PHY_CTL2_REFCLK_SELECT_MASK));
        if (result != kStatus_Success)
        {
            return result;
        }
#endif /* FSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE */

        /* Set the negotiation. */
        result = MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_AUTONEG_ADVERTISE_REG,
                            (PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
                             PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK | 0x1U));
        if (result == kStatus_Success)
        {
            result = MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_BASICCONTROL_REG,
                                (PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
        }
    }
    return result;
}

status_t PHY_KSZ8081_Write(phy_handle_t *handle, uint32_t phyReg, uint32_t data)
{
    return MDIO_Write(handle->mdioHandle, handle->phyAddr, phyReg, data);
}

status_t PHY_KSZ8081_Read(phy_handle_t *handle, uint32_t phyReg, uint32_t *dataPtr)
{
    return MDIO_Read(handle->mdioHandle, handle->phyAddr, phyReg, dataPtr);
}

status_t PHY_KSZ8081_GetLinkStatus(phy_handle_t *handle, bool *status)
{
    assert(status);

    status_t result = kStatus_Success;
    uint32_t data;

    /* Read the basic status register. */
    result = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_BASICSTATUS_REG, &data);
    if (result == kStatus_Success)
    {
        if (!(PHY_BSTATUS_LINKSTATUS_MASK & data))
        {
            /* link down. */
            *status = false;
        }
        else
        {
            /* link up. */
            *status = true;
        }
    }
    return result;
}

status_t PHY_KSZ8081_GetLinkSpeedDuplex(phy_handle_t *handle, phy_speed_t *speed, phy_duplex_t *duplex)
{
    assert(duplex);

    status_t result = kStatus_Success;
    uint32_t data, ctlReg;

    /* Read the control two register. */
    result = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_CONTROL1_REG, &ctlReg);
    if (result == kStatus_Success)
    {
        data = ctlReg & PHY_CTL1_SPEEDUPLX_MASK;
        if ((PHY_CTL1_10FULLDUPLEX_MASK == data) || (PHY_CTL1_100FULLDUPLEX_MASK == data))
        {
            /* Full duplex. */
            *duplex = kPHY_FullDuplex;
        }
        else
        {
            /* Half duplex. */
            *duplex = kPHY_HalfDuplex;
        }

        data = ctlReg & PHY_CTL1_SPEEDUPLX_MASK;
        if ((PHY_CTL1_100HALFDUPLEX_MASK == data) || (PHY_CTL1_100FULLDUPLEX_MASK == data))
        {
            /* 100M speed. */
            *speed = kPHY_Speed100M;
        }
        else
        { /* 10M speed. */
            *speed = kPHY_Speed10M;
        }
    }
    return result;
}

status_t PHY_KSZ8081_GetAutoNegotiationStatus(phy_handle_t *handle, bool *status)
{
    status_t result;
    uint32_t bssReg;

    *status = false;

    /* Check auto negotiation complete. */
    result = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_BASICSTATUS_REG, &bssReg);
    if (result == kStatus_Success)
    {
        if ((bssReg & PHY_BSTATUS_AUTONEGCOMP_MASK) != 0)
        {
            *status = true;
        }
    }
    return result;
}

status_t PHY_KSZ8081_EnableLoopback(phy_handle_t *handle, phy_loop_t mode, phy_speed_t speed, bool enable)
{
    status_t result;
    uint32_t data = 0;

    /* Set the loop mode. */
    if (enable)
    {
        if (mode == kPHY_LocalLoop)
        {
            if (speed == kPHY_Speed100M)
            {
                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            else
            {
                data = PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            return MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_BASICCONTROL_REG, data);
        }
        else
        {
            /* First read the current status in control register. */
            result = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_CONTROL2_REG, &data);
            if (result == kStatus_Success)
            {
                return MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_CONTROL2_REG,
                                  (data | PHY_CTL2_REMOTELOOP_MASK));
            }
        }
    }
    else
    {
        /* Disable the loop mode. */
        if (mode == kPHY_LocalLoop)
        {
            /* First read the current status in control register. */
            result = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_BASICCONTROL_REG, &data);
            if (result == kStatus_Success)
            {
                data &= ~PHY_BCTL_LOOP_MASK;
                return MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_BASICCONTROL_REG,
                                  (data | PHY_BCTL_RESTART_AUTONEG_MASK));
            }
        }
        else
        {
            /* First read the current status in control one register. */
            result = MDIO_Read(handle->mdioHandle, handle->phyAddr, PHY_CONTROL2_REG, &data);
            if (result == kStatus_Success)
            {
                return MDIO_Write(handle->mdioHandle, handle->phyAddr, PHY_CONTROL2_REG,
                                  (data & ~PHY_CTL2_REMOTELOOP_MASK));
            }
        }
    }
    return result;
}
