/*
 * Copyright 2017-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the timeout macro. */
#define PHY_TIMEOUT_COUNT 100000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the ENET instance from peripheral base address.
 *
 * @param base ENET peripheral base address.
 * @return ENET instance.
 */
extern uint32_t ENET_GetInstance(ENET_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to enet clocks for each instance. */
extern clock_ip_name_t s_enetClock[FSL_FEATURE_SOC_ENET_COUNT];
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t PHY_Init(ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz)
{
    uint32_t bssReg  = 0;
    uint32_t counter = PHY_TIMEOUT_COUNT;
    uint32_t idReg   = 0;
    status_t result  = kStatus_Success;
    uint32_t timeDelay;
    uint32_t instance = ENET_GetInstance(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Set SMI first. */
    CLOCK_EnableClock(s_enetClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    ENET_SetSMI(base, srcClock_Hz, false);

    /* Initialization after PHY stars to work. */
    while ((idReg != PHY_CONTROL_ID1) && (counter != 0))
    {
        PHY_Read(base, phyAddr, PHY_ID1_REG, &idReg);
        counter--;
    }


    if (!counter)
    {
        return kStatus_Fail;
    }

    /* Configure RMII voltage 1.8V */
    result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, 0x1F);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, 0x8);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Reset PHY. */
    counter = PHY_TIMEOUT_COUNT;
    result  = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, PHY_BCTL_RESET_MASK);
    if (result == kStatus_Success)
    {
        /* Close smartEEE. */
        result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_CONTROL_REG, PHY_MMD_DEVICEID3);
        if (result == kStatus_Success)
        {
            result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_DATA_REG, PHY_MMD_SMARTEEE_CTL_OFFSET);
            if (result == kStatus_Success)
            {
                result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_CONTROL_REG, PHY_MMD_DEVICEID3 | 0x4000);
                if (result == kStatus_Success)
                {
                    PHY_Read(base, phyAddr, PHY_MMD_ACCESS_DATA_REG, &bssReg);
                    PHY_Write(base, phyAddr, PHY_MMD_ACCESS_DATA_REG,
                              (bssReg & ~((uint32_t)1 << PHY_MMD_SMARTEEE_LPI_EN_SHIFT)));
                }
            }
        }
        /* Enable Tx clock delay */
        result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, 0x5);
        if (result != kStatus_Success)
        {
            return result;
        }
        result = PHY_Read(base, phyAddr, PHY_DEBUGPORT_DATA_REG, &bssReg);
        if (result != kStatus_Success)
        {
            return result;
        }
        result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, (bssReg | 0x0100));
        if (result != kStatus_Success)
        {
            return result;
        }

        /* Enable Rx clock delay */
        result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, 0x0);
        if (result != kStatus_Success)
        {
            return result;
        }
        result = PHY_Read(base, phyAddr, PHY_DEBUGPORT_DATA_REG, &bssReg);
        if (result != kStatus_Success)
        {
            return result;
        }
        result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, (bssReg | 0x8000));
        if (result != kStatus_Success)
        {
            return result;
        }

        /* Set the negotiation. */
        result = PHY_Write(base, phyAddr, PHY_AUTONEG_ADVERTISE_REG,
                           (PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
                            PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK | 0x1U));
        if (result == kStatus_Success)
        {
            result = PHY_Write(base, phyAddr, PHY_1000BASET_CONTROL_REG,
                               PHY_1000BASET_FULLDUPLEX_MASK | PHY_1000BASET_HALFDUPLEX_MASK);
            if (result == kStatus_Success)
            {
                result = PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &bssReg);

                result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG,
                                   (bssReg | PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
                if (result == kStatus_Success)
                {
                    /* Check auto negotiation complete. */
                    while (counter--)
                    {
                        result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &bssReg);
                        if (result == kStatus_Success)
                        {
                            if ((bssReg & PHY_BSTATUS_AUTONEGCOMP_MASK) != 0)
                            {
                                /* Wait a moment for Phy status stable due to Auto complete takes about 3s ~ 4s. */
                                for (timeDelay = 0; timeDelay < PHY_TIMEOUT_COUNT; timeDelay++)
                                {
                                    __ASM("nop");
                                }
                                break;
                            }
                        }

                        if (!counter)
                        {
                            return kStatus_PHY_AutoNegotiateFail;
                        }
                    }
                }
            }
        }
    }

    return result;
}

