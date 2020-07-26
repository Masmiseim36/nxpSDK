/*
*                     Copyright 2016-2017,2019 NXP
*
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
 * Freertos OSAL Component of Reader Library Framework.
 * $Author$
 * $Revision$
 * $Date$
 *
 * History:
 *  PC: Generated 23. Aug 2012
 *
 */
#include <phOsal.h>

#ifdef PH_OSAL_FREERTOS
#include <FreeRTOS.h>
#include <event_groups.h>
#include "phOsal_Freertos.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <projdefs.h>
#include <portmacro.h>

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
static StaticTask_t sTaskBuffer[PH_OSAL_CONFIG_MAX_NUM_TASKS];
static volatile uint32_t dwTotalTasksCreated;
static volatile uint32_t dwTaskBitMap;

static StaticEventGroup_t sEventGroup[PH_OSAL_CONFIG_MAX_NUM_EVENTS];
static volatile uint32_t dwTotalEventsCreated;
static volatile uint32_t dwEventBitMap;

static StaticSemaphore_t sSemaphoreBuff[PH_OSAL_CONFIG_MAX_NUM_SEMAPHORE];
static volatile uint32_t dwTotalSemCreated;
static volatile uint32_t dwSemBitMap;

static StaticSemaphore_t sMutexBuff[PH_OSAL_CONFIG_MAX_NUM_MUTEX];
static volatile uint32_t dwTotalMutexCreated;
static volatile uint32_t dwMutexBitMap;

