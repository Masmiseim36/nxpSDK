/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       timer_server_shim.c
\brief      provides the wrapper functions for timer server manager
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#ifndef USE_FWK_TIMER
    #define USE_FWK_TIMER 1
#endif

#ifndef TSV_PARAM_VALIDATION
#define TSV_PARAM_VALIDATION 0
#endif

#include "fsl_common.h"
#include "mac_vs_sap.h"
#include "fsl_os_abstraction.h"

#if USE_FWK_TIMER
  #include "TimersManager.h"
  #include "Panic.h"
#else
  #include "fsl_clock.h"
  #include "fsl_tpm.h"
#endif

/*****************************************************************************
******************************************************************************

******************************************************************************
*****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#if !USE_FWK_TIMER
/* Base address & Other components for ZPSTSV */
#define TPM_BASEADDR TPM0
#define TPM_OUT_CHANNEL kTPM_Chnl_0
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
//volatile static uint32 count = 0;
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
TSV_Timer_s *psTimerServerList = NULL;
#if !USE_FWK_TIMER
static const tpm_config_t mTpmConfig = {.prescale = kTPM_Prescale_Divide_32,
                                        .useGlobalTimeBase = 0,
                                        .enableDoze = 0,
                                        .enableDebugMode = 0,
                                        .enableReloadOnTrigger = 0,
                                        .enableStopOnOverflow = 0,
                                        .enableStartOnTrigger = 0,
                                        .triggerSelect = kTPM_Trigger_Select_0};
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
void vTimerServerInit(void)
{
#if USE_FWK_TIMER
    TMR_Init();
#else
    /* Select the clock source for the TPM counter as MCGPLLCLK */
    TPM_Init(TPM_BASEADDR, &mTpmConfig);
    /* Setup the output compare mode to toggle output on a match */
    TPM_SetupOutputCompare(TPM_BASEADDR, TPM_OUT_CHANNEL, kTPM_NoOutputSignal,1000);
    /* Set the timer to be in free-running mode */
    TPM_BASEADDR->MOD = 0xFFFF;
    TPM_EnableInterrupts(TPM_BASEADDR, kTPM_Chnl0InterruptEnable );
    NVIC_ClearPendingIRQ(TPM0_IRQn);
    NVIC_EnableIRQ(TPM0_IRQn);
    TPM_StartTimer(TPM_BASEADDR, kTPM_SystemClock);
#endif
}

void vTimerServerDeinit(void)
{
#if !USE_FWK_TIMER
    TPM_ClearStatusFlags(TPM_BASEADDR, TPM_GetStatusFlags(TPM_BASEADDR));
    NVIC_ClearPendingIRQ(TPM0_IRQn);
    TPM_Deinit(TPM_BASEADDR);
#endif
}

#if USE_FWK_TIMER
void TSV_callback(void * param)
{
    TSV_Timer_s * psTimer = (TSV_Timer_s *)param;

    psTimer->u32Value = 0;

    if (psTimer->prExpireCb)
    {
        psTimer->prExpireCb(psTimer->pvParam);
    }
}

#else
void vTimerFired(void)
{
    TSV_Timer_s     *psTimer;
    /* Go through timer server timers. We don't support TSV_eClose so once
       a timer is on the list it stays on the list: hence no need to store
       the next entry ahead of expiring the current one */

    psTimer = psTimerServerList;
    while (psTimer)
    {
        if (psTimer->u32Value)
        {
            psTimer->u32Value--;
            if (psTimer->u32Value == 0)
            {
                if (psTimer->prExpireCb)
                {
                    psTimer->prExpireCb(psTimer->pvParam);
                }
            }
        }
        /* Move to next entry */
        psTimer = psTimer->psTimerNext;
    }
}
#endif

