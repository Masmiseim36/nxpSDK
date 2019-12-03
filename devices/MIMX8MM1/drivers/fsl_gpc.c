/*
 * Copyright 2017, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.gpc_2"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * brief GPC init function.
 *
 * param base GPC peripheral base address.
 * param powerUpSlot power up slot number.
 * param powerDownSlot power down slot number.
 */
void GPC_Init(GPC_Type *base, uint32_t powerUpSlot, uint32_t powerDownSlot)
{
    assert(powerUpSlot < GPC_PCG_TIME_SLOT_TOTAL_NUMBER);
    assert(powerDownSlot < GPC_PCG_TIME_SLOT_TOTAL_NUMBER);

    /* Disable all M4 interrupt */
    base->IMR_M4[0U] = GPC_IMR_M4_IMR1_M4_MASK;
    base->IMR_M4[1U] = GPC_IMR_M4_IMR2_M4_MASK;
    base->IMR_M4[2U] = GPC_IMR_M4_IMR3_M4_MASK;
    base->IMR_M4[3U] = GPC_IMR_M4_IMR4_M4_MASK;
    /* not mask M4 power down request */
    base->MISC |= GPC_MISC_M4_PDN_REQ_MASK_MASK;
    /* select virtual PGC ack for M4 */
    base->PGC_ACK_SEL_M4 |= kGPC_VirtualPGCPowerUpAck | kGPC_VirtualPGCPowerDownAck;
    /* slot configurations */
    base->SLT_CFG_PU[powerDownSlot] |=
        GPC_SLT_CFG_PU_M4_PDN_SLOT_CONTROL_MASK | GPC_SLT_CFG_PU_MF_PDN_SLOT_CONTROL_MASK;
    base->SLT_CFG_PU[powerUpSlot] |= GPC_SLT_CFG_PU_M4_PUP_SLOT_CONTROL_MASK | GPC_SLT_CFG_PU_MF_PUP_SLOT_CONTROL_MASK;
    /* mapping M4 PGC power up/down slot to fastmix/megamix PGC power up/down slow */
    base->PGC_CPU_0_1_MAPPING = GPC_PGC_CPU_0_1_MAPPING_MF_M4_DOMAIN_MASK;
}

/*!
 * brief Enable the IRQ.
 *
 * param base GPC peripheral base address.
 * param irqId ID number of IRQ to be enabled, available range is 0-127,reference SOC headerfile IRQn_Type.
 */
void GPC_EnableIRQ(GPC_Type *base, uint32_t irqId)
{
    uint32_t irqRegNum      = irqId / 32U;
    uint32_t irqRegShiftNum = irqId % 32U;

    assert(irqRegNum <= GPC_IMR_M4_COUNT);

    base->IMR_M4[irqRegNum] &= ~(1U << irqRegShiftNum);
}

/*!
 * brief Disable the IRQ.
 *
 * param base GPC peripheral base address.
 * param irqId ID number of IRQ to be disabled, available range is 0-127,reference SOC headerfile IRQn_Type.
 */
void GPC_DisableIRQ(GPC_Type *base, uint32_t irqId)
{
    uint32_t irqRegNum      = irqId / 32U;
    uint32_t irqRegShiftNum = irqId % 32U;

    assert(irqRegNum <= GPC_IMR_M4_COUNT);

    base->IMR_M4[irqRegNum] |= (1U << irqRegShiftNum);
}

/*!
 * brief Get the IRQ/Event flag.
 *
 * param base GPC peripheral base address.
 * param irqId ID number of IRQ to be enabled, available range is 0-127,reference SOC headerfile IRQn_Type.
 * return Indicated IRQ/Event is asserted or not.
 */
bool GPC_GetIRQStatusFlag(GPC_Type *base, uint32_t irqId)
{
    uint32_t isrRegNum      = irqId / 32U;
    uint32_t isrRegShiftNum = irqId % 32U;
    uint32_t ret;

    assert(isrRegNum <= GPC_IMR_M4_COUNT);

    ret = base->ISR_M4[isrRegNum] & (1U << isrRegShiftNum);

    return (1U << isrRegShiftNum) == ret;
}

/*!
 * brief Enter WAIT mode.
 *
 * param base GPC peripheral base address.
 * param config lpm mode configurations.
 */
void GPC_EnterWaitMode(GPC_Type *base, gpc_lpm_config_t *config)
{
    uint32_t lpcr  = (base->LPCR_M4) & (~GPC_LPCR_M4_LPM0_MASK);
    uint32_t slpcr = base->SLPCR;

    if (config != NULL)
    {
        lpcr &= ~(GPC_LPCR_M4_CPU_CLK_ON_LPM_MASK | GPC_LPCR_M4_EN_M4_PUP_MASK | GPC_LPCR_M4_EN_M4_PDN_MASK);
        lpcr |= (config->enCpuClk ? GPC_LPCR_M4_CPU_CLK_ON_LPM_MASK : 0U) |
                (config->enVirtualPGCPowerup ? GPC_LPCR_M4_EN_M4_PUP_MASK : 0U) |
                (config->enVirtualPGCPowerdown ? GPC_LPCR_M4_EN_M4_PDN_MASK : 0U) |
                (config->enWfiMask ? GPC_LPCR_M4_MASK_M4_WFI_MASK : 0U) |
                (config->enDsmMask ? GPC_LPCR_M4_MASK_DSM_TRIGGER_MASK : 0U);
        slpcr &= ~GPC_SLPCR_EN_M4_FASTWUP_WAIT_MODE_MASK;
        slpcr |= config->enFastWakeUp ? GPC_SLPCR_EN_M4_FASTWUP_WAIT_MODE_MASK : 0U;
    }

    base->SLPCR = slpcr;
    /* WAIT mode */
    base->LPCR_M4 = lpcr | kGPC_WaitMode;
}

/*!
 * brief Enter STOP mode.
 *
 * param base GPC peripheral base address.
 * param config lpm mode configurations.
 */
void GPC_EnterStopMode(GPC_Type *base, gpc_lpm_config_t *config)
{
    uint32_t lpcr  = (base->LPCR_M4) & (~GPC_LPCR_M4_LPM0_MASK);
    uint32_t slpcr = base->SLPCR;

    if (config != NULL)
    {
        lpcr &= ~(GPC_LPCR_M4_CPU_CLK_ON_LPM_MASK | GPC_LPCR_M4_EN_M4_PUP_MASK | GPC_LPCR_M4_EN_M4_PDN_MASK);
        lpcr |= (config->enCpuClk ? GPC_LPCR_M4_CPU_CLK_ON_LPM_MASK : 0U) |
                (config->enVirtualPGCPowerup ? GPC_LPCR_M4_EN_M4_PUP_MASK : 0U) |
                (config->enVirtualPGCPowerdown ? GPC_LPCR_M4_EN_M4_PDN_MASK : 0U) |
                (config->enWfiMask ? GPC_LPCR_M4_MASK_M4_WFI_MASK : 0U) |
                (config->enDsmMask ? GPC_LPCR_M4_MASK_DSM_TRIGGER_MASK : 0U);
        slpcr &= ~GPC_SLPCR_EN_M4_FASTWUP_STOP_MODE_MASK;
        slpcr |= config->enFastWakeUp ? GPC_SLPCR_EN_M4_FASTWUP_STOP_MODE_MASK : 0U;
    }

    base->SLPCR = slpcr;
    /* STOP mode */
    base->LPCR_M4 = lpcr | kGPC_StopMode;
}
