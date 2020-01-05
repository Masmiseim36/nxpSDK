/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>
#include "nt_drv_tsi_driver.h"
#include "nt_setup.h"
#if CPU_K32L2A41VLL1A || CPU_K32L2A31VLL1A
#include "fsl_intmux.h"
#endif
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
 * Local functions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_Init
* Description   : Initialize whole the TSI peripheral to be ready to read capacitance changes
* To initialize the TSI driver, the configuration structure should be handled.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_Init(uint32_t instance, nt_tsi_state_t * tsiState, const nt_tsi_user_config_t * tsiUserConfig)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiSt = g_tsiStatePtr[instance];

    /* OS: start of critical section, critical code will not be pre-empted */
    NT_OSA_EnterCritical();

    /* Exit if current instance is already initialized. */
    if(tsiSt)
    {
      /* OS: End of critical section. */
        NT_OSA_ExitCritical();

        return kStatus_TSI_Initialized;
    }
    /* Save runtime structure pointer.*/
    tsiSt = g_tsiStatePtr[instance] = tsiState;

    /* Clear the state structure for this instance. */
    memset(tsiSt, 0, sizeof(nt_tsi_state_t));

    /* OS: create the mutex used by whole driver. */
    NT_OSA_MutexCreate(&tsiSt->lock);
    /* OS: create the mutex used by change mode function. */
    NT_OSA_MutexCreate(&tsiSt->lockChangeMode);
    
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiSt->lock, OSA_WAIT_FOREVER))
    {
        /* TODO: OS End of critical section. */
        NT_OSA_ExitCritical();
        return kStatus_TSI_Error;
    }

    /* OS: End of critical section. */
    NT_OSA_ExitCritical();

    tsiSt->opMode = tsi_OpModeNormal;

    tsiSt->opModesData[tsiSt->opMode].config = *tsiUserConfig->config; /* Store the hardware configuration. */

    tsiSt->pCallBackFunc = tsiUserConfig->pCallBackFunc;
    tsiSt->usrData = tsiUserConfig->usrData;

    NT_TSI_DRV_InitSpecific(base, &tsiSt->opModesData[tsiSt->opMode].config);
    
    /* Clear possible pending flags */
    TSI_ClearStatusFlags(base,(kTSI_EndOfScanFlag | (uint32_t)kTSI_OutOfRangeFlag));
    
    TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(base, kTSI_EndOfScanInterruptEnable);

#if (FSL_FEATURE_TSI_VERSION == 2)
    TSI_EnablePeriodicalScan(base, false);
#elif (FSL_FEATURE_TSI_VERSION == 4)
    TSI_EnableHardwareTriggerScan(base, false);
#endif

    /* Disable all electrodes */
    tsiState->opModesData[tsiState->opMode].enabledElectrodes = 0;

    /* Enable TSI interrupt on NVIC level. */
    #if CPU_K32L2A41VLL1A || CPU_K32L2A31VLL1A
    INTMUX_Init(INTMUX0);
    INTMUX_EnableInterrupt(INTMUX0, 0, TSI0_IRQn);  
    #else    
    EnableIRQ(g_tsiIrqId[instance]);
    #endif
    tsiSt->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiSt->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_DeInit
* Description   : De initialize whole the TSI peripheral and driver to be ready
* for any future use and don't load the system.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_DeInit(uint32_t instance)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    
    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];

    if (tsiState == NULL)
    {
        return kStatus_TSI_Error;
    }

    TSI_DisableInterrupts(base, kTSI_GlobalInterruptEnable);
    tsiState->opModesData[tsiState->opMode].enabledElectrodes = 0;
    TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
    TSI_ClearStatusFlags(base, kTSI_EndOfScanFlag);
    TSI_EnableModule(base, false);

    /* Disable the interrupt */
    DisableIRQ(g_tsiIrqId[instance]);

    /* Clear runtime structure pointer.*/
    tsiState = NULL;

    /* Gate TSI module clock */
    CLOCK_DisableClock(kCLOCK_Tsi0);
    
    /*Deinit TSI module*/
    TSI_Deinit(base);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_SetCallBackFunc
* Description   : Set the TSI call back function pointer for non blocking measurement
*
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_SetCallBackFunc(uint32_t instance, const tsi_callback_t pFuncCallBack, void * usrData)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
#ifdef NT_OSA
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
#endif
    
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (g_tsiStatePtr[instance]->status != kStatus_TSI_Initialized)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return g_tsiStatePtr[instance]->status;
    }

    g_tsiStatePtr[instance]->pCallBackFunc = pFuncCallBack;
    g_tsiStatePtr[instance]->usrData = usrData;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_GetEnabledElectrodes
* Description   : Get Enables electrodes for measuring.
*
*END**************************************************************************/
uint64_t NT_TSI_DRV_GetEnabledElectrodes(uint32_t instance)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];

    return tsiState->opModesData[tsiState->opMode].enabledElectrodes;
}

/*FUNCTION**********************************************************************
*
* Function Name : TSI_DRV_IsBusy
* Description   : Function returns the busy state of the driver
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_GetStatus(uint32_t instance)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    return g_tsiStatePtr[instance]->status;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_DisableLowPower
* Description   : Enables/Disables the low power module.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_DisableLowPower(uint32_t instance, const nt_tsi_modes_t mode)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    tsi_status_t status;

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_LowPower)
    {
        /* TODO: OS End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return tsiState->status;
    }
    TSI_EnableLowPower(base, false);
    TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(base, kTSI_EndOfScanInterruptEnable);
    
#if (FSL_FEATURE_TSI_VERSION == 2)
    TSI_EnablePeriodicalScan(base, false);
#elif (FSL_FEATURE_TSI_VERSION == 4)
    TSI_EnableHardwareTriggerScan(base, false);
#endif

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    status = NT_TSI_DRV_ChangeMode(instance, mode);

    return status;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_GetMode
* Description   : Function returns the current mode of the driver.
*
*END**************************************************************************/
nt_tsi_modes_t NT_TSI_DRV_GetMode(uint32_t instance)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    return g_tsiStatePtr[instance]->opMode;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_SaveConfiguration
* Description   : The function save the configuration for one mode of operation.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_SaveConfiguration(uint32_t instance, const nt_tsi_modes_t mode, nt_tsi_operation_mode_t * operationMode)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(operationMode);
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    
    if(mode >= tsi_OpModeCnt)
    {
        return kStatus_TSI_InvalidMode;
    }

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    *operationMode =  tsiState->opModesData[mode];

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return  kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_Suspend
* Description   : Temporary block of TSI measurement
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_Suspend(uint32_t instance)
{
  NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
  
  nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
  tsiState->opSatus = tsi_OpStatusSuspend;
   
  return  kStatus_TSI_Success;  
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_Resume
* Description   : Resumes measurement after Suspend
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_Resume(uint32_t instance)
{
  NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
  
  TSI_Type * base = g_tsiBase[instance];
  
  nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
  
  if(tsiState->opSatus == tsi_OpStatusSuspend)
  {
    TSI_StartSoftwareTrigger(base);  // Fire next conversion to resume measurement
    tsiState->opSatus = tsi_OpStatusNormal;  
  }
  
  return  kStatus_TSI_Success;
}


#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/

