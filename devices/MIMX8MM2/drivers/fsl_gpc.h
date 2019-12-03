/*
 * Copyright 2017, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_GPC_H_
#define _FSL_GPC_H_

#include "fsl_common.h"

/*!
 * @addtogroup gpc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief GPC driver version 2.0.1. */
#define FSL_GPC_DRIVER_VERSION (MAKE_VERSION(2, 0, 1))
/*@}*/

/*! @brief Total number of the timeslot */
#define GPC_PCG_TIME_SLOT_TOTAL_NUMBER (20U)

/*!< GPC LPM mode definition */
enum _gpc_lpm_mode
{
    kGPC_RunMode  = 0U, /*!< run mode */
    kGPC_WaitMode = 1U, /*!< wait mode */
    kGPC_StopMode = 2U, /*!< stop mode */
};

/*!< PGC ack signal selection  */
enum _gpc_pgc_ack_sel
{
    kGPC_DummyPGCPowerUpAck   = GPC_PGC_ACK_SEL_M4_M4_DUMMY_PGC_PUP_ACK_MASK,   /*!< dummy power up ack signal */
    kGPC_VirtualPGCPowerUpAck = GPC_PGC_ACK_SEL_M4_M4_VIRTUAL_PGC_PUP_ACK_MASK, /*!< virtual pgc power up ack signal */
    kGPC_DummyPGCPowerDownAck = GPC_PGC_ACK_SEL_M4_M4_DUMMY_PGC_PDN_ACK_MASK,   /*!< dummy power down ack signal */
    kGPC_VirtualPGCPowerDownAck =
        GPC_PGC_ACK_SEL_M4_M4_VIRTUAL_PGC_PDN_ACK_MASK, /*!< virtual pgc power down ack signal */
};

/*!< Standby counter which GPC will wait between PMIC_STBY_REQ negation and assertion of PMIC_READY  */
enum _gpc_standby_count
{
    kGPC_StandbyCounter4CkilClk   = 0U, /*!< 4 ckil clocks */
    kGPC_StandbyCounter8CkilClk   = 1U, /*!< 8 ckil clocks */
    kGPC_StandbyCounter16CkilClk  = 2U, /*!< 16 ckil clocks */
    kGPC_StandbyCounter32CkilClk  = 3U, /*!< 32 ckil clocks */
    kGPC_StandbyCounter64CkilClk  = 4U, /*!< 64 ckil clocks */
    kGPC_StandbyCounter128CkilClk = 5U, /*!< 128 ckil clocks */
    kGPC_StandbyCounter256CkilClk = 6U, /*!< 256 ckil clocks */
    kGPC_StandbyCounter512CkilClk = 7U, /*!< 512 ckil clocks */
};

/*!< configuration for enter LPM mode */
typedef struct _gpc_lpm_config
{
    bool enFastWakeUp;          /*!< enable fast wake up from lpm mode */
    bool enCpuClk;              /*!< enable CPU clock when LPM enter */
    bool enVirtualPGCPowerup;   /*!< enable m4 virtual PGC power up with LPM enter */
    bool enVirtualPGCPowerdown; /*!< enable m4 virtual PGC power down with LPM enter */
    bool enWfiMask;             /*!< enable M4 WFI Mask */
    bool enDsmMask;             /*!< enable M4 DSM Mask */
} gpc_lpm_config_t;

/*!< configuration for enter DSM mode */
typedef struct _gpc_dsm_config
{
    bool disableRamLpctl; /*!< Memory can be defined to go to retention mode or not */

    bool enPMICStandBy;         /*!< PMIC can be defined to be stand-by mode or not */
    uint8_t pmicStandByCounter; /*!< PMIC standby counter, reference _gpc_standby_count */
    uint8_t regBypassCounter; /*!< if PMIC standby is request, regulator bypass should be enable, and the counter can be
                                 defined */

} gpc_dsm_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Allow all the IRQ/Events within the charge of GPC.
 *
 * @param base GPC peripheral base address.
 */
static inline void GPC_AllowIRQs(GPC_Type *base)
{
    base->MISC &= ~GPC_MISC_GPC_IRQ_MASK_MASK; /* Events would not be masked. */
}

/*!
 * @brief Disallow all the IRQ/Events within the charge of GPC.
 *
 * @param base GPC peripheral base address.
 */
static inline void GPC_DisallowIRQs(GPC_Type *base)
{
    base->MISC |= GPC_MISC_GPC_IRQ_MASK_MASK; /* Mask all the events. */
}

/*!
 * @brief Get current LPM mode.
 *
 * @param base GPC peripheral base address.
 * @retval lpm mode, reference _gpc_lpm_mode
 */
