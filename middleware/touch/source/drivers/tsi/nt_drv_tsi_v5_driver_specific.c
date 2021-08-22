/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>
#include "fsl_tsi_v5.h"
#include "nt_drv_tsi_driver.h"
#include "nt_module_tsi.h"
#include "../source/system/nt_system_prv.h"
#include "../source/modules/nt_modules_prv.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#define NT_MODULE_TSI_NAME "nt_module_tsi_interface"
#if FSL_FEATURE_SOC_TSI_COUNT
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * variable definition for obtaining flash start and end adress to check if configuration is located in flash or ram
 */

#ifndef NT_FLASH_START
#ifndef NT_FLASH_END
#if defined(__IAR_SYSTEMS_ICC__) /* For IAR compiler   */
#pragma section = ".rodata"
uint32_t flash_start, flash_end;
#elif defined(__CC_ARM)                                 /* For ARM(KEIL) version < 6 compiler */
#elif defined(__MCUXPRESSO)                             /* For GCC compiler  MCUX IDE */
uint32_t flash_start;
uint32_t flash_end;
uint32_t __base_PROGRAM_FLASH;
uint32_t __top_PROGRAM_FLASH;
#elif defined(__GNUC__) && (__ARMCC_VERSION == 0)       /* For ARMGCC compiler */
uint32_t flash_start;
uint32_t flash_end;
#elif defined(__GNUC__) && (__ARMCC_VERSION >= 6010050) /* For ARM(KEIL) version >= 60 compiler */
#else                                                   /* Other compiler used */
#warning "Unsupported compiler/IDE used !"
#endif
#endif /* NT_FLASH_END */
#endif /* NT_FLASH_START */

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile tsi_lpwr_status_flags_t tsi_lpwr_status;

/******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_Init
 * Description   : Encapsulates TSI v5 init.
 *
 *END**************************************************************************/
