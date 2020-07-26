/*
*                    Copyright (c), NXP Semiconductors
*
*                       (C) NXP Semiconductors 2014
*
*         All rights are reserved. Reproduction in whole or in part is
*        prohibited without the written consent of the copyright owner.
*    NXP reserves the right to make changes without notice at any time.
*   NXP makes no warranty, expressed, implied or statutory, including but
*   not limited to any implied warranty of merchantability or fitness for any
*  particular purpose, or that the use will not infringe any third party patent,
*   copyright or trademark. NXP must not be liable for any loss or damage
*                            arising from its use.
*/

/** @file
*
* phTools_Q.c:  <The purpose and scope of this file>
*
* Project:  NxpNfcRdLib
*
* $Date$
* $Author$
* $Revision$ (v06.01.00)
*/


/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */

#include "ph_NxpBuild.h"
#include <ph_Status.h>
#include <phOsal.h>
#include <phTools.h>

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */
static phOsal_MutexObj_t gphTools_Q_Mutex;
static phOsal_MutexObj_t gphTools_Q_MsgMutex;
static phOsal_SemObj_t gphTools_Q_MsgSema;

static phTools_Q_t gsphTools_Q_Pool[PH_TOOLS_Q_MAX_ELEMENTS];
static phTools_Q_t *gpphTools_Q_PoolHead;
static phTools_Q_t *gpphTools_Q_Msgs;
static uint8_t     bQCount;

static const uint8_t bToolsxSemaName[] = "ToolsxSema";
/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */
static phStatus_t phTools_Q_SendInt(phTools_Q_t *psMsgQueue, uint8_t bFront, uint32_t dwBlockTime, uint16_t wFrameOpt);
static phTools_Q_t* phTools_Q_PopInt(phTools_Q_t **ppQHead, phOsal_Mutex_t pMutex, uint32_t dwBlockTime, uint8_t bPriority);
static void phTools_Q_Push(phTools_Q_t **ppsQHead, phTools_Q_t *psMsgQueue, uint8_t bFront);

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

phStatus_t phTools_Q_Init(void)
{
    uint8_t count;
    phTools_Q_t *psQueue = NULL;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;

    bQCount = 0;
    gpphTools_Q_Msgs = NULL;
    gpphTools_Q_PoolHead = gsphTools_Q_Pool;
    psQueue = gsphTools_Q_Pool;
    gphTools_Q_MsgSema.pSemName = (uint8_t *)bToolsxSemaName;
    gphTools_Q_MsgSema.semInitialCount = 0;
    gphTools_Q_MsgSema.semMaxCount = PH_TOOLS_Q_MAX_ELEMENTS;

    PH_CHECK_SUCCESS_FCT(bRetstatus,phOsal_SemCreate(&gphTools_Q_MsgSema.SemHandle, &gphTools_Q_MsgSema, E_OS_SEM_OPT_COUNTING_SEM));
    if(gphTools_Q_MsgSema.SemHandle == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_TOOLS);
    }

    PH_CHECK_SUCCESS_FCT(bRetstatus,phOsal_MutexCreate(&gphTools_Q_Mutex.MutexHandle,&gphTools_Q_Mutex));
    if(gphTools_Q_Mutex.MutexHandle == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_TOOLS);
    }

    PH_CHECK_SUCCESS_FCT(bRetstatus,phOsal_MutexCreate(&gphTools_Q_MsgMutex.MutexHandle,&gphTools_Q_MsgMutex));
    if(gphTools_Q_Mutex.MutexHandle == NULL)
    {
        return (PH_ERR_RESOURCE_ERROR | PH_COMP_TOOLS);
    }

    for(count = 1; count < PH_TOOLS_Q_MAX_ELEMENTS; count++)
    {
        psQueue->pNext = &gsphTools_Q_Pool[count];
        psQueue = psQueue->pNext;
    }
    psQueue->pNext = NULL;

    return PH_ERR_SUCCESS;
}