static inline uint32_t GPC_GetLpmMode(GPC_Type *base)
{
    return base->LPCR_M4 & GPC_LPCR_M4_LPM0_MASK;
}

/*!
 * @brief Enable the IRQ.
 *
 * @param base GPC peripheral base address.
 * @param irqId ID number of IRQ to be enabled, available range is 0-127,reference SOC headerfile IRQn_Type.
 */
void GPC_EnableIRQ(GPC_Type *base, uint32_t irqId);

/*!
 * @brief Disable the IRQ.
 *
 * @param base GPC peripheral base address.
 * @param irqId ID number of IRQ to be disabled, available range is 0-127,reference SOC headerfile IRQn_Type.
 */
void GPC_DisableIRQ(GPC_Type *base, uint32_t irqId);

/*!
 * @brief Get the IRQ/Event flag.
 *
 * @param base GPC peripheral base address.
 * @param irqId ID number of IRQ to be enabled, available range is 0-127,reference SOC headerfile IRQn_Type.
 * @return Indicated IRQ/Event is asserted or not.
 */
bool GPC_GetIRQStatusFlag(GPC_Type *base, uint32_t irqId);

/*!
 * @brief Mask the DSM trigger for M4.
 *
 * @param base GPC peripheral base address.
 * @param enable true to enable mask, false to disable mask.
 */
static inline void GPC_DsmTriggerMask(GPC_Type *base, bool enable)
{
    if (enable)
    {
        base->LPCR_M4 |= GPC_LPCR_M4_MASK_DSM_TRIGGER_MASK;
    }
    else
    {
        base->LPCR_M4 &= ~GPC_LPCR_M4_MASK_DSM_TRIGGER_MASK;
    }
}

/*!
 * @brief Mask the WFI for M4.
 *
 * @param base GPC peripheral base address.
 * @param enable true to enable mask, false to disable mask.
 */
static inline void GPC_WFIMask(GPC_Type *base, bool enable)
{
    if (enable)
    {
        base->LPCR_M4 |= GPC_LPCR_M4_MASK_M4_WFI_MASK;
    }
    else
    {
        base->LPCR_M4 &= ~GPC_LPCR_M4_MASK_M4_WFI_MASK;
    }
}

/*!
 * @brief Select the PGC ACK signal for M4.
 *
 * @param base GPC peripheral base address.
 * @param mask reference _gpc_pgc_ack_sel.
 */
static inline void GPC_SelectPGCAckSignal(GPC_Type *base, uint32_t mask)
{
    base->PGC_ACK_SEL_M4 |= mask;
}

/*!
 * @brief M4 power down request to virtual PGC mask or not.
 *
 * @param base GPC peripheral base address.
 * @param enable true to mask, false to not mask.
 */
static inline void GPC_PowerDownRequestMask(GPC_Type *base, bool enable)
{
    if (enable)
    {
        base->MISC &= ~GPC_MISC_M4_PDN_REQ_MASK_MASK;
    }
    else
    {
        base->MISC |= GPC_MISC_M4_PDN_REQ_MASK_MASK;
    }
}

/*!
 * @brief PGC CPU Mapping to M4 domain.
 *
 * @param base GPC peripheral base address.
 * @param mask mask value reference PGC CPU mapping definition.
 */
static inline void GPC_PGCMappingToM4Domain(GPC_Type *base, uint32_t mask)
{
    base->PGC_CPU_0_1_MAPPING |= mask & 0xFFFD0000U;
}

/*!
 * @brief Time slot configure.
 *
 * @param base GPC peripheral base address.
 * @param slotIndex time slot index.
 * @param value value to be configured
 */
static inline void GPC_TimeSlotConfigureForPUS(GPC_Type *base, uint8_t slotIndex, uint32_t value)
{
    assert(slotIndex < GPC_PCG_TIME_SLOT_TOTAL_NUMBER);

    base->SLT_CFG_PU[slotIndex] |= value;
}

/*!
 * @brief Enter WAIT mode.
 *
 * @param base GPC peripheral base address.
 * @param config lpm mode configurations.
 */
void GPC_EnterWaitMode(GPC_Type *base, gpc_lpm_config_t *config);

/*!
 * @brief Enter STOP mode.
 *
 * @param base GPC peripheral base address.
 * @param config lpm mode configurations.
 */
void GPC_EnterStopMode(GPC_Type *base, gpc_lpm_config_t *config);

/*!
 * @brief GPC init function.
 *
 * @param base GPC peripheral base address.
 * @param powerUpSlot power up slot number.
 * @param powerDownSlot power down slot number.
 */
void GPC_Init(GPC_Type *base, uint32_t powerUpSlot, uint32_t powerDownSlot);

#if defined(__cplusplus)
}
#endif
/*!
 * @}
 */
#endif /* _FSL_GPC_H_ */