void NT_TSI_DRV_InitSpecific(TSI_Type *base, const tsi_config_t *config)
{
    TSI_InitSelfCapMode(base, &config->configSelfCap);
    TSI_EnableModule(base, true);
#if (defined(FSL_FEATURE_TSI_HAS_M_TX_USED) && FSL_FEATURE_TSI_HAS_M_TX_USED)
    /* Clear M_TX_USED bitfield, reset values are 1 values*/
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_0);
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_1);
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_2);
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_3);
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_4);
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_5);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_EnableElectrode
 * Description   : Enables/Disables the electrode for measuring.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_EnableElectrode(uint32_t instance, const uint32_t channel, const bool enable)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(channel < TF_TSI_TOTAL_CHANNEL_COUNT);

    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return tsiState->status;
    }

    /* Check the condition for low power mode. */
    if ((tsiState->opMode == tsi_OpModeLowPower) || (tsiState->opMode == tsi_OpModeProximity))
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes = 0;
    }

    if (enable)
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes |= (1ULL << channel);
    }
    else
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes &= ~(1ULL << channel);
    }

    /* Check self/mutual setting feasibility */
    if (channel > (TF_TSI_SELF_CAP_CHANNEL_COUNT - 1U))
    {
        NT_ASSERT(!(bool)(tsiState->opModesData[tsiState->opMode].enabledElectrodes &
                          (1ULL << ((channel - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT))));
        NT_ASSERT(!(bool)(tsiState->opModesData[tsiState->opMode].enabledElectrodes &
                          (1ULL << (((channel - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT) +
                                    TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT))));
#if (defined(FSL_FEATURE_TSI_HAS_M_TX_USED) && FSL_FEATURE_TSI_HAS_M_TX_USED)
        /* Set or clear M_TX_USED bitfield to make normal GPIO function for pins not used as TSI mutual TX pins */
        TSI_Type *base = g_tsiBase[instance];
        if (enable)
            TSI_SetUsedTxChannel(base, (tsi_mutual_tx_channel_t)((channel - TF_TSI_SELF_CAP_CHANNEL_COUNT) /
                                                                 TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
        else
            TSI_ClearUsedTxChannel(base, (tsi_mutual_tx_channel_t)((channel - TF_TSI_SELF_CAP_CHANNEL_COUNT) /
                                                                   TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
#endif
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_Measure
 * Description   : This function gets (measure) capacitance of enabled electrodes
 *               from the TSI module using a non-blocking method.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_Measure(uint32_t instance)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    struct nt_module_data *module =
        _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    const struct nt_electrode *const *electrodes = module->rom->electrodes;

    uint32_t mutual_rx, mutual_tx;
    uint8_t electrode_last = module->electrodes_cnt;

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return tsiState->status;
    }

    /* Check if at least one electrode is enabled. */
    while ((bool)(electrode_last--))
    {
        if ((bool)_nt_electrode_get_flag(module->electrodes[electrode_last], (uint32_t)NT_ELECTRODE_ENABLED))
        {
            break;
        }
        if (electrode_last == 0U)
        {
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            return kStatus_TSI_InvalidChannel;
        }
    }
    tsiState->status = kStatus_TSI_Busy;

    electrode_last = module->electrode_last;
    NT_ASSERT(electrode_last <= module->electrodes_cnt);

    /* re-trigger new measure set in case the measurement was not start from handler */
    if (tsiState->status != kStatus_TSI_Overflow)
    {
        if (tsiState->opSatus != tsi_OpStatusSuspend)
        {
            if (electrode_last == 0U)
            {
                module->electrode_last = module->electrodes_cnt; /* reset electrode number */
                electrode_last         = module->electrode_last;
            }
        }
    }

    while ((bool)electrode_last--)
    {
        /* Find the last enabled electrode */
        if ((bool)_nt_electrode_get_flag(module->electrodes[electrode_last], (uint32_t)NT_ELECTRODE_ENABLED))
        {
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init self-cap sensing */
            TSI_EnableModule(base, false);

            /* If this electrode is a self-cap mode electrode initialize self-cap sensing*/
            if (electrodes[electrode_last]->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT)
            {
                TSI_InitSelfCapMode(base, &module->electrodes[electrode_last]->tsi_hw_config->configSelfCap);
                TSI_SetSelfCapMeasuredChannel(base, (uint8_t)electrodes[electrode_last]->pin_input);
            }
            /* If this electrode is a mutual-cap mdde electrode, initialize mutual-cap sensing*/
            else
            {
                /*Parse electrode number into rx, tx components*/
                mutual_tx = (electrodes[electrode_last]->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) /
                            TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
                mutual_rx = (electrodes[electrode_last]->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) %
                            TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;

                TSI_InitMutualCapMode(base, &module->electrodes[electrode_last]->tsi_hw_config->configMutual);
                TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
                TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
            }
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            if (tsiState->opSatus != tsi_OpStatusSuspend)
            {
                TSI_StartSoftwareTrigger(base);
                module->electrode_last =
                    electrode_last; /* store TSI channel triggered for sensing to be read in handler*/
            }
            return kStatus_TSI_Success;
        }
    }
    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_EnableLowPower
 * Description   : Enables/Disables the low power module.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_EnableLowPower(uint32_t instance)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    tsi_status_t status;

    if ((status = NT_TSI_DRV_ChangeMode(instance, tsi_OpModeLowPower)) != kStatus_TSI_Success)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return status;
    }

    if (tsiState->opModesData[tsiState->opMode].enabledElectrodes == 0U)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }

    tsiState->status = kStatus_TSI_LowPower;

    /* Enable TSI to run in STOP and VLPS mode */
    TSI_EnableModule(base, false);
    TSI_EnableLowPower(base, true);
    TSI_EnableModule(base, true);

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_ChangeMode
 * Description   : The function change the current mode.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_ChangeMode(uint32_t instance, const nt_tsi_modes_t mode)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];

    if ((mode == tsiState->opMode) || (mode == tsi_OpModeNoChange))
    {
        return kStatus_TSI_Success;
    }

    if (mode >= tsi_OpModeCnt)
    {
        return kStatus_TSI_InvalidMode;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lockChangeMode);

        return tsiState->status;
    }

    tsiState->opMode = mode;

    NT_TSI_DRV_InitSpecific(base, &tsiState->opModesData[mode].config);

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lockChangeMode);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_LoadConfiguration
 * Description   : The function load the configuration for one mode of operation.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_LoadConfiguration(uint32_t instance,
                                          const nt_tsi_modes_t mode,
                                          const nt_tsi_operation_mode_t *operationMode)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(operationMode != NULL);
    TSI_Type *base;
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];

    if (mode >= tsi_OpModeCnt)
    {
        return kStatus_TSI_InvalidMode;
    }

    tsiState->opModesData[mode] = *operationMode;

    /* In case that the loaded configuration is active one, update the HW also. */
    if (mode == tsiState->opMode)
    {
        base = g_tsiBase[instance];

        NT_TSI_DRV_InitSpecific(base, &tsiState->opModesData[mode].config);
        TSI_EnableInterrupts(base, (uint32_t)kTSI_GlobalInterruptEnable);
        TSI_EnableInterrupts(base, (uint32_t)kTSI_EndOfScanInterruptEnable);
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*!
 * @brief Get Sinc status flags
 * This function get tsi Sinc status flags.
 *
 * @param    base  TSI peripheral base address.
 * @return         The mask of these status flags combination.
 */
static inline uint32_t TSI_GetSincFlags(TSI_Type *base)
{
    return (base->SINC & ((uint32_t)kTSI_EndOfScanFlag | (uint32_t)kTSI_OutOfRangeFlag));
}

/*!
 * @brief Interrupt handler for TSI.
 * This handler uses the tsi State structure to handle the instance depend data.
 * This is not a public API as it is called whenever an interrupt occurs.
 */
void TSI_DRV_IRQHandler(uint32_t instance)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    uint32_t mutual_rx;
    uint32_t mutual_tx;
    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    struct nt_module_data *module =
        _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    const struct nt_electrode *const *electrodes = module->rom->electrodes;
    uint8_t electrode_last                       = module->electrode_last;
    NT_ASSERT(electrode_last <= module->electrodes_cnt);

    if ((bool)(TSI_GetSincFlags(base) & (uint32_t)kTSI_SincOverflowFlag))
    {
        tsiState->status = kStatus_TSI_Overflow;
        /* If overflow execute following commands */
        if ((bool)(tsiState->pCallBackFunc))
        {
            tsiState->pCallBackFunc(instance, tsiState->usrData);
        }
        tsiState->status = kStatus_TSI_Success;
    }

    if ((bool)(tsi_lpwr_status.TSILowPower))
    {
        tsi_lpwr_status.SelfLowPowerCountBuff   = TSI_GetCounter(base);
        tsi_lpwr_status.SelfLowPowerChannelBuff = TSI_GetSelfCapMeasuredChannel(base);
        tsi_lpwr_status.TSILowPower             = 0;
    }

    /* Clear interrupt flags. */
    TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
    TSI_ClearStatusFlags(base, (uint32_t)kTSI_EndOfScanFlag);

    /* reading the measured value */
    if ((bool)(tsiState->status != kStatus_TSI_Overflow))
    {
        if ((bool)(tsiState->opSatus != tsi_OpStatusSuspend))
        {                                          /* Save TSICNT value */
            if (!(bool)(TSI_GetSensingMode(base))) /* self-cap electrode was measured */
            {
                _nt_electrode_set_raw_signal(module->electrodes[electrode_last],
                                             (uint32_t)0xFFFF - (uint32_t)TSI_GetCounter(base));
            }
            else /* mutual-cap electrode was measured */
            {
                _nt_electrode_set_raw_signal(module->electrodes[electrode_last], TSI_GetCounter(base));
            }
            if ((bool)(electrode_last == 0U)) /* The last channel was read */
            {
                module->electrode_last              = module->electrodes_cnt; /* reset electrode number */
                tsi_lpwr_status.TSIScanCompleteFlag = 1;                      /* set the complete flag */
            }
            else
            {
                tsi_lpwr_status.TSIScanCompleteFlag = 0;
            }
        }
    }
    /* Find the next enabled electrode and assigned it for next channel measurement */
    if ((bool)(tsiState->opSatus != tsi_OpStatusSuspend))
    {
        while ((bool)electrode_last--)
        { /* Check if the electrode is enabled */
            if ((bool)(_nt_electrode_get_flag(module->electrodes[electrode_last], (uint32_t)NT_ELECTRODE_ENABLED)))
            { /* OS: End of critical section. */
                NT_OSA_MutexUnlock(&tsiState->lock);

                /* Disable TSI module, the same for self or mutual sensing */
                TSI_EnableModule(base, false);
                /* If this electrode is a self-cap mode electrode initialize self-cap sensing */
                if ((bool)(electrodes[electrode_last]->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT))
                { /* Init self-cap sensing */
                    TSI_InitSelfCapMode(base, &module->electrodes[electrode_last]->tsi_hw_config->configSelfCap);
                    TSI_SetSelfCapMeasuredChannel(base, (uint8_t)electrodes[electrode_last]->pin_input);
                }
                else /* If this electrode is a mutual-cap mdde electrode, initialize mutual-cap sensing */
                {
                    /*Parse electrode number into rx, tx components*/
                    mutual_tx = (electrodes[electrode_last]->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) /
                                TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
                    mutual_rx = (electrodes[electrode_last]->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) %
                                TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
                    /* Init mutual-cap sensing */
                    TSI_InitMutualCapMode(base, &module->electrodes[electrode_last]->tsi_hw_config->configMutual);
                    TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
                    TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
                }
                TSI_EnableHardwareTriggerScan(base, false); /* Set HW trigger, the same for self or mutual sensing */
                TSI_EnableModule(base, true);               /* Enable TSI module, the same for self or mutual sensing */

                if ((bool)(tsiState->opSatus != tsi_OpStatusSuspend)) /* If TSI not Suspended */
                {
                    TSI_StartSoftwareTrigger(base); /* Set SW trigger, the same for self or mutual sensing */
                }
                module->electrode_last =
                    electrode_last; /* Store TSI channel triggered for sensing to be read in handler */
                return;             /* After new measurement assigned go out from handler to measure new sample */
            }
        }
    }
    /* If all samples have been measured execute following commands */
    if ((bool)(tsiState->pCallBackFunc))
    {
        tsiState->pCallBackFunc(instance, tsiState->usrData);
    }

    if ((bool)(tsiState->status != kStatus_TSI_LowPower))
    { /* Return status of the driver to initialized state */
        tsiState->status = kStatus_TSI_Initialized;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_MeasureAllElect
 * Description   : Automatically measures all used TSI electrodes.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_MeasureAllElect(uint32_t instance, uint16_t (*counters)[])
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(counters != NULL);

    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    TSI_Type *base           = g_tsiBase[instance];
    struct nt_module_data *module =
        _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    struct nt_electrode_data **electrodes = module->electrodes;
    uint32_t mutual_rx;
    uint32_t mutual_tx;
    uint32_t gencs         = 0;
    uint32_t timeout       = 1000000U;
    uint8_t electrode_last = module->electrodes_cnt;
    uint8_t n;
    bool is_enabled     = (bool)0;
    bool is_int_enabled = (bool)0;

    /* Check wheather the array for all electrode exists */
    n = module->electrodes_cnt;
    while ((bool)(n--))
    {
        (*counters)[n] = 0U;
        NT_ASSERT((*counters)[n] == 0U);
    }

    if ((bool)(tsiState->status != kStatus_TSI_Initialized))
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }

    /* Check if at least one electrode is enabled. */
    while ((bool)(electrode_last--))
    {
        if ((bool)(_nt_electrode_get_flag(module->electrodes[electrode_last], (int32_t)NT_ELECTRODE_ENABLED)))
        {
            break;
        }
        if ((bool)(electrode_last == 0U))
        {
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            return kStatus_TSI_InvalidChannel;
        }
    }
    tsiState->status = kStatus_TSI_Busy;

    electrode_last = module->electrode_last;
    NT_ASSERT(electrode_last <= module->electrodes_cnt);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable tsi clock */
    (void)CLOCK_EnableClock(s_tsiClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    if ((bool)(base->GENCS & TSI_GENCS_TSIEN_MASK))
    {
        is_enabled = true;
        TSI_EnableModule(base, false);
    }
    if ((bool)(base->GENCS & TSI_GENCS_TSIIEN_MASK))
    {
        is_int_enabled = true;
        TSI_DisableInterrupts(base, (int32_t)kTSI_GlobalInterruptEnable);
    }

    while ((bool)(electrode_last--))
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        /* Init self-cap sensing */
        TSI_EnableModule(base, false);

        /* If this electrode is a self-cap mode electrode initialize self-cap sensing*/
        if (electrodes[electrode_last]->rom->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT)
        {
            TSI_InitSelfCapMode(base, &electrodes[electrode_last]->tsi_hw_config->configSelfCap);
            TSI_SetSelfCapMeasuredChannel(base, (uint8_t)electrodes[electrode_last]->rom->pin_input);
        }
        /* If this electrode is a mutual-cap mdde electrode, initialize mutual-cap sensing*/
        else
        {
            /*Parse electrode number into rx, tx components*/
            mutual_tx = (electrodes[electrode_last]->rom->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) /
                        TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            mutual_rx = (electrodes[electrode_last]->rom->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) %
                        TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;

            TSI_InitMutualCapMode(base, &electrodes[electrode_last]->tsi_hw_config->configMutual);
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
        }
        TSI_EnableHardwareTriggerScan(base, false);
        TSI_EnableModule(base, true);
        TSI_StartSoftwareTrigger(base);

        while ((bool)1) /* not an infinite loop, there is break on the end of loop,it can start again a few time. */
        {               /* reused from TSI_DRV_IRQHandler */
            gencs = base->GENCS;
            while (((gencs & TSI_GENCS_EOSF_MASK) == 0U) && (bool)(timeout != 0U))
            {
                gencs = base->GENCS;
                timeout--;
            } /* Do nothing, just to meet MISRA C 2004 rule 14.3 . */

            /* Clear interrupt flags. */
            TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
            TSI_ClearStatusFlags(base, (int32_t)kTSI_EndOfScanFlag);

            /* reading the measured value,  save TSICNT value */
            if (!(bool)(TSI_GetSensingMode(base))) /* self-cap electrode was measured */
            {
                (*counters)[electrode_last] = ((uint16_t)0xFFFF - TSI_GetCounter(base));
            }
            else /* mutual-cap electrode was measured */
            {
                (*counters)[electrode_last] = (TSI_GetCounter(base));
            }
            if (electrode_last == 0U) /* The last channel was read */
            {
                module->electrode_last = module->electrodes_cnt; /* reset electrode number */
                break;
            }

            electrode_last--; /*  decrease electrode counter to assign new measurement */
            /* Disable TSI module, the same for self or mutual sensing */
            TSI_EnableModule(base, false);
            /* If this electrode is a self-cap mode electrode initialize self-cap sensing*/
            if (electrodes[electrode_last]->rom->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT)
            { /* Init self-cap sensing */
                TSI_InitSelfCapMode(base, &electrodes[electrode_last]->tsi_hw_config->configSelfCap);
                TSI_SetSelfCapMeasuredChannel(base, (uint8_t)electrodes[electrode_last]->rom->pin_input);
            }
            else /* If this electrode is a mutual-cap mdde electrode, initialize mutual-cap sensing*/
            {
                /*Parse electrode number into rx, tx components*/
                mutual_tx = (electrodes[electrode_last]->rom->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) /
                            TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
                mutual_rx = (electrodes[electrode_last]->rom->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) %
                            TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
                /* Init mutual-cap sensing */
                TSI_InitMutualCapMode(base, &electrodes[electrode_last]->tsi_hw_config->configMutual);
                TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
                TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
            }
            TSI_EnableHardwareTriggerScan(base, false); /* Set HW trigger, the same for self or mutual sensing */
            TSI_EnableModule(base, true);               /* Enable TSI module, the same for self or mutual sensing */
            TSI_StartSoftwareTrigger(base);
        }
    }

    if (is_enabled)
    {
        TSI_EnableModule(base, true);
    }
    if (is_int_enabled)
    {
        TSI_EnableInterrupts(base, (int32_t)kTSI_GlobalInterruptEnable);
    }

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_MeasureMoreElect
 * Description   : Automatically measures selected TSI electrodes.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_MeasureMoreElect(uint32_t instance,
                                         uint16_t (*counters)[],
                                         struct nt_electrode **electrodes,
                                         uint8_t elNumber)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(counters != NULL);
    NT_ASSERT(electrodes != NULL);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    uint32_t mutual_rx, mutual_tx;
    uint32_t gencs   = 0;
    uint32_t timeout = 1000000U;
    uint8_t n;
    bool is_enabled     = (bool)0;
    bool is_int_enabled = (bool)0;

    /* Check weather the array for all electrode exists */
    n = elNumber;
    while ((bool)(n--))
    {
        (*counters)[n] = 0;
        NT_ASSERT((*counters)[n] == 0U);
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }

    tsiState->status = kStatus_TSI_Busy;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable tsi clock */
    (void)CLOCK_EnableClock(s_tsiClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    if ((bool)(base->GENCS & TSI_GENCS_TSIEN_MASK))
    {
        is_enabled = true;
        TSI_EnableModule(base, false);
    }
    if ((bool)(base->GENCS & TSI_GENCS_TSIIEN_MASK))
    {
        is_int_enabled = true;
        TSI_DisableInterrupts(base, (uint32_t)kTSI_GlobalInterruptEnable);
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    while ((bool)(elNumber--))
    {
        NT_ASSERT(electrodes[elNumber] != NULL);
        struct nt_electrode_data *electrode_data = _nt_electrode_get_data(electrodes[elNumber]);
        /* Init self-cap sensing */
        TSI_EnableModule(base, false);

        /* If this electrode is a self-cap mode electrode initialize self-cap sensing*/
        if (electrodes[elNumber]->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT)
        {
            TSI_InitSelfCapMode(base, &electrode_data->tsi_hw_config->configSelfCap);
            TSI_SetSelfCapMeasuredChannel(base, (uint8_t)electrodes[elNumber]->pin_input);
        }
        /* If this electrode is a mutual-cap mdde electrode, initialize mutual-cap sensing*/
        else
        {
            /*Parse electrode number into rx, tx components*/
            mutual_tx =
                (electrodes[elNumber]->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            mutual_rx =
                (electrodes[elNumber]->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;

            TSI_InitMutualCapMode(base, &electrode_data->tsi_hw_config->configMutual);
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
        }
        TSI_EnableHardwareTriggerScan(base, false);
        TSI_EnableModule(base, true);
        TSI_StartSoftwareTrigger(base);

        gencs = base->GENCS;
        while (((gencs & TSI_GENCS_EOSF_MASK) == 0U) && (bool)(timeout != 0U))
        {
            timeout--;
            gencs = base->GENCS;
        } /* Do nothing, just to meet MISRA C 2004 rule 14.3 . */

        /* Clear interrupt flags. */
        TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
        TSI_ClearStatusFlags(base, (uint32_t)kTSI_EndOfScanFlag);

        /* reading the measured value,  save TSICNT value */
        if (!(bool)(TSI_GetSensingMode(base))) /* self-cap electrode was measured */
        {
            (*counters)[elNumber] = ((uint16_t)0xFFFF - TSI_GetCounter(base));
        }
        else /* mutual-cap electrode was measured */
        {
            (*counters)[elNumber] = (TSI_GetCounter(base));
        }
    }

    if (is_enabled)
    {
        TSI_EnableModule(base, true);
    }
    if (is_int_enabled)
    {
        TSI_EnableInterrupts(base, (uint32_t)kTSI_GlobalInterruptEnable);
    }

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_MeasureOneElect
 * Description   : Automatically measures one electrode.
 *
 *END**************************************************************************/
uint16_t NT_TSI_DRV_MeasureOneElect(uint32_t instance, const struct nt_electrode *electrode)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(electrode != NULL);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    uint32_t mutual_rx, mutual_tx;

    uint32_t gencs   = 0;
    uint32_t timeout = 1000000U;
    uint16_t counter;

    bool is_enabled     = (bool)0;
    bool is_int_enabled = (bool)0;

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return ((uint16_t)tsiState->status);
    }

    tsiState->status = kStatus_TSI_Busy;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable tsi clock */
    (void)CLOCK_EnableClock(s_tsiClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    if ((bool)(base->GENCS & TSI_GENCS_TSIEN_MASK))
    {
        is_enabled = true;
        TSI_EnableModule(base, false);
    }
    if ((bool)(base->GENCS & TSI_GENCS_TSIIEN_MASK))
    {
        is_int_enabled = true;
        TSI_DisableInterrupts(base, (uint32_t)kTSI_GlobalInterruptEnable);
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    struct nt_electrode_data *electrode_data = _nt_electrode_get_data(electrode);
    /* Init self-cap sensing */
    TSI_EnableModule(base, false);

    /* If this electrode is a self-cap mode electrode initialize self-cap sensing*/
    if (electrode->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT)
    {
        TSI_InitSelfCapMode(base, &electrode_data->tsi_hw_config->configSelfCap);
        TSI_SetSelfCapMeasuredChannel(base, (uint8_t)electrode->pin_input);
    }
    /* If this electrode is a mutual-cap mode electrode, initialize mutual-cap sensing*/
    else
    {
        /*Parse electrode number into rx, tx components*/
        mutual_tx = (electrode->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
        mutual_rx = (electrode->pin_input - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;

        TSI_InitMutualCapMode(base, &electrode_data->tsi_hw_config->configMutual);
        TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
        TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
    }
    TSI_EnableHardwareTriggerScan(base, false);
    TSI_EnableModule(base, true);
    TSI_StartSoftwareTrigger(base);

    gencs = base->GENCS;
    while (((gencs & TSI_GENCS_EOSF_MASK) == 0U) && (bool)(timeout != 0U))
    {
        gencs = base->GENCS;
        timeout--;
    } /* Do nothing, just to meet MISRA C 2004 rule 14.3 . */

    /* Clear interrupt flags. */
    TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
    TSI_ClearStatusFlags(base, (uint32_t)kTSI_EndOfScanFlag);

    /* reading the measured value,  save TSICNT value */
    if (!(bool)(TSI_GetSensingMode(base))) /* self-cap electrode was measured */
    {
        counter = ((uint16_t)0xFFFF - TSI_GetCounter(base));
    }
    else /* mutual-cap electrode was measured */
    {
        counter = (TSI_GetCounter(base));
    }

    if (is_enabled)
    {
        TSI_EnableModule(base, true);
    }
    if (is_int_enabled)
    {
        TSI_EnableInterrupts(base, (uint32_t)kTSI_GlobalInterruptEnable);
    }

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return counter;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_Recalibrate
 * Description   : Automatically recalibrates all important TSI settings.
 *
 *END**************************************************************************/
#if (NT_SAFETY_SUPPORT == 1)
tsi_status_t NT_TSI_DRV_Recalibrate(uint32_t instance, void *configuration)
{
    NT_ASSERT(instance < (uint32_t)FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(configuration != NULL);

    int32_t delta;
    uint32_t cycleCounter, elNumber, selfCount, mutualCount;
    uint16_t counter, counter_previous;
    nt_tsi_modes_t curr_mode;
    struct nt_electrode_data *electrode_data;
    tsi_selfCap_config_t *configSelfCap;
    tsi_mutualCap_config_t *configMutual;
    nt_tsi_state_t *tsiState                     = g_tsiStatePtr[instance];
    struct nt_kernel *system                     = _nt_system_get();
    nt_tsi_recalib_config_t *recalib_config      = system->rom->modules[instance]->recalib_config;
    const struct nt_electrode *const *electrodes = system->rom->modules[instance]->electrodes;
    NT_ASSERT(electrodes != NULL);
    const struct nt_electrode *electrodesEnabledSelf[TF_TSI_SELF_CAP_CHANNEL_COUNT];
    const struct nt_electrode *electrodesEnabledMutual[TF_TSI_MUTUAL_CHANNEL_COUNT];
    curr_mode = NT_TSI_DRV_GetMode(system->rom->modules[instance]->instance);
    if (curr_mode != tsi_OpModeNormal)
    {
        return kStatus_TSI_Success;
    }
    /* finding all enabled electrodes */
    selfCount   = 0;
    mutualCount = 0;
    while ((bool)(*electrodes != NULL))
    { /* Check weather configuration can be modify (is RAM located) for recalibration reasons */
        electrode_data   = _nt_electrode_get_data(*electrodes);
        configSelfCap    = (tsi_selfCap_config_t *)&electrode_data->tsi_hw_config->configSelfCap;
        uint32_t address = (uint32_t) & *configSelfCap;

/* variable setting for obtaining FLASH start and end address to check if configuration is in FLASH or RAM */
#ifndef NT_FLASH_START
#ifndef NT_FLASH_END
#if defined(__IAR_SYSTEMS_ICC__) /* For IAR compiler   */
        flash_start = (uint32_t)__section_begin(".rodata");
        flash_end   = (uint32_t)__section_end(".rodata");
#elif defined(__CC_ARM)                                 /* For ARM(KEIL) version < 6 compiler */
        volatile uint32_t flash_start = (uint32_t)&Load$$ER_m_text$$RO$$Base;
        volatile uint32_t flash_end   = (uint32_t)&Load$$ER_m_text$$RO$$Limit;
#elif defined(__MCUXPRESSO)                             /* For GCC compiler  MCUX IDE */
        flash_start = (uint32_t)&__base_PROGRAM_FLASH;
        flash_end   = (uint32_t)&__top_PROGRAM_FLASH;
#elif defined(__GNUC__) && (__ARMCC_VERSION == 0)       /* For ARMGCC compiler */
        flash_start = (uint32_t)&__data_start__;
        flash_end   = (uint32_t)&__data_end__;
#elif defined(__GNUC__) && (__ARMCC_VERSION >= 6010050) /* For ARM(KEIL) version >= 60 compiler */
        volatile uint32_t flash_start = (uint32_t)&Load$$ER_m_text$$RO$$Base;
        volatile uint32_t flash_end   = (uint32_t)&Load$$ER_m_text$$RO$$Limit;
#else                                                   /* Other compiler used */
#warning "Unsupported compiler/IDE used !"
#endif /* NT_FLASH_END */
#endif /* NT_FLASH_START */
        if (flash_start <= address && address <= flash_end)
        {
            return kStatus_TSI_CofigurationIsReadOnly;
        }
#else  /*(!(NT_FLASH_START) && !(NT_FLASH_END))*/
        if (NT_FLASH_START <= address && address <= NT_FLASH_END)
        {
            return kStatus_TSI_CofigurationIsReadOnly;
        }
#endif /*(!(NT_FLASH_START) && !(NT_FLASH_END))*/

        if ((bool)_nt_electrode_get_flag(electrode_data, (uint32_t)NT_ELECTRODE_ENABLED))
        {
            if (electrode_data->rom->pin_input < TF_TSI_SELF_CAP_CHANNEL_COUNT)
            {
                electrodesEnabledSelf[selfCount] = *electrodes;
                selfCount++;
            }
            else
            {
                electrodesEnabledMutual[mutualCount] = *electrodes;
                mutualCount++;
            }
        }
        electrodes++;
    }

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }

    /* untouched counter calibration for self-cap mode */
    cycleCounter = 0;
    elNumber     = selfCount;
    while ((bool)(elNumber--))
    {
        do
        {
            electrode_data = _nt_electrode_get_data(electrodesEnabledSelf[elNumber]);
            configSelfCap  = (tsi_selfCap_config_t *)&electrode_data->tsi_hw_config->configSelfCap;
            counter        = NT_TSI_DRV_MeasureOneElect(instance, electrodesEnabledSelf[elNumber]);
            if (counter == 0U)
            {
                return kStatus_TSI_MutUntouchRecalibError;
            }
            cycleCounter++;
            if (counter < recalib_config->SelfRangeMin) /* untouched counter value is too low */
            {
                if (configSelfCap->commonConfig.decimation > kTSI_SincDecimationValue_1 &&
                    cycleCounter <= 32U) /* SINC decimation calibration, 32 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.decimation;
                    temp -= 1U;
                    configSelfCap->commonConfig.decimation = (tsi_sinc_decimation_value_t)temp; //-= 1;
                    continue;
                }
                else if (configSelfCap->chargeCurrent < kTSI_CurrentMultipleChargeValue_7 && cycleCounter > 32U &&
                         cycleCounter <= 32U + 8U) /* Charge/Discharge current multiple calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->chargeCurrent;
                    temp += 1U;
                    configSelfCap->chargeCurrent = (tsi_current_multiple_charge_t)temp;
                    continue;
                }
                else if (configSelfCap->inputCurrent < kTSI_CurrentMultipleInputValue_1 && cycleCounter > 32U + 8U &&
                         cycleCounter <= 32U + 8U + 2U) /* Input current multiple calibration, 2 values */
                {
                    configSelfCap->inputCurrent = kTSI_CurrentMultipleInputValue_1;
                    continue;
                }
                else if (configSelfCap->commonConfig.dvolt > kTSI_DvoltOption_0 && cycleCounter > 32U + 8U + 2U &&
                         cycleCounter <= 32U + 8U + 2U + 4U) /* DVOLT option value calibration, 4 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.dvolt;
                    temp -= 1U;
                    configSelfCap->commonConfig.dvolt = (tsi_dvolt_option_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.cutoff < kTSI_SincCutoffDiv_128 &&
                         cycleCounter > 32U + 8U + 2U + 4U &&
                         cycleCounter <= 32U + 8U + 2U + 4U + 8U) /* Cut-off divider calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.cutoff;
                    temp += 1U;
                    configSelfCap->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.order > kTSI_SincFilterOrder_1 &&
                         cycleCounter > 32U + 8U + 2U + 4U + 8U &&
                         cycleCounter <= 32U + 8U + 2U + 4U + 8U + 2U) /* SINC filter order, 2 values */
                {
                    configSelfCap->commonConfig.order = kTSI_SincFilterOrder_1;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 32U + 8U + 2U + 4U + 8U + 2U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_SelfUntouchRecalibError; /* no configuration setting available - go out from
                                                                   calibration */
                }
            }
            else if (counter > recalib_config->SelfRangeMax) /* untouched counter value is too high */
            {
                if (configSelfCap->commonConfig.decimation < kTSI_SincDecimationValue_32 &&
                    cycleCounter <= 32U) /* SINC decimation calibration, 32 values  */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.decimation;
                    temp += 1U;
                    configSelfCap->commonConfig.decimation = (tsi_sinc_decimation_value_t)temp; //+= 1;
                    continue;
                }
                else if (configSelfCap->chargeCurrent > kTSI_CurrentMultipleChargeValue_0 && cycleCounter > 32U &&
                         cycleCounter <= 32U + 8U) /* Charge/Discharge current multiple calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->chargeCurrent;
                    temp -= 1U;
                    configSelfCap->chargeCurrent = (tsi_current_multiple_charge_t)temp;
                    continue;
                }
                else if (configSelfCap->inputCurrent > kTSI_CurrentMultipleInputValue_0 && cycleCounter > 32U + 8U &&
                         cycleCounter <= 32U + 8U + 2U) /* Input current multiple calibration, 2 values */
                {
                    configSelfCap->inputCurrent = kTSI_CurrentMultipleInputValue_0;
                    continue;
                }
                else if (configSelfCap->commonConfig.dvolt < kTSI_DvoltOption_3 && cycleCounter > 32U + 8U + 2U &&
                         cycleCounter <= 32U + 8U + 2U + 4U) /* DVOLT option value calibration, 4 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.dvolt;
                    temp += 1U;
                    configSelfCap->commonConfig.dvolt = (tsi_dvolt_option_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.cutoff > kTSI_SincCutoffDiv_0 &&
                         cycleCounter > 32U + 8U + 2U + 4U &&
                         cycleCounter <= 32U + 8U + 2U + 4U + 8U) /* Cut-off divider calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.cutoff;
                    temp -= 1U;
                    configSelfCap->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.order < kTSI_SincFilterOrder_2 &&
                         cycleCounter > 32U + 8U + 2U + 4U + 8U &&
                         cycleCounter <= 32U + 8U + 2U + 4U + 8U + 2U) /* SINC filter order, 2 values */
                {
                    configSelfCap->commonConfig.order = kTSI_SincFilterOrder_2;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 32U + 8U + 2U + 4U + 8U + 2U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_SelfUntouchRecalibError; /* no configuration setting available - go out from
                                                                   calibration */
                }
            }
            else
            { /* no command to avoid Misra issue */
            }
        } while (
            (bool)(counter < recalib_config->SelfRangeMin) ||
            (bool)(counter > recalib_config->SelfRangeMax)); /* next calibration cycle required, TSI setting changed */
        cycleCounter = 0;
    }
    /* sensitivity calibration for self-cap mode, it makes one or more touches using gpio method on self-cap electrode
     */
    elNumber = selfCount;
    while ((bool)(elNumber--))
    {
        do
        {
            electrode_data   = _nt_electrode_get_data(electrodesEnabledSelf[elNumber]);
            configSelfCap    = &electrode_data->tsi_hw_config->configSelfCap;
            counter_previous = NT_TSI_DRV_MeasureOneElect(instance, electrodesEnabledSelf[elNumber]);
            (void)(int32_t) nt_module_TSI_TouchOneElect(electrodesEnabledSelf[elNumber]);
            uint8_t n;
            for (n = 0U; n < 9U; n++)
            {
                CUSTOM_DELAY()
            };
            counter = NT_TSI_DRV_MeasureOneElect(instance, electrodesEnabledSelf[elNumber]);
            (void)(int32_t) nt_module_TSI_ReleaseOneElect(electrodesEnabledSelf[elNumber]);
            delta = (int32_t)counter - (int32_t)counter_previous;
            for (n = 0U; n < 9U; n++)
            {
                CUSTOM_DELAY()
            };
            if (delta < 0)
            {
                delta = -delta;
            }
            cycleCounter++;
            if (delta < (int32_t)recalib_config->SelfRangeDeltaMin)
            {
                configSelfCap->enableSensitivity = true;
                if (configSelfCap->xdn < kTSI_SensitivityXdnOption_7 &&
                    cycleCounter <= 8U) /* Sensitivity XDN option calibration, 8 values   */
                {
                    uint8_t temp = (uint8_t)configSelfCap->xdn;
                    temp += 1U;
                    configSelfCap->xdn = (tsi_sensitivity_xdn_option_t)temp;
                    continue;
                }
                else if (configSelfCap->ctrim < kTSI_SensitivityCtrimOption_7 && cycleCounter > 8U &&
                         cycleCounter <= 8U + 8U) /* Sensitivity CTRIM option calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->ctrim;
                    temp += 1U;
                    configSelfCap->ctrim = (tsi_sensitivity_ctrim_option_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.dvolt > kTSI_DvoltOption_0 && cycleCounter > 8U + 8U &&
                         cycleCounter <= 8U + 8U + 4U) /* DVOLT option value calibration, 4 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.dvolt;
                    temp -= 1U;
                    configSelfCap->commonConfig.dvolt = (tsi_dvolt_option_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.cutoff > kTSI_SincCutoffDiv_0 && cycleCounter > 8U + 8U + 4U &&
                         cycleCounter <= 8U + 8U + 4U + 8U) /* Cut-off divider calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.cutoff;
                    temp -= 1U;
                    configSelfCap->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 8U + 8U + 4U + 8U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_SelfSensitivRecalibError; /* no configuration setting available - go out from
                                                                    calibration */
                }
            }
            else if (delta > (int32_t)recalib_config->SelfRangeDeltaMax)

            {
                configSelfCap->enableSensitivity = true;
                if (configSelfCap->xdn > kTSI_SensitivityXdnOption_0 &&
                    cycleCounter <= 8U) /* Sensitivity XDN option calibration, 8 values   */
                {
                    uint8_t temp = (uint8_t)configSelfCap->xdn;
                    temp -= 1U;
                    configSelfCap->xdn = (tsi_sensitivity_xdn_option_t)temp;
                    continue;
                }
                else if (configSelfCap->ctrim > kTSI_SensitivityCtrimOption_0 && cycleCounter > 8U &&
                         cycleCounter <= 8U + 8U) /* Sensitivity CTRIM option calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->ctrim;
                    temp -= 1U;
                    configSelfCap->ctrim = (tsi_sensitivity_ctrim_option_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.dvolt < kTSI_DvoltOption_3 && cycleCounter > 8U + 8U &&
                         cycleCounter <= 8U + 8U + 4U) /* DVOLT option value calibration, 4 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.dvolt;
                    temp += 1U;
                    configSelfCap->commonConfig.dvolt = (tsi_dvolt_option_t)temp;
                    continue;
                }
                else if (configSelfCap->commonConfig.cutoff < kTSI_SincCutoffDiv_128 && cycleCounter > 8U + 8U + 4U &&
                         cycleCounter <= 8U + 8U + 4U + 8U) /* Cut-off divider calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configSelfCap->commonConfig.cutoff;
                    temp += 1U;
                    configSelfCap->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 8U + 8U + 4U + 8U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_SelfSensitivRecalibError; /* no configuration setting available - go out from
                                                                    calibration */
                }
            }
            else
            { /* no command to avoid Misra issue */
            }
        } while ((bool)(delta < (int32_t)recalib_config->SelfRangeDeltaMin) ||
                 (bool)(delta > (int32_t)recalib_config->SelfRangeDeltaMax));
        cycleCounter = 0;
    }

    /* untouched counter calibration for mutual-cap mode */
    elNumber = mutualCount;
    while ((bool)(elNumber--))
    {
        do
        {
            electrode_data = _nt_electrode_get_data(electrodesEnabledMutual[elNumber]);
            configMutual   = (tsi_mutualCap_config_t *)&electrode_data->tsi_hw_config->configMutual;
            counter        = NT_TSI_DRV_MeasureOneElect(instance, electrodesEnabledMutual[elNumber]);
            (void)(int32_t) nt_module_TSI_TouchOneElect(electrodesEnabledMutual[elNumber]);
            if (counter == 0U)
            {
                return kStatus_TSI_MutUntouchRecalibError;
            }
            cycleCounter++;
            if (counter < recalib_config->MutRangeMin) /* untouched counter value is too low */
            {
                if (configMutual->commonConfig.decimation < kTSI_SincDecimationValue_32 &&
                    cycleCounter <= 32U) /* SINC decimation calibration, 32 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.decimation;
                    temp += 1U;
                    configMutual->commonConfig.decimation = (tsi_sinc_decimation_value_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.cutoff > kTSI_SincCutoffDiv_0 && cycleCounter > 32U &&
                         cycleCounter <= 32U + 8U) /* Cut-off divider calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.cutoff;
                    temp -= 1U;
                    configMutual->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.chargeNum > kTSI_SscChargeNumValue_1 && cycleCounter > 32U + 8U &&
                         cycleCounter <= 32U + 8U + 16U) /* SSC output bit0's period setting calibration, 16 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.chargeNum;
                    temp -= 1U;
                    configMutual->commonConfig.chargeNum = (tsi_ssc_charge_num_t)temp;
                    continue;
                }
                else if (configMutual->preResistor < kTSI_MutualPreResistor_8k && cycleCounter > 32U + 8U + 16U &&
                         cycleCounter <= 32U + 8U + 16U + 8U) /* Vref generator resistor  calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configMutual->preResistor;
                    temp += 1U;
                    configMutual->preResistor = (tsi_mutual_pre_resistor_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.dvolt < kTSI_DvoltOption_3 && cycleCounter > 32U + 8U + 16U + 8U &&
                         cycleCounter <= 32U + 8U + 16U + 8U + 4U) /* DVOLT option value calibration, 4 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.dvolt;
                    temp += 1U;
                    configMutual->commonConfig.dvolt = (tsi_dvolt_option_t)temp;
                    continue;
                }
                else if (configMutual->boostCurrent > kTSI_MutualSenseBoostCurrent_0uA &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U &&
                         cycleCounter <=
                             32U + 8U + 16U + 8U + 4U + 32U) /* Sensitivity boost current setting, 32 values */
                {
                    uint8_t temp = (uint8_t)configMutual->boostCurrent;
                    temp += 1U;
                    configMutual->boostCurrent = (tsi_mutual_sense_boost_current_t)temp;
                    continue;
                }
                else if (configMutual->pmosLeftCurrent > kTSI_MutualPmosCurrentMirrorLeft_4 &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U + 32U &&
                         cycleCounter <=
                             32U + 8U + 16U + 8U + 4U + 32U + 8U) /* Pmos current mirror on the left side, 8 values */
                {
                    uint8_t temp = (uint8_t)configMutual->pmosLeftCurrent;
                    temp += 1U;
                    configMutual->pmosLeftCurrent = (tsi_mutual_pmos_current_left_t)temp;
                    continue;
                }
                else if (configMutual->pmosRightCurrent < kTSI_MutualPmosCurrentMirrorRight_4 &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U + 32U + 8U &&
                         cycleCounter <= 32U + 8U + 16U + 8U + 4U + 32U + 8U +
                                             4U) /* Pmos current mirror on the right side, 4 values */
                {
                    uint8_t temp = (uint8_t)configMutual->pmosRightCurrent;
                    temp -= 1U;
                    configMutual->pmosRightCurrent = (tsi_mutual_pmos_current_right_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.order > kTSI_SincFilterOrder_1 &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U + 32U + 8U + 4U &&
                         cycleCounter <=
                             32U + 8U + 16U + 8U + 4U + 32U + 8U + 4U + 2U) /* SINC filter order, 2 values */
                {
                    configMutual->commonConfig.order = kTSI_SincFilterOrder_1;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 32U + 8U + 16U + 8U + 4U + 32U + 8U + 4U + 2U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_MutUntouchRecalibError; /* no configuration setting available - go out from
                                                                  calibration */
                }
            }
            else if (counter > recalib_config->MutRangeMax) /* untouched counter value is too high */
            {
                if (configMutual->commonConfig.decimation > kTSI_SincDecimationValue_1 &&
                    cycleCounter <= 32U) /* SINC decimation calibration, 32 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.decimation;
                    temp -= 1U;
                    configMutual->commonConfig.decimation = (tsi_sinc_decimation_value_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.cutoff < kTSI_SincCutoffDiv_128 && cycleCounter > 32U &&
                         cycleCounter <= 32U + 8U) /* Cut-off divider calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.cutoff;
                    temp += 1U;
                    configMutual->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.chargeNum < kTSI_SscChargeNumValue_16 && cycleCounter > 32U + 8U &&
                         cycleCounter <= 32U + 8U + 16U) /* SSC output bit0's period setting calibration, 16 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.chargeNum;
                    temp += 1U;
                    configMutual->commonConfig.chargeNum = (tsi_ssc_charge_num_t)temp;
                    continue;
                }
                else if (configMutual->preResistor > kTSI_MutualPreResistor_1k && cycleCounter > 32U + 8U + 16U &&
                         cycleCounter <= 32U + 8U + 16U + 8U) /* Vref generator resistor  calibration, 8 values */
                {
                    uint8_t temp = (uint8_t)configMutual->preResistor;
                    temp -= 1U;
                    configMutual->preResistor = (tsi_mutual_pre_resistor_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.dvolt > kTSI_DvoltOption_0 && cycleCounter > 32U + 8U + 16U + 8U &&
                         cycleCounter <= 32U + 8U + 16U + 8U + 4U) /* DVOLT option value calibration, 4 values */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.dvolt;
                    temp -= 1U;
                    configMutual->commonConfig.dvolt = (tsi_dvolt_option_t)temp;
                    continue;
                }
                else if (configMutual->boostCurrent < kTSI_MutualSenseBoostCurrent_62uA &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U &&
                         cycleCounter <=
                             32U + 8U + 16U + 8U + 4U + 32U) /* Sensitivity boost current setting, 32 values */
                {
                    uint8_t temp = (uint8_t)configMutual->boostCurrent;
                    temp -= 1U;
                    configMutual->boostCurrent = (tsi_mutual_sense_boost_current_t)temp;
                    continue;
                }
                else if (configMutual->pmosLeftCurrent < kTSI_MutualPmosCurrentMirrorLeft_32 &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U + 32U &&
                         cycleCounter <=
                             32U + 8U + 16U + 8U + 4U + 32U + 8U) /* Pmos current mirror on the left side, 8 values */
                {
                    uint8_t temp = (uint8_t)configMutual->pmosLeftCurrent;
                    temp -= 1U;
                    configMutual->pmosLeftCurrent = (tsi_mutual_pmos_current_left_t)temp;
                    continue;
                }
                else if (configMutual->pmosRightCurrent > kTSI_MutualPmosCurrentMirrorRight_1 &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U + 32U + 8U &&
                         cycleCounter <= 32U + 8U + 16U + 8U + 4U + 32U + 8U +
                                             4U) /* Pmos current mirror on the right side, 4 values */
                {
                    uint8_t temp = (uint8_t)configMutual->pmosRightCurrent;
                    temp += 1U;
                    configMutual->pmosRightCurrent = (tsi_mutual_pmos_current_right_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.order < kTSI_SincFilterOrder_2 &&
                         cycleCounter > 32U + 8U + 16U + 8U + 4U + 32U + 8U + 4U &&
                         cycleCounter <=
                             32U + 8U + 16U + 8U + 4U + 32U + 8U + 4U + 2U) /* SINC filter order, 2 values */
                {
                    configMutual->commonConfig.order = kTSI_SincFilterOrder_2;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 32U + 8U + 16U + 8U + 4U + 32U + 8U + 4U + 2U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_MutUntouchRecalibError; /* no configuration setting available - go out from
                                                                  calibration */
                }
            }
            else
            { /* no command to avoid Misra issue */
            }
        } while (
            (bool)(counter < recalib_config->MutRangeMin) ||
            (bool)(counter > recalib_config->MutRangeMax)); /* next calibration cycle required, TSI setting changed */
        cycleCounter = 0;
    }
    /* sensitivity calibration for mutual-cap mode, it makes one or more touches using gpio method on mutual-cap
     * electrode */
    elNumber = mutualCount;
    while ((bool)(elNumber--))
    {
        do
        {
            electrode_data   = _nt_electrode_get_data(electrodesEnabledMutual[elNumber]);
            configMutual     = (tsi_mutualCap_config_t *)&electrode_data->tsi_hw_config->configMutual;
            counter_previous = NT_TSI_DRV_MeasureOneElect(instance, electrodesEnabledMutual[elNumber]);
            (void)(int32_t) nt_module_TSI_TouchOneElect(electrodesEnabledMutual[elNumber]);
            uint8_t n;
            for (n = 0U; n < 9U; n++)
            {
                CUSTOM_DELAY()
            };
            counter = NT_TSI_DRV_MeasureOneElect(instance, electrodesEnabledMutual[elNumber]);
            (void)(int32_t) nt_module_TSI_ReleaseOneElect(electrodesEnabledMutual[elNumber]);
            delta = (int32_t)counter_previous - (int32_t)counter;
            for (n = 0U; n < 9U; n++)
            {
                CUSTOM_DELAY()
            };
            if (delta < 0)
            {
                delta = -delta;
            }
            cycleCounter++;
            if (delta < (int32_t)recalib_config->MutRangeDeltaMin)
            {
                configMutual->enableNmosMirror = true;
                if (configMutual->boostCurrent < kTSI_MutualSenseBoostCurrent_62uA &&
                    cycleCounter <= 32U) /* Sensitivity boost current calibration, 32 values  */
                {
                    uint8_t temp = (uint8_t)configMutual->boostCurrent;
                    temp += 1U;
                    configMutual->boostCurrent = (tsi_mutual_sense_boost_current_t)temp;
                    continue;
                }
                if (configMutual->commonConfig.decimation > kTSI_SincDecimationValue_1 && cycleCounter > 32U &&
                    cycleCounter <= 32U + 32U) /* SINC decimation calibration, 32 values  */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.decimation;
                    temp += 1U;
                    configMutual->commonConfig.decimation = (tsi_sinc_decimation_value_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.cutoff > kTSI_SincCutoffDiv_0 && cycleCounter > 32U + 32U &&
                         cycleCounter <= 32U + 32U + 8U) /* Cut-off divider, 8 calibration values  */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.cutoff;
                    temp += 1U;
                    configMutual->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 32U + 32U + 8U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_MutSensitivRecalibError; /* no configuration setting available - go out from
                                                                   calibration */
                }
            }
            else if (delta > (int32_t)recalib_config->MutRangeDeltaMax)
            {
                configMutual->enableNmosMirror = true;
                if (configMutual->boostCurrent > kTSI_MutualSenseBoostCurrent_0uA &&
                    cycleCounter <= 32U) /* Sensitivity boost current calibration, 32 values  */
                {
                    uint8_t temp = (uint8_t)configMutual->boostCurrent;
                    temp -= 1U;
                    configMutual->boostCurrent = (tsi_mutual_sense_boost_current_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.decimation < kTSI_SincDecimationValue_32 && cycleCounter > 32U &&
                         cycleCounter <= 32U + 32U) /* SINC decimation calibration, 32 values  */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.decimation;
                    temp -= 1U;
                    configMutual->commonConfig.decimation = (tsi_sinc_decimation_value_t)temp;
                    continue;
                }
                else if (configMutual->commonConfig.cutoff < kTSI_SincCutoffDiv_128 && cycleCounter > 32U + 32U &&
                         cycleCounter <= 32U + 32U + 8U) /* Cut-off divider, 8 calibration values  */
                {
                    uint8_t temp = (uint8_t)configMutual->commonConfig.cutoff;
                    temp -= 1U;
                    configMutual->commonConfig.cutoff = (tsi_sinc_cutoff_div_t)temp;
                    continue;
                }
                else if (cycleCounter > 0U && cycleCounter <= 32U + 32U + 8U)
                {
                    continue;
                }
                else
                {
                    return kStatus_TSI_MutSensitivRecalibError; /* no configuration setting available - go out from
                                                                   calibration */
                }
            }
            else
            { /* no command to avoid Misra issue */
            }
        } while ((bool)(delta < (int32_t)recalib_config->MutRangeDeltaMin) ||
                 (bool)(delta > (int32_t)recalib_config->MutRangeDeltaMax));
    }
    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}
#endif /* NT_SAFETY_SUPPORT */
#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