static StaticTimer_t sTimerBuffer[PH_OSAL_CONFIG_MAX_NUM_TIMERS];
static volatile uint32_t dwTotalTimersCreated;
static volatile uint32_t dwTimerBitMap;
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
static phStatus_t phOsal_FreeRtos_GetFreeIndex(uint32_t * dwFreeIndex, uint32_t dwBitMap, uint32_t dwMaxLimit);
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
static phStatus_t phOsal_TimeToTick(phOsal_TimerPeriodObj_t timerObj, TickType_t * TimerPeriod);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phOsal_Init(void)
{
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    dwTotalTasksCreated         = 0;
    dwTaskBitMap                = 0;

    dwTotalEventsCreated        = 0;
    dwEventBitMap               = 0;

    dwTotalSemCreated           = 0;
    dwSemBitMap                 = 0;

    dwTotalMutexCreated         = 0;
    dwMutexBitMap               = 0;

    dwTotalTimersCreated        = 0;
    dwTimerBitMap               = 0;
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_ThreadCreate(phOsal_Thread_t *threadHandle, const pphOsal_ThreadObj_t threadObj, pphOsal_StartFunc_t startFunc, void *arg)
{
    BaseType_t status;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    uint32_t dwFreeIndex;

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    if((threadHandle == NULL) || (threadObj == NULL) || (startFunc == NULL))
    {
        return PH_OSAL_ADD_COMPCODE(PH_OSAL_ERROR, PH_COMP_OSAL);
    }

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    if(threadObj->stackSizeInNum)
    {
        PH_OSAL_CHECK_SUCCESS(phOsal_FreeRtos_GetFreeIndex(&dwFreeIndex, dwTaskBitMap, PH_OSAL_CONFIG_MAX_NUM_TASKS));

        memset(&sTaskBuffer[dwFreeIndex], 0, sizeof(StaticTask_t));

        *threadHandle = xTaskCreateStatic(startFunc,
            (const char *)(threadObj->pTaskName),
            threadObj->stackSizeInNum,
            arg,
            threadObj->priority,
            threadObj->pStackBuffer,
            &sTaskBuffer[dwFreeIndex]);

        status = pdPASS;
    }
    else
    {

#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

        status = xTaskCreate(startFunc,
            (const char *)threadObj->pTaskName,
            threadObj->stackSizeInNum,
            arg,
            threadObj->priority,
			(TaskHandle_t *)threadHandle);

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    }

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    if(status != pdPASS)
    {
        return PH_OSAL_ADD_COMPCODE(PH_OSAL_FAILURE, PH_COMP_OSAL);
    }

    threadObj->ThreadHandle = *threadHandle;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    if(threadObj->stackSizeInNum)
    {
        threadObj->dwThreadIndex = dwFreeIndex;
        dwTaskBitMap |= (1 << dwFreeIndex);
        dwTotalTasksCreated++;
    }

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_ThreadChangePrio(phOsal_Thread_t * threadHandle, uint32_t newPrio)
{
    if ((threadHandle == NULL) || ((*threadHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    vTaskPrioritySet(*threadHandle, newPrio);
    ((pphOsal_ThreadObj_t)threadHandle)->priority = newPrio;
    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_ThreadExit(void)
{
    TaskHandle_t threadHandle;

    threadHandle = xTaskGetCurrentTaskHandle();

    return phOsal_ThreadDelete((phOsal_Thread_t)threadHandle);
}

phStatus_t phOsal_ThreadDelete(phOsal_Thread_t * threadHandle)
{
    vTaskDelete(*threadHandle);

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    dwTaskBitMap &= ~(1 << (((pphOsal_ThreadObj_t)threadHandle)->dwThreadIndex));
    dwTotalTasksCreated--;
    memset(threadHandle, 0, sizeof(phOsal_ThreadObj_t));

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_ThreadDelay(phOsal_Ticks_t ticksToSleep)
{
    vTaskDelay(ticksToSleep);

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_EventCreate(phOsal_Event_t * eventHandle, pphOsal_EventObj_t eventObj)
{
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    uint32_t dwFreeIndex;

    PH_OSAL_CHECK_SUCCESS(phOsal_FreeRtos_GetFreeIndex(&dwFreeIndex, dwEventBitMap, PH_OSAL_CONFIG_MAX_NUM_EVENTS));

    memset(&sEventGroup[dwFreeIndex], 0, sizeof(StaticEventGroup_t));

    *eventHandle = xEventGroupCreateStatic(&sEventGroup[dwFreeIndex]);

#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    *eventHandle = xEventGroupCreate();

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    if(*eventHandle == NULL)
    {
        return PH_OSAL_ADD_COMPCODE(PH_OSAL_FAILURE, PH_COMP_OSAL);
    }

    eventObj->EventHandle = *eventHandle;
    eventObj->intialValue = 0;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    eventObj->dwEventIndex = dwFreeIndex;
    dwTotalEventsCreated++;
    dwEventBitMap |= (1 << dwFreeIndex);

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_EventPend(volatile phOsal_Event_t * eventHandle, phOsal_EventOpt_t options, phOsal_Ticks_t ticksToWait, phOsal_EventBits_t FlagsToWait, phOsal_EventBits_t *pCurrFlags)
{
    phOsal_EventBits_t CurrentFlags;

    if((eventHandle == NULL) || ((*eventHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    CurrentFlags = xEventGroupWaitBits(*eventHandle, FlagsToWait,
        (options & E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT) >> E_OS_EVENT_OPT_POS_PEND_CLEAR_ON_EXIT,
        (options & E_OS_EVENT_OPT_PEND_SET_ALL) >> E_OS_EVENT_OPT_POS_PEND_SET_ALL,
        ticksToWait);

    if (pCurrFlags != NULL)
    {
        *pCurrFlags = CurrentFlags;
    }

    if (E_OS_EVENT_OPT_PEND_SET_ALL & options)
    {
        if ((CurrentFlags & FlagsToWait) == FlagsToWait)
        {
            return PH_OSAL_SUCCESS;
        }
        else
        {
            return (PH_OSAL_IO_TIMEOUT | PH_COMP_OSAL);
        }
    }
    if (CurrentFlags & FlagsToWait)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_IO_TIMEOUT | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_EventPost(phOsal_Event_t * eventHandle, phOsal_EventOpt_t options, phOsal_EventBits_t FlagsToPost, phOsal_EventBits_t *pCurrFlags)
{
    BaseType_t HigherPriorityTaskWoken;
    phOsal_EventBits_t CurrentFlags;

    if((eventHandle == NULL) || ((*eventHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    HigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        CurrentFlags = xEventGroupSetBitsFromISR(*eventHandle, FlagsToPost, &HigherPriorityTaskWoken);
        if(CurrentFlags == pdPASS)
        {
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
    else
    {
        CurrentFlags = xEventGroupSetBits(*eventHandle, FlagsToPost);
    }

    if (pCurrFlags != NULL)
    {
        *pCurrFlags = CurrentFlags;
    }

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_EventClear(phOsal_Event_t * eventHandle, phOsal_EventOpt_t options, phOsal_EventBits_t FlagsToClear, phOsal_EventBits_t *pCurrFlags)
{
    phOsal_EventBits_t CurrentFlags;

    if((eventHandle == NULL) || ((*eventHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        CurrentFlags = xEventGroupClearBitsFromISR(*eventHandle, FlagsToClear);
    }
    else
    {
        CurrentFlags = xEventGroupClearBits(*eventHandle, FlagsToClear);
    }

    if (pCurrFlags != NULL)
    {
        *pCurrFlags = CurrentFlags;
    }

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_EventGet(phOsal_Event_t * eventHandle, phOsal_EventBits_t *pCurrFlags)
{
    phOsal_EventBits_t CurrentFlags;

    if((eventHandle == NULL) || ((*eventHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    CurrentFlags =  xEventGroupGetBits(*eventHandle);
    if (pCurrFlags != NULL)
    {
        *pCurrFlags = CurrentFlags;
    }

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_EventDelete(phOsal_Event_t * eventHandle)
{
    if((eventHandle == NULL) || ((*eventHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    vEventGroupDelete(*eventHandle);

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    dwEventBitMap &= ~(1<< (((pphOsal_EventObj_t)eventHandle)->dwEventIndex));
    dwTotalEventsCreated--;
    memset(eventHandle, 0, sizeof(phOsal_EventObj_t));

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_SemCreate(phOsal_Semaphore_t *semHandle, pphOsal_SemObj_t semObj,phOsal_SemOpt_t opt)
{
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    uint32_t dwFreeIndex;

    if (semHandle == NULL)
    {
        return PH_OSAL_ADD_COMPCODE(PH_OSAL_FAILURE, PH_COMP_OSAL);
    }

    PH_OSAL_CHECK_SUCCESS(phOsal_FreeRtos_GetFreeIndex(&dwFreeIndex, dwSemBitMap, PH_OSAL_CONFIG_MAX_NUM_SEMAPHORE));

    memset(&sSemaphoreBuff[dwFreeIndex], 0, sizeof(StaticSemaphore_t));

    if (opt == E_OS_SEM_OPT_COUNTING_SEM)
    {
        *semHandle = xSemaphoreCreateCountingStatic(semObj->semMaxCount, semObj->semInitialCount, &sSemaphoreBuff[dwFreeIndex]);
    }
    else
    {
        *semHandle = xSemaphoreCreateBinaryStatic(&sSemaphoreBuff[dwFreeIndex]);
        if (opt == E_OS_SEM_OPT_BINARY_SEM)
        {
            xSemaphoreGive(*semHandle);
        }

    }
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    if (semHandle == NULL)
    {
        return PH_OSAL_ADD_COMPCODE(PH_OSAL_FAILURE, PH_COMP_OSAL);
    }

    if (opt == E_OS_SEM_OPT_COUNTING_SEM)
    {
        *semHandle = xSemaphoreCreateCounting(semObj->semMaxCount, semObj->semInitialCount);
    }
    else
    {
        *semHandle = xSemaphoreCreateBinary();
        if (opt == E_OS_SEM_OPT_BINARY_SEM)
        {
            xSemaphoreGive(*semHandle);
        }
    }

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */


    semObj->SemHandle = *semHandle;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    semObj->dwSemIndex = dwFreeIndex;
    dwTotalSemCreated++;
    dwSemBitMap |= (1 << dwFreeIndex);

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_SemPend(phOsal_Semaphore_t * semHandle, phOsal_TimerPeriodObj_t timePeriodToWait)
{
    BaseType_t HigherPriorityTaskWoken;
    BaseType_t statusTmp;
    TickType_t TimerPeriod;

    HigherPriorityTaskWoken = pdFALSE;

    if ((semHandle == NULL) || ((*semHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    if (timePeriodToWait.period == PHOSAL_MAX_DELAY)
    {
        TimerPeriod = timePeriodToWait.period;
    }
    else
    {
        PH_OSAL_CHECK_SUCCESS(phOsal_TimeToTick(timePeriodToWait, &TimerPeriod));
    }

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        statusTmp = xSemaphoreTakeFromISR(*semHandle, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    }
    else
    {
        statusTmp = xSemaphoreTake(*semHandle, TimerPeriod);
    }

    if (statusTmp == pdPASS)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_IO_TIMEOUT | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_SemPost(phOsal_Semaphore_t * semHandle, phOsal_SemOpt_t opt)
{
    BaseType_t HigherPriorityTaskWoken;
    BaseType_t statusTmp;

    HigherPriorityTaskWoken = pdFALSE;

    if ((semHandle == NULL) || ((*semHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        statusTmp = xSemaphoreGiveFromISR(*semHandle, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    }
    else
    {
        statusTmp = xSemaphoreGive(*semHandle);
    }

    if (statusTmp == pdPASS)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_SemDelete(phOsal_Semaphore_t * semHandle)
{
    if ((semHandle == NULL) || ((*semHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    vSemaphoreDelete(*semHandle);

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    dwSemBitMap &= ~(1 << (((pphOsal_SemObj_t)semHandle)->dwSemIndex));
    dwTotalSemCreated--;
    memset(semHandle, 0, sizeof(phOsal_SemObj_t));

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_MutexCreate(phOsal_Mutex_t *mutexHandle, pphOsal_MutexObj_t mutexObj)
{
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    uint32_t dwFreeIndex;

    if (mutexHandle == NULL)
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }

    PH_OSAL_CHECK_SUCCESS(phOsal_FreeRtos_GetFreeIndex(&dwFreeIndex, dwMutexBitMap, PH_OSAL_CONFIG_MAX_NUM_MUTEX));

    memset(&sMutexBuff[dwFreeIndex], 0, sizeof(StaticSemaphore_t));

    *mutexHandle = xSemaphoreCreateMutexStatic(&sMutexBuff[dwFreeIndex]);

#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    if (mutexHandle == NULL)
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }

    *mutexHandle = xSemaphoreCreateMutex();

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */


    mutexObj->MutexHandle = *mutexHandle;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    mutexObj->dwMutexIndex = dwFreeIndex;
    dwTotalMutexCreated++;
    dwMutexBitMap |= (1 << dwFreeIndex);

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_MutexLock(phOsal_Mutex_t * mutexHandle, phOsal_TimerPeriodObj_t timePeriodToWait)
{
    if ((mutexHandle == NULL) || ((*mutexHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    if(xSemaphoreTake(*mutexHandle, portMAX_DELAY) == pdTRUE)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_MutexUnLock(phOsal_Mutex_t * mutexHandle)
{
    if ((mutexHandle == NULL) || ((*mutexHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    if(xSemaphoreGive(*mutexHandle)  == pdTRUE)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_MutexDelete(phOsal_Mutex_t * mutexHandle)
{
    if ((mutexHandle == NULL) || ((*mutexHandle) == NULL))
    {
        return (PH_OSAL_ERROR | PH_COMP_OSAL);
    }

    vSemaphoreDelete(*mutexHandle);

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    dwMutexBitMap &= ~(1 << (((pphOsal_MutexObj_t)mutexHandle)->dwMutexIndex));
    dwTotalMutexCreated--;
    memset(mutexHandle, 0, sizeof(phOsal_MutexObj_t));

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_TimerCreate(phOsal_Timer_t *timerHandle, pphOsal_TimerObj_t timerObj)
{
    TickType_t TimerPeriod;

    PH_OSAL_CHECK_SUCCESS(phOsal_TimeToTick(timerObj->timePeriod, &TimerPeriod));

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    uint32_t dwFreeIndex;

    if (timerHandle == NULL)
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }

    dwFreeIndex = 0;

    PH_OSAL_CHECK_SUCCESS(phOsal_FreeRtos_GetFreeIndex(&dwFreeIndex, dwTimerBitMap, PH_OSAL_CONFIG_MAX_NUM_TIMERS));

    memset(&sTimerBuffer[dwFreeIndex], 0, sizeof(StaticTimer_t));

    *timerHandle = xTimerCreateStatic(((const char *)timerObj->pTimerName), TimerPeriod, timerObj->opt, timerObj->arg,
        timerObj->timerCb, &sTimerBuffer[dwFreeIndex]);

#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    if (timerHandle == NULL)
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }

    *timerHandle = xTimerCreate(
        (const char *)timerObj->pTimerName, TimerPeriod, timerObj->opt, timerObj->arg,
		 (TimerCallbackFunction_t)timerObj->timerCb);

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    timerObj->TimerHandle = *timerHandle;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

    timerObj->dwTimerIndex = dwFreeIndex;
    dwTimerBitMap |= (1 << dwFreeIndex);
    dwTotalTimersCreated++;

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_TimerStart(phOsal_Timer_t * timerHandle)
{
    BaseType_t statusTmp;
    BaseType_t HigherPriorityTaskWoken;

    HigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        statusTmp = xTimerStartFromISR(*timerHandle, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    }
    else
    {
        statusTmp = xTimerStart(*timerHandle, 0);
    }

    if (statusTmp == pdPASS)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_TimerStop(phOsal_Timer_t * timerHandle)
{
    BaseType_t statusTmp;
    BaseType_t HigherPriorityTaskWoken;

    HigherPriorityTaskWoken = pdFALSE;

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        statusTmp = xTimerStopFromISR(*timerHandle, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    }
    else
    {
        statusTmp = xTimerStop(*timerHandle, 0);
    }
    if (statusTmp == pdPASS)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_TimerGetCurrent(phOsal_Timer_t * timerHandle, uint32_t * pdwGetElapsedTime)
{
#if tskKERNEL_VERSION_MAJOR >= 9  
    *pdwGetElapsedTime = xTimerGetExpiryTime(*timerHandle);
    return PH_OSAL_SUCCESS;
#else
    return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_OSAL);
#endif
}

phStatus_t phOsal_TimerModify(phOsal_Timer_t * timerHandle, pphOsal_TimerObj_t timerObj)
{
    BaseType_t statusTmp;
    BaseType_t HigherPriorityTaskWoken;
    TickType_t TimerPeriod;

    HigherPriorityTaskWoken = pdFALSE;

    PH_OSAL_CHECK_SUCCESS(phOsal_TimeToTick(timerObj->timePeriod, &TimerPeriod));

    if (xPortIsInsideInterrupt() == pdTRUE)
    {
        statusTmp = xTimerChangePeriodFromISR(*timerHandle, TimerPeriod, &HigherPriorityTaskWoken);
        portYIELD_FROM_ISR(HigherPriorityTaskWoken);
    }
    else
    {
        statusTmp = xTimerChangePeriod(*timerHandle, TimerPeriod, 0);
    }

    if (statusTmp == pdPASS)
    {
        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

phStatus_t phOsal_TimerDelete(phOsal_Timer_t * timerHandle)
{
    if (xTimerDelete(*timerHandle, 0) == pdPASS)
    {

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
        dwTimerBitMap &= ~(1 << (((pphOsal_TimerObj_t)timerHandle)->dwTimerIndex));
        dwTotalTimersCreated--;
        memset(timerHandle, 0, sizeof(phOsal_TimerObj_t));

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

        return PH_OSAL_SUCCESS;
    }
    else
    {
        return (PH_OSAL_FAILURE | PH_COMP_OSAL);
    }
}

void phOsal_StartScheduler(void)
{
    vTaskStartScheduler();
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION

static phStatus_t phOsal_FreeRtos_GetFreeIndex(uint32_t * dwFreeIndex, uint32_t dwBitMap, uint32_t dwMaxLimit)
{
    phStatus_t status;

    (*dwFreeIndex) = 0;

    while(((1 << (*dwFreeIndex)) & dwBitMap) && ((*dwFreeIndex) < dwMaxLimit))
    {
        (*dwFreeIndex)++;
    }

    if (*dwFreeIndex == dwMaxLimit)
    {
        status = PH_OSAL_FAILURE | PH_COMP_OSAL;
    }
    else
    {
        status = PH_OSAL_SUCCESS;
    }

    return status;
}

#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

static phStatus_t phOsal_TimeToTick(phOsal_TimerPeriodObj_t timerObj, TickType_t * TimerPeriod)
{
    phStatus_t status;

    status = PH_OSAL_SUCCESS;
    *TimerPeriod = 1;

    switch (timerObj.unitPeriod)
    {
    case OS_TIMER_UNIT_MSEC:
        *TimerPeriod = timerObj.period;
        break;
    case OS_TIMER_UNIT_SEC:
        *TimerPeriod = timerObj.period * 1000;
        break;
    default:
        status = PH_OSAL_ERROR;
        break;
    }

    *TimerPeriod = pdMS_TO_TICKS(*TimerPeriod);

    return PH_OSAL_ADD_COMPCODE(status, PH_COMP_OSAL);
}

#endif  /* NXPBUILD__PH_OSAL_FREERTOS */