phTools_Q_t *phTools_Q_Receive(uint32_t dwBlockTime)
{
    phStatus_t bRetstatus;
    phOsal_TimerPeriodObj_t timePeriodToWait;

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = dwBlockTime;

    /* wait for incoming messages */
    bRetstatus = phOsal_SemPend(&gphTools_Q_MsgSema.SemHandle, timePeriodToWait);
    if (bRetstatus != PH_ERR_SUCCESS)
    {
        return NULL;
    }

    return phTools_Q_PopInt(&gpphTools_Q_Msgs, gphTools_Q_MsgMutex.MutexHandle, dwBlockTime, PH_ON);
}

phTools_Q_t *phTools_Q_Get(uint32_t dwBlockTime, uint8_t bPriority)
{
    phTools_Q_t *pEmptyMsg = phTools_Q_PopInt(&gpphTools_Q_PoolHead, gphTools_Q_Mutex.MutexHandle, dwBlockTime, bPriority);

    if(pEmptyMsg == NULL)
    {
        return pEmptyMsg;
    }

    pEmptyMsg->pNext = NULL;

    return pEmptyMsg;
}

static phTools_Q_t *phTools_Q_PopInt(phTools_Q_t **ppQHead, phOsal_Mutex_t pMutex, uint32_t dwBlockTime, uint8_t bPriority)
{
    void *pQueue;
    phOsal_TimerPeriodObj_t timePeriodToWait;

    phStatus_t bRetstatus;

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = dwBlockTime;

    bRetstatus = phOsal_MutexLock(&pMutex, timePeriodToWait);
    if (bRetstatus != PH_ERR_SUCCESS)
    {
        return NULL;
    }

    if((bPriority == PH_OFF) && (bQCount >= (PH_TOOLS_Q_MAX_ELEMENTS - PH_TOOLS_Q_RESERVED_ELEMENTS)) && (pMutex == gphTools_Q_Mutex.MutexHandle))
    {
        return NULL;
    }

    /* Get a msg Queue from the pool */
    if(*ppQHead == NULL)
    {
        return NULL;
    }
    pQueue = *ppQHead;
    *ppQHead = (*ppQHead)->pNext;

    if(pMutex == gphTools_Q_Mutex.MutexHandle)
    {
        bQCount++;
    }

    bRetstatus = phOsal_MutexUnLock(&pMutex);
    if (bRetstatus != PH_ERR_SUCCESS)
    {
        return NULL;
    }

    return pQueue;
}

phStatus_t phTools_Q_Release(phTools_Q_t *psMsgQueue, uint32_t dwBlockTime)
{
    phStatus_t wStatus;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;
    phOsal_TimerPeriodObj_t timePeriodToWait;

    psMsgQueue->pNext = NULL;
    psMsgQueue->bType = 0U;
    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = dwBlockTime;

    PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_MutexLock(&gphTools_Q_Mutex.MutexHandle, timePeriodToWait));

    if(gpphTools_Q_PoolHead == NULL)
    {
        gpphTools_Q_PoolHead = psMsgQueue;
        gpphTools_Q_PoolHead->pNext = NULL;
        return PH_ERR_SUCCESS;
    }

    psMsgQueue->pNext = gpphTools_Q_PoolHead;
    gpphTools_Q_PoolHead = psMsgQueue;
    bQCount--;
    PH_CHECK_SUCCESS_FCT(wStatus, phOsal_MutexUnLock(&gphTools_Q_Mutex.MutexHandle));

    return PH_ERR_SUCCESS;
}