/* Timer server replacements. Note that MiniMac itself does not use the timer
   server: it uses a more compact set of timers. The timer server replacements
   are for other stack layers to use */
 TSV_ResultCode_e TSV_eOpen(
    TSV_Timer_s* psNewTimer,
    TSV_ExpireCb_pr prExpireCb,
    void *pvParam)
{
    TSV_ResultCode_e status = TSV_I_OK;
#if TSV_PARAM_VALIDATION
    if (psNewTimer == NULL)
    {
        status = TSV_E_INVALID_PARAM;
    }
    else
#endif
    {
        /* Store the callback */
        psNewTimer->prExpireCb = prExpireCb;
        psNewTimer->pvParam = pvParam;
    }
    
    return status;
}

TSV_ResultCode_e TSV_eStart(
    TSV_Timer_s* psNewTimer,
    uint32_t u32Value)
{
    TSV_ResultCode_e eResult;
    TSV_Timer_s *psTimer;
#if USE_FWK_TIMER
    tmrErrCode_t status;
    tmrTimerID_t tmr;
#endif
#if TSV_PARAM_VALIDATION
    if (psNewTimer == NULL)
    {
        eResult = TSV_E_INVALID_PARAM;
    }
    else
#endif
    {
        OSA_InterruptDisable();
        
        if (psNewTimer->u32Value)
        {
            eResult = TSV_E_ALREADY_RUNNING;
        }
        else
        {
            psNewTimer->u32Value = u32SymbolsToTicks(u32Value);
            eResult = TSV_I_OK;
        }
        
        /* Add the timer to the list, if it isn't there already */
        psTimer = psTimerServerList;
        while (psTimer)
        {
            if (psTimer == psNewTimer)
            {
                /* Already on list */
                break;
            }
            /* Move to next entry */
            psTimer = psTimer->psTimerNext;
        }
        
        if (psTimer != psNewTimer)
        {
            /* Timer not on list: add it (to front: simpler code) */
            psNewTimer->psTimerNext = psTimerServerList;
            psTimerServerList = psNewTimer;
        }
        
        OSA_InterruptEnable();
        
#if USE_FWK_TIMER
        tmr = TMR_GetFirstWithParam(psNewTimer);
        
        if (tmr == gTmrInvalidTimerID_c)
        {
            tmr = TMR_AllocateTimer();
        }
        
        status = TMR_StartSingleShotTimer(tmr,
                                          psNewTimer->u32Value,
                                          TSV_callback,
                                          psNewTimer);
        if (status != gTmrSuccess_c)
        {
            eResult = TSV_E_ERROR;
        }
#endif
    }
    return eResult;
}

TSV_ResultCode_e TSV_eStop(
    TSV_Timer_s* psTimer)
{
    TSV_ResultCode_e eResult;
#if USE_FWK_TIMER
    tmrTimerID_t tmrId;
#endif

#if TSV_PARAM_VALIDATION
    if (psTimer == NULL)
    {
        eResult = TSV_E_INVALID_PARAM;
    }
    else
#endif
    if (0 == psTimer->u32Value)
    {
        eResult = TSV_E_NOT_RUNNING;
    }
    else
    {
        psTimer->u32Value = 0;
        eResult = TSV_I_OK;
#if USE_FWK_TIMER
        tmrId = TMR_GetFirstWithParam(psTimer);
        TMR_StopTimer(tmrId);
#endif
    }

    return eResult;
}

PUBLIC TSV_ResultCode_e
TSV_eClose(TSV_Timer_s* psTimer,
           bool_t bInvokeCBIfRunning)
{
#if USE_FWK_TIMER
    tmrTimerID_t tmrId = TMR_GetFirstWithParam(psTimer);
    
    TMR_StopTimer(tmrId);
    TMR_FreeTimer(tmrId);
#endif
    if (bInvokeCBIfRunning && psTimer && psTimer->prExpireCb)
    {
        psTimer->prExpireCb(psTimer->pvParam);
    }
    return TSV_I_OK;
}


#if !USE_FWK_TIMER
void TPM0_IRQHandler(void)
{
    uint32_t status = TPM_GetStatusFlags(TPM_BASEADDR);
    TPM_ClearStatusFlags(TPM_BASEADDR, status);
    TPM_SetupOutputCompare(TPM_BASEADDR, TPM_OUT_CHANNEL, kTPM_NoOutputSignal,(1000+TPM_BASEADDR->CNT));

    vTimerFired();
}
#endif
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
