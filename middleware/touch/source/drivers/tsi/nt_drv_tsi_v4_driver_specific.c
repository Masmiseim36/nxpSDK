/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>
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
extern IRQn_Type tsi_irq_ids[FSL_FEATURE_SOC_TSI_COUNT];
extern void TSI_DRV_IRQHandler0(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_Init
 * Description   : Encapsulates TSI v4 init.
 *
 *END**************************************************************************/
void NT_TSI_DRV_InitSpecific(TSI_Type *base, const tsi_config_t *config)
{
    TSI_Init(base, config);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_EnableElectrode
 * Description   : Enables/Disables the electrode for measuring.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_EnableElectrode(uint32_t instance, const uint32_t channel, const bool enable)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(channel < FSL_FEATURE_TSI_CHANNEL_COUNT);

    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];

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

    /* Check the condition for low power mode. */
    if ((tsiState->opMode == tsi_OpModeLowPower) || (tsiState->opMode == tsi_OpModeProximity))
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes = 0;
    }

    if (enable)
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes |= (1U << channel);
    }
    else
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes &= ~(1U << channel);
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
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    struct nt_module_data *module =
        _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    const struct nt_electrode *const *electrodes = module->rom->electrodes;
    uint8_t electrode_last                       = module->electrodes_cnt;

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

    /* Check if at least one electrode is enabled. */
    while (electrode_last--)
    {
        if (_nt_electrode_get_flag(module->electrodes[electrode_last], NT_ELECTRODE_ENABLED))
            break;
        if (electrode_last == 0)
        {
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            return kStatus_TSI_InvalidChannel;
        }
    }

    tsiState->status = kStatus_TSI_Busy;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    /* Init self-cap sensing */
    TSI_EnableModule(base, false);
    TSI_SetMeasuredChannelNumber(base, electrodes[electrode_last]->pin_input);
    TSI_EnableHardwareTriggerScan(base, false);
    TSI_EnableModule(base, true);
    TSI_StartSoftwareTrigger(base);
    module->electrode_last = electrode_last; /* store TSI channel triggered for sensing to be read in handler*/

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
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    tsi_status_t status;
    uint32_t i;
    int32_t channel = -1;

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if ((tsiState->opModesData[tsiState->opMode].config.thresl == 0) ||
        (tsiState->opModesData[tsiState->opMode].config.thresh == 0))
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_Error;
    }

    if ((status = NT_TSI_DRV_ChangeMode(instance, tsi_OpModeLowPower)) != kStatus_TSI_Success)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return status;
    }

    if (tsiState->opModesData[tsiState->opMode].enabledElectrodes == 0)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }

    /* Configurate the peripheral for next use */
    TSI_EnableInterrupts(base, kTSI_OutOfRangeInterruptEnable);
    TSI_EnableHardwareTriggerScan(base, true);

    for (i = 0; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        if ((uint32_t)(1 << i) & tsiState->opModesData[tsiState->opMode].enabledElectrodes)
        {
            channel = i;
            break;
        }
    }

    if (channel == -1)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }

    tsiState->status = kStatus_TSI_LowPower;

    TSI_EnableLowPower(base, true);
    TSI_SetMeasuredChannelNumber(base, channel);
    TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
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
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

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

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lockChangeMode, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lockChangeMode);

        return tsiState->status;
    }

    if (mode == tsi_OpModeNoise)
    {
        if (!tsiState->opModesData[mode].config.mode)
        {
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lockChangeMode);

            return kStatus_TSI_InvalidMode;
        }
    }
    else
    {
        if (tsiState->opModesData[mode].config.mode)
        {
            /* OS End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lockChangeMode);

            return kStatus_TSI_InvalidMode;
        }
    }

    tsiState->opMode = mode;

    TSI_Init(base, &tsiState->opModesData[mode].config);

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
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(operationMode);
    TSI_Type *base;
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];

    if (mode >= tsi_OpModeCnt)
    {
        return kStatus_TSI_InvalidMode;
    }

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    tsiState->opModesData[mode] = *operationMode;

    /* In case that the loaded configuration is active one, update the HW also. */
    if (mode == tsiState->opMode)
    {
        base = g_tsiBase[instance];

        TSI_Init(base, &tsiState->opModesData[mode].config);
        TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
        TSI_EnableInterrupts(base, kTSI_EndOfScanInterruptEnable);
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*!
 * @brief Interrupt handler for TSI.
 * This handler uses the tsi State structure to handle the instance depend data.
 * This is not a public API as it is called whenever an interrupt occurs.
 */
void TSI_DRV_IRQHandler(uint32_t instance)
{
    TSI_Type *base           = g_tsiBase[instance];
    nt_tsi_state_t *tsiState = g_tsiStatePtr[instance];
    struct nt_module_data *module =
        _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    const struct nt_electrode *const *electrodes = module->rom->electrodes;
    uint8_t electrode_last                       = module->electrode_last;
    /* Check if a measure is running and wanted. */

    TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
    TSI_ClearStatusFlags(base, kTSI_EndOfScanFlag);

    if (_nt_electrode_get_flag(module->electrodes[electrode_last], NT_ELECTRODE_ENABLED))
    {
        /* Am I in noise mode? */
        if (tsiState->opMode == tsi_OpModeNoise)
        {
            _nt_electrode_set_raw_signal(module->electrodes[electrode_last], TSI_GetNoiseModeResult(base));
        }
        else
        {
            _nt_electrode_set_raw_signal(module->electrodes[electrode_last], TSI_GetCounter(base));
        }
    }

    /* Find the next enabled electrode and assigned it for next channel measurement */
    while (electrode_last--)
    { /* Check if the electrode is enabled */
        if (_nt_electrode_get_flag(module->electrodes[electrode_last], NT_ELECTRODE_ENABLED))
        {
            TSI_SetMeasuredChannelNumber(base, electrodes[electrode_last]->pin_input);
            TSI_StartSoftwareTrigger(base); /* Set SW trigger */

            module->electrode_last = electrode_last; /* Store TSI channel triggered for sensing to be read in handler */
            return; /* After new measurement assigned go out from handler to measure new sample */
        }
    }

    if (tsiState->pCallBackFunc)
    {
        tsiState->pCallBackFunc(instance, tsiState->usrData);
    }

    if (tsiState->status != kStatus_TSI_LowPower)
    {
        /* Return status of the driver to initialized state */
        tsiState->status = kStatus_TSI_Initialized;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : NT_TSI_DRV_Recalibrate
 * Description   : Automatically recalibrates all important TSI settings.
 *
 *END**************************************************************************/
tsi_status_t NT_TSI_DRV_Recalibrate(uint32_t instance, void *configuration)
{
    return kStatus_TSI_Error;
}

#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