static phStatus_t phTools_Q_SendInt(phTools_Q_t *psMsgQueue, uint8_t bFront, uint32_t dwBlockTime, uint16_t wFrameOpt)
{
    static uint8_t bNoOfMsgs = 0;
    uint8_t i;
    phStatus_t bRetstatus = PH_ERR_INTERNAL_ERROR;
    phOsal_TimerPeriodObj_t timePeriodToWait;

    timePeriodToWait.unitPeriod = OS_TIMER_UNIT_MSEC;
    timePeriodToWait.period = dwBlockTime;

    /* Shared memory pool, Mutex  protected until all the fragments are queued. */
    if((wFrameOpt == PH_TRANSMIT_BUFFER_FIRST) || (wFrameOpt == PH_TRANSMIT_DEFAULT))
    {
        bNoOfMsgs = 1;
        PH_CHECK_SUCCESS_FCT(bRetstatus,phOsal_MutexLock(&gphTools_Q_MsgMutex.MutexHandle, timePeriodToWait));
    }
    else
    {
        bNoOfMsgs++;
    }

    phTools_Q_Push(&gpphTools_Q_Msgs, psMsgQueue, bFront);

    /* Check for the last fragment to be queued, post the semaphore. */
    if((wFrameOpt == PH_TRANSMIT_BUFFER_LAST) || (wFrameOpt == PH_TRANSMIT_DEFAULT))
    {
        /* TODO: chk for posting single semaphore instead multi No of Semaphore. */
        for(i=0; i<bNoOfMsgs;i++)
        {
            PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_SemPost(&gphTools_Q_MsgSema.SemHandle, E_OS_SEM_OPT_NONE));
        }
        PH_CHECK_SUCCESS_FCT(bRetstatus, phOsal_MutexUnLock(&gphTools_Q_MsgMutex.MutexHandle));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phTools_Q_Send(
                          phTools_Q_t* psMsgQueue,
                          uint32_t dwBlockTime,
                          uint16_t wFrameOpt
                          )
{
    return phTools_Q_SendInt(psMsgQueue, (uint8_t)PH_OFF, dwBlockTime, wFrameOpt);
}

phStatus_t phTools_Q_SendFront(
                               phTools_Q_t* psMsgQueue,
                               uint32_t dwBlockTime,
                               uint16_t wFrameOpt
                               )
{
    return phTools_Q_SendInt(psMsgQueue, (uint8_t)PH_ON, dwBlockTime, wFrameOpt);
}

static void phTools_Q_Push(phTools_Q_t **ppsQHead, phTools_Q_t *psMsgQueue, uint8_t bFront)
{
    phTools_Q_t *psQList = NULL;

    psMsgQueue->pNext = NULL;

    if(*ppsQHead == NULL)
    {
        *ppsQHead = psMsgQueue;
        return;
    }

    /* Queue to the head */
    if(bFront == PH_ON)
    {
        psMsgQueue->pNext = *ppsQHead;
        *ppsQHead = psMsgQueue;
    }
    else
    {
        /* Queue to the tail */
        psQList = *ppsQHead;
        while(psQList->pNext != NULL)
        {
            psQList = psQList->pNext;
        }
        psQList->pNext = psMsgQueue;
    }

    return;
}

void phTools_Q_DeInit(void)
{
    gpphTools_Q_PoolHead = NULL;
    bQCount = 0;

    /* Release the acquired memory. */
    if (gphTools_Q_Mutex.MutexHandle != NULL)
    {
        (void)phOsal_MutexDelete(&gphTools_Q_Mutex.MutexHandle);
        gphTools_Q_Mutex.MutexHandle = NULL;
    }
    if (gphTools_Q_MsgMutex.MutexHandle != NULL)
    {
        (void)phOsal_MutexDelete(&gphTools_Q_MsgMutex.MutexHandle);
        gphTools_Q_MsgMutex.MutexHandle = NULL;
    }
    if (gphTools_Q_MsgSema.SemHandle != NULL)
    {
        (void)phOsal_SemDelete(&gphTools_Q_MsgSema.SemHandle);
        gphTools_Q_MsgSema.SemHandle = NULL;
    }
}
