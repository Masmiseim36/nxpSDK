/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Software LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 20. Feb 2015
*
*/

/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include "ph_NxpBuild.h"
#include <ph_Status.h>
#include <phlnLlcp.h>

#ifdef NXPBUILD__PHLN_LLCP
#include "phlnLlcp_Timers.h"
#include "phlnLlcp_Sw_Mac.h"

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
static uint8_t gkphlnLlcp_baSymPdu[2] = {0x0, 0x0};
extern phlnLlcp_Transport_Socket_t gsphlnLlcp_Socket;
static const uint8_t bLlcpSymTimerName[] = "LLCPSymTimer";
static const uint8_t bLlcpLtoTimerName[] = "LLCPLtoTimer";
/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */

phStatus_t phlnLlcp_Timers_InitSym(phlnLlcp_Sw_DataParams_t * pDataParams,
    phlnLlcpTimer_notify_function_t pCB)
{
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;

    pDataParams->LlcpSymTimerObj.pTimerName = (uint8_t *)bLlcpSymTimerName;
    pDataParams->LlcpSymTimerObj.opt = OS_TIMER_OPT_SINGLE_SHOT;
    pDataParams->LlcpSymTimerObj.timePeriod.unitPeriod = OS_TIMER_UNIT_MSEC;
    pDataParams->LlcpSymTimerObj.timePeriod.period = 100;
    pDataParams->LlcpSymTimerObj.timerCb = pCB;
    pDataParams->LlcpSymTimerObj.arg = NULL;

    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_TimerCreate(&pDataParams->LlcpSymTimerObj.TimerHandle,
        &pDataParams->LlcpSymTimerObj));

    if(pDataParams->LlcpSymTimerObj.TimerHandle == NULL){
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Timers_InitLto(phlnLlcp_Sw_DataParams_t * pDataParams,
    phlnLlcpTimer_notify_function_t pCB)
{
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;

    pDataParams->LlcpLtoTimerObj.pTimerName = (uint8_t *)bLlcpLtoTimerName;
    pDataParams->LlcpLtoTimerObj.opt = OS_TIMER_OPT_SINGLE_SHOT;
    pDataParams->LlcpLtoTimerObj.timePeriod.unitPeriod = OS_TIMER_UNIT_MSEC;
    pDataParams->LlcpLtoTimerObj.timePeriod.period = 100;
    pDataParams->LlcpLtoTimerObj.timerCb = pCB;
    pDataParams->LlcpLtoTimerObj.arg = NULL;

    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_TimerCreate(&pDataParams->LlcpLtoTimerObj.TimerHandle,
        &pDataParams->LlcpLtoTimerObj));

    if(pDataParams->LlcpLtoTimerObj.TimerHandle == NULL){
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_LN_LLCP);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phlnLlcp_Timers_DeInitSym(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
    if(pDataParams->LlcpSymTimerObj.TimerHandle != NULL)
    {
        wStatus = phOsal_TimerDelete(&pDataParams->LlcpSymTimerObj.TimerHandle);
    }
    pDataParams->LlcpSymTimerObj.TimerHandle = NULL;
    return (phStatus_t)wStatus;
}

phStatus_t phlnLlcp_Timers_DeInitLto(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
    if(pDataParams->LlcpLtoTimerObj.TimerHandle != NULL)
    {
        wStatus = phOsal_TimerDelete(&pDataParams->LlcpLtoTimerObj.TimerHandle);
    }
    pDataParams->LlcpLtoTimerObj.TimerHandle = NULL;
    return (phStatus_t)wStatus;
}

phStatus_t phlnLlcp_Timers_StopLto(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    if(pDataParams->LlcpLtoTimerObj.TimerHandle != NULL)
    {
        wStatus = phOsal_TimerStop(&pDataParams->LlcpLtoTimerObj.TimerHandle);
    }
    return (phStatus_t)wStatus;
}

phStatus_t phlnLlcp_Timers_SymStart(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t wStatus;

    wStatus = phOsal_TimerModify(&pDataParams->LlcpSymTimerObj.TimerHandle, &pDataParams->LlcpSymTimerObj);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return wStatus;
    }

    return phOsal_TimerStart(&pDataParams->LlcpSymTimerObj.TimerHandle);
}


phStatus_t phlnLlcp_Timers_LtoStart(phlnLlcp_Sw_DataParams_t * pDataParams)
{
    phStatus_t wStatus;

    wStatus = phOsal_TimerModify(&pDataParams->LlcpLtoTimerObj.TimerHandle, &pDataParams->LlcpLtoTimerObj);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return wStatus;
    }

    return phOsal_TimerStart(&pDataParams->LlcpLtoTimerObj.TimerHandle);
}


void phlnLlcp_SymTimerCallback(void * arg)
{
    /* Check whether the DTS message q mutex is available */
    /* If available post a SymmPdu to the DataToBeSent */
    /* else do nothing since already some other service is queuing the DTS msg */

    phTools_Q_t * PH_MEMLOC_REM psMsgQueue = phTools_Q_Get(1, PH_ON);
    /* If Lack of  Free Message Q pool, Symm will fail and LTO expires on the Peer Side.
    * Increase the pool size to avoid this issue. */
    if (psMsgQueue != NULL)
    {
        psMsgQueue->pbData = (uint8_t *)gkphlnLlcp_baSymPdu;
        psMsgQueue->dwLength = 2;
        psMsgQueue->bType = PH_TOOLS_Q_DATA_TO_BE_SENT;
        psMsgQueue->wFrameOpt = PH_TRANSMIT_DEFAULT;
        psMsgQueue->pSender = &gsphlnLlcp_Socket;

        (void)phTools_Q_SendFront(psMsgQueue, 0, psMsgQueue->wFrameOpt);
    }
    (void)arg;
}

void phlnLlcp_LtoTimerCallback(void * arg)
{
    (void)arg;

    /* Abort 18092 operation by calling HAL Shutdown. */
    phlnLlcp_Sw_MacHAL_ShutDown();
}

#endif /* NXPBUILD__PHLN_LLCP */