status_t PHY_Write(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t data)
{
    uint32_t counter;

    /* Clear the SMI interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI write command. */
    ENET_StartSMIWrite(base, phyAddr, phyReg, kENET_MiiWriteValidFrame, data);

    /* Wait for SMI complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_Read(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t *dataPtr)
{
    assert(dataPtr);

    uint32_t counter;

    /* Clear the MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI read command operation. */
    ENET_StartSMIRead(base, phyAddr, phyReg, kENET_MiiReadValidFrame);

    /* Wait for MII complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Get data from MII register. */
    *dataPtr = ENET_ReadSMIData(base);

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_EnableLoopback(ENET_Type *base, uint32_t phyAddr, phy_loop_t mode, phy_speed_t speed, bool enable)
{
    status_t result;
    uint32_t data = 0;

    /* Set the loop mode. */
    if (enable)
    {
        if (mode == kPHY_LocalLoop)
        {
            if (speed == kPHY_Speed1000M)
            {
                data = PHY_BCTL_SPEED_1000M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            else if (speed == kPHY_Speed100M)
            {
                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            else
            {
                data = PHY_BCTL_DUPLEX_MASK | PHY_BCTL_LOOP_MASK;
            }
            return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);
        }
        else if (mode == kPHY_RemoteLoop)
        {
            /* First read the current status in control register. */
            result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_CONTROL_REG, PHY_MMD_DEVICEID3);
            if (result == kStatus_Success)
            {
                result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_DATA_REG, PHY_MMD_REMOTEPHY_LOOP_OFFSET);
                if (result == kStatus_Success)
                {
                    result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_CONTROL_REG, PHY_MMD_DEVICEID3 | 0x4000);
                    if (result == kStatus_Success)
                    {
                        return PHY_Write(base, phyAddr, PHY_MMD_ACCESS_DATA_REG, 1U);
                    }
                }
            }
        }
        else
        {
            /* First read the current status in control register. */
            result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, PHY_DEBUG_HIBECTL_REG_OFFSET);
            if (result == kStatus_Success)
            {
                result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, 0);
                if (result == kStatus_Success)
                {
                    result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, PHY_DEBUG_EXTLOOP_REG_OFFSET);
                    if (result == kStatus_Success)
                    {
                        result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, 1);
                        if (result == kStatus_Success)
                        {
                            if (speed == kPHY_Speed1000M)
                            {
                                data = PHY_BCTL_SPEED_1000M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_RESET_MASK;
                            }
                            else if (speed == kPHY_Speed100M)
                            {
                                data = PHY_BCTL_SPEED_100M_MASK | PHY_BCTL_DUPLEX_MASK | PHY_BCTL_RESET_MASK;
                            }
                            else
                            {
                                data = PHY_BCTL_DUPLEX_MASK | PHY_BCTL_RESET_MASK;
                            }
                            return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);
                        }
                    }
                }
            }
        }
    }
    else
    {
        /* Disable the loop mode. */
        if (mode == kPHY_LocalLoop)
        {
            /* First read the current status in control register. */
            result = PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &data);
            if (result == kStatus_Success)
            {
                data &= ~PHY_BCTL_LOOP_MASK;
                return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, (data | PHY_BCTL_RESTART_AUTONEG_MASK));
            }
        }
        else if (mode == kPHY_RemoteLoop)
        {
            /* First read the current status in control register. */
            result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_CONTROL_REG, PHY_MMD_DEVICEID3);
            if (result == kStatus_Success)
            {
                result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_DATA_REG, PHY_MMD_REMOTEPHY_LOOP_OFFSET);
                if (result == kStatus_Success)
                {
                    result = PHY_Write(base, phyAddr, PHY_MMD_ACCESS_CONTROL_REG, PHY_MMD_DEVICEID3 | 0x4000);
                    if (result == kStatus_Success)
                    {
                        return PHY_Write(base, phyAddr, PHY_MMD_ACCESS_DATA_REG, 0x0U);
                    }
                }
            }
        }
        else
        {
            /* First read the current status in control register. */
            result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, PHY_DEBUG_HIBECTL_REG_OFFSET);
            if (result == kStatus_Success)
            {
                result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, 0);
                if (result == kStatus_Success)
                {
                    result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_ADDR_REG, PHY_DEBUG_EXTLOOP_REG_OFFSET);
                    if (result == kStatus_Success)
                    {
                        result = PHY_Write(base, phyAddr, PHY_DEBUGPORT_DATA_REG, 0);
                        if (result == kStatus_Success)
                        {
                            data = PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESET_MASK;
                            return PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, data);
                        }
                    }
                }
            }
        }
    }
    return result;
}

status_t PHY_GetLinkStatus(ENET_Type *base, uint32_t phyAddr, bool *status)
{
    assert(status);

    status_t result = kStatus_Success;
    uint32_t data;

    /* Read the basic status register. */
    result = PHY_Read(base, phyAddr, PHY_SPECIFIC_STATUS_REG, &data);
    if (result == kStatus_Success)
    {
        if (!(PHY_SSTATUS_LINKSTATUS_MASK & data))
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

status_t PHY_GetLinkSpeedDuplex(ENET_Type *base, uint32_t phyAddr, phy_speed_t *speed, phy_duplex_t *duplex)
{
    assert(duplex);

    status_t result = kStatus_Success;
    uint32_t statReg;

    /* Read the control two register. */
    result = PHY_Read(base, phyAddr, PHY_SPECIFIC_STATUS_REG, &statReg);
    if (result == kStatus_Success)
    {
        if ((statReg & PHY_SSTATUS_LINKDUPLEX_MASK))
        {
            /* Full duplex. */
            *duplex = kPHY_FullDuplex;
        }
        else
        {
            /* Half duplex. */
            *duplex = kPHY_HalfDuplex;
        }

        switch ((statReg & PHY_SSTATUS_LINKSPEED_MASK) >> PHY_SSTATUS_LINKSPEED_SHIFT)
        {
            case kPHY_Speed10M:
                *speed = kPHY_Speed10M;
                break;
            case kPHY_Speed100M:
                *speed = kPHY_Speed100M;
                break;
            case kPHY_Speed1000M:
                *speed = kPHY_Speed1000M;
                break;
            default:
                *speed = kPHY_Speed10M;
                break;
        }
    }

    return result;
}
