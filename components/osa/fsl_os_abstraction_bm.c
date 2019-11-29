/*!
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * ile
 *
 * This is the source file for the OS Abstraction layer for MQXLite.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "fsl_common.h"
#include "generic_list.h"
#include "fsl_os_abstraction.h"
#include "fsl_os_abstraction_bm.h"
#include <string.h>

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#endif

#ifdef DEBUG_ASSERT
#define OS_ASSERT(condition) \
    if (!(condition))        \
        while (1)            \
            ;
#else
#define OS_ASSERT(condition) (void)(condition);
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
/*! @brief State structure for bm osa manager. */
typedef struct _osa_state
{
    list_t taskList;
    task_handler_t curTaskHandler;
    volatile uint32_t interruptDisableCount;
    volatile uint32_t tickCounter;
    uint8_t mainTaskHandle[OSA_TASK_HANDLE_SIZE];
} osa_state_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
__WEAK_FUNC void main_task(osa_task_param_t arg);
__WEAK_FUNC void main_task(osa_task_param_t arg)
{
}
__WEAK_FUNC void OSA_TimeInit(void);
__WEAK_FUNC uint32_t OSA_TimeDiff(uint32_t time_start, uint32_t time_end);
osa_status_t OSA_Init(void);
void OSA_Start(void);

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
const uint8_t gUseRtos_c = USE_RTOS; /* USE_RTOS = 0 for BareMetal and 1 for OS */

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
static osa_state_t s_osaState;

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EnableIRQGlobal
 * Description   : Disable system interrupt.
 *
 *END**************************************************************************/
void OSA_EnableIRQGlobal(void)
{
    if (s_osaState.interruptDisableCount > 0U)
    {
        s_osaState.interruptDisableCount--;

        if (s_osaState.interruptDisableCount == 0U)
        {
            __enable_irq();
        }
        /* call core API to enable the global interrupt*/
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_DisableIRQGlobal
 * Description   : Disable system interrupt
 * This function will disable the global interrupt by calling the core API
 *
 *END**************************************************************************/
void OSA_DisableIRQGlobal(void)
{
    /* call core API to disable the global interrupt*/
    __disable_irq();

    /* update counter*/
    s_osaState.interruptDisableCount++;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetCurrentHandle
 * Description   : This function is used to get current active task's handler.
 *
 *END**************************************************************************/

osa_task_handle_t OSA_TaskGetCurrentHandle(void)
{
    return (osa_task_handle_t)s_osaState.curTaskHandler;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EXT_TaskYield
 * Description   : When a task calls this function, it will give up CPU and put
 * itself to the tail of ready list.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskYield(void)
{
    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetPriority
 * Description   : This function returns task's priority by task handler.
 *
 *END**************************************************************************/

osa_task_priority_t OSA_TaskGetPriority(osa_task_handle_t taskHandle)
{
    assert(taskHandle);
    task_handler_t handler = (task_handler_t)taskHandle;
    return handler->priority;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskSetPriority
 * Description   : This function sets task's priority by task handler.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskSetPriority(osa_task_handle_t taskHandle, osa_task_priority_t taskPriority)
{
    assert(taskHandle);
    list_element_handle_t list_element;
    task_control_block_t *tcb         = NULL;
    task_control_block_t *ptaskStruct = (task_control_block_t *)taskHandle;

    ptaskStruct->priority = taskPriority;
    (void)LIST_RemoveElement(&ptaskStruct->link);
    /* Insert task control block into the task list. */
    list_element = LIST_GetHead(&s_osaState.taskList);
    while (list_element != NULL)
    {
        tcb = (task_control_block_t *)(void *)list_element;
        if (ptaskStruct->priority <= tcb->priority)
        {
            (void)LIST_AddPrevElement(&tcb->link, &ptaskStruct->link);
            break;
        }
        list_element = LIST_GetNext(list_element);
    }
    if (ptaskStruct->priority > tcb->priority)
    {
        OSA_InterruptDisable();
        (void)LIST_AddTail(&s_osaState.taskList, (list_element_handle_t)(void *)&(ptaskStruct->link));
        OSA_InterruptEnable();
    }

    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskCreate
 * Description   : This function is used to create a task and make it ready.
 * Param[in]     :  threadDef  - Definition of the thread.
 *                  task_param - Parameter to pass to the new thread.
 * Return Thread handle of the new thread, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskCreate(osa_task_handle_t taskHandle, osa_task_def_t *thread_def, osa_task_param_t task_param)
{
    list_element_handle_t list_element;
    list_status_t listStatus;
    task_control_block_t *tcb         = NULL;
    task_control_block_t *ptaskStruct = (task_control_block_t *)taskHandle;
    assert(sizeof(task_control_block_t) == OSA_TASK_HANDLE_SIZE);
    assert(taskHandle);

    ptaskStruct->p_func    = thread_def->pthread;
    ptaskStruct->haveToRun = true;
    ptaskStruct->priority  = (uint16_t)PRIORITY_OSA_TO_RTOS(thread_def->tpriority);
    ptaskStruct->param     = task_param;

    list_element = LIST_GetHead(&s_osaState.taskList);
    while (list_element != NULL)
    {
        tcb = (task_control_block_t *)(void *)list_element;
        if (ptaskStruct->priority <= tcb->priority)
        {
            OSA_InterruptDisable();
            listStatus = LIST_AddPrevElement(&tcb->link, &ptaskStruct->link);
            if (listStatus == (list_status_t)kLIST_DuplicateError)
            {
                OSA_InterruptEnable();
                return KOSA_StatusError;
            }
            OSA_InterruptEnable();
            break;
        }
        list_element = LIST_GetNext(list_element);
    }

    if ((NULL == tcb) || (ptaskStruct->priority > tcb->priority))
    {
        OSA_InterruptDisable();
        listStatus = LIST_AddTail(&s_osaState.taskList, (list_element_handle_t)(void *)&(ptaskStruct->link));
        assert(listStatus == kLIST_Ok);
        OSA_InterruptEnable();
    }

    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskDestroy
 * Description   : This function destroy a task.
 * Param[in]     :taskHandle - Thread handle.
 * Return KOSA_StatusSuccess if the task is destroied, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskDestroy(osa_task_handle_t taskHandle)
{
    assert(taskHandle);

    OSA_InterruptDisable();
    (void)LIST_RemoveElement(taskHandle);
    OSA_InterruptEnable();
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeInit
 * Description   : This function initializes the timer used in BM OSA, the
 * functions such as OSA_TimeDelay, OSA_TimeGetMsec, and the timeout are all
 * based on this timer.
 *
 *END**************************************************************************/
__WEAK_FUNC void OSA_TimeInit(void)
{
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
    SysTick->LOAD = (uint32_t)(SystemCoreClock / 1000U - 1U);
    SysTick->VAL  = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeDiff
 * Description   : This function gets the difference between two time stamp,
 * time overflow is considered.
 *
 *END**************************************************************************/
__WEAK_FUNC uint32_t OSA_TimeDiff(uint32_t time_start, uint32_t time_end)
{
    if (time_end >= time_start)
    {
        return time_end - time_start;
    }
    else
    {
        return FSL_OSA_TIME_RANGE - time_start + time_end + 1UL;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA__TimeDelay
 * Description   : This function is used to suspend the active thread for the given number of milliseconds.
 *
 *END**************************************************************************/
void OSA_TimeDelay(uint32_t millisec)
{
    uint32_t currTime, timeStart;

    timeStart = OSA_TimeGetMsec();

    do
    {
        currTime = OSA_TimeGetMsec(); /* Get current time stamp */
    } while (millisec >= OSA_TimeDiff(timeStart, currTime));
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeGetMsec
 * Description   : This function gets current time in milliseconds.
 *
 *END**************************************************************************/
__WEAK_FUNC uint32_t OSA_TimeGetMsec(void)
{
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    return s_osaState.tickCounter;
#else
    return 0;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreCreate
 * Description   : This function is used to create a semaphore.
 * Return         : Semaphore handle of the new semaphore, or NULL if failed.
 *
 *END**************************************************************************/

osa_status_t OSA_SemaphoreCreate(osa_semaphore_handle_t semaphoreHandle, uint32_t initValue)
{
    semaphore_t *pSemStruct = (semaphore_t *)semaphoreHandle;
    assert(sizeof(semaphore_t) == OSA_SEM_HANDLE_SIZE);
    assert(semaphoreHandle);

    pSemStruct->semCount   = (uint8_t)initValue;
    pSemStruct->isWaiting  = false;
    pSemStruct->time_start = 0u;
    pSemStruct->timeout    = 0u;
    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreDestroy
 * Description   : This function is used to destroy a semaphore.
 * Return        : KOSA_StatusSuccess if the semaphore is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreDestroy(osa_semaphore_handle_t semaphoreHandle)
{
    assert(semaphoreHandle);
    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreWait
 * Description   : This function checks the semaphore's counting value, if it is
 * positive, decreases it and returns KOSA_StatusSuccess, otherwise, timeout
 * will be used for wait. The parameter timeout indicates how long should wait
 * in milliseconds. Pass osaWaitForever_c to wait indefinitely, pass 0 will
 * return KOSA_StatusTimeout immediately if semaphore is not positive.
 * This function returns KOSA_StatusSuccess if the semaphore is received, returns
 * KOSA_StatusTimeout if the semaphore is not received within the specified
 * 'timeout', returns KOSA_StatusError if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreWait(osa_semaphore_handle_t semaphoreHandle, uint32_t millisec)
{
    semaphore_t *pSemStruct = (semaphore_t *)semaphoreHandle;
    assert(semaphoreHandle);
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
#endif
    /* Check the sem count first. Deal with timeout only if not already set */

    if (pSemStruct->semCount != 0U)
    {
        OSA_DisableIRQGlobal();
        pSemStruct->semCount--;
        pSemStruct->isWaiting = false;
        OSA_EnableIRQGlobal();
        return KOSA_StatusSuccess;
    }
    else
    {
        if (0U == millisec)
        {
            /* If timeout is 0 and semaphore is not available, return kStatus_OSA_Timeout. */
            return KOSA_StatusTimeout;
        }
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
        else if (pSemStruct->isWaiting != false)
        {
            /* Check for timeout */
            currentTime = OSA_TimeGetMsec();
            if (pSemStruct->timeout < OSA_TimeDiff(pSemStruct->time_start, currentTime))
            {
                OSA_DisableIRQGlobal();
                pSemStruct->isWaiting = false;
                OSA_EnableIRQGlobal();
                return KOSA_StatusTimeout;
            }
        }
        else if (millisec != osaWaitForever_c) /* If don't wait forever, start the timer */
        {
            /* Start the timeout counter */
            OSA_DisableIRQGlobal();
            pSemStruct->isWaiting = true;
            OSA_EnableIRQGlobal();
            pSemStruct->time_start = OSA_TimeGetMsec();
            pSemStruct->timeout    = millisec;
        }
#endif
        else
        {
            ;
        }
    }

    return KOSA_StatusIdle;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphorePost
 * Description   : This function is used to wake up one task that wating on the
 * semaphore. If no task is waiting, increase the semaphore. The function returns
 * KOSA_StatusSuccess if the semaphre is post successfully, otherwise returns
 * KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphorePost(osa_semaphore_handle_t semaphoreHandle)
{
    semaphore_t *pSemStruct = (semaphore_t *)semaphoreHandle;
    assert(semaphoreHandle);

    /* The max value is 0xFF */
    if (0xFFU == pSemStruct->semCount)
    {
        return KOSA_StatusError;
    }
    OSA_DisableIRQGlobal();
    ++pSemStruct->semCount;
    OSA_EnableIRQGlobal();

    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexCreate
 * Description   : This function is used to create a mutex.
 * Return        : Mutex handle of the new mutex, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexCreate(osa_mutex_handle_t mutexHandle)
{
    mutex_t *pMutexStruct = (mutex_t *)mutexHandle;
    assert(sizeof(mutex_t) == OSA_MUTEX_HANDLE_SIZE);
    assert(mutexHandle);

    pMutexStruct->isLocked   = false;
    pMutexStruct->isWaiting  = false;
    pMutexStruct->time_start = 0u;
    pMutexStruct->timeout    = 0u;
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexLock
 * Description   : This function checks the mutex's status, if it is unlocked,
 * lock it and returns KOSA_StatusSuccess, otherwise, wait for the mutex.
 * MQX does not support timeout to wait for a mutex.
 * This function returns KOSA_StatusSuccess if the mutex is obtained, returns
 * KOSA_StatusError if any errors occur during waiting. If the mutex has been
 * locked, pass 0 as timeout will return KOSA_StatusTimeout immediately.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexLock(osa_mutex_handle_t mutexHandle, uint32_t millisec)
{
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
#endif
    mutex_t *pMutexStruct = (mutex_t *)mutexHandle;

    /* Always check first. Deal with timeout only if not available. */
    if (pMutexStruct->isLocked == false)
    {
        /* Get the lock and return success */
        OSA_DisableIRQGlobal();
        pMutexStruct->isLocked  = true;
        pMutexStruct->isWaiting = false;
        OSA_EnableIRQGlobal();
        return KOSA_StatusSuccess;
    }
    else
    {
        if (0U == millisec)
        {
            /* If timeout is 0 and mutex is not available, return kStatus_OSA_Timeout. */
            return KOSA_StatusTimeout;
        }
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
        else if (pMutexStruct->isWaiting != false)
        {
            /* Check for timeout */
            currentTime = OSA_TimeGetMsec();
            if (pMutexStruct->timeout < OSA_TimeDiff(pMutexStruct->time_start, currentTime))
            {
                OSA_DisableIRQGlobal();
                pMutexStruct->isWaiting = false;
                OSA_EnableIRQGlobal();
                return KOSA_StatusTimeout;
            }
        }
        else if (millisec != osaWaitForever_c) /* If dont't wait forever, start timer. */
        {
            /* Start the timeout counter */
            OSA_DisableIRQGlobal();
            pMutexStruct->isWaiting = true;
            OSA_EnableIRQGlobal();
            pMutexStruct->time_start = OSA_TimeGetMsec();
            pMutexStruct->timeout    = millisec;
        }
#endif
        else
        {
            ;
        }
    }

    return KOSA_StatusIdle;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexUnlock
 * Description   : This function is used to unlock a mutex.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexUnlock(osa_mutex_handle_t mutexHandle)
{
    mutex_t *pMutexStruct = (mutex_t *)mutexHandle;
    assert(mutexHandle);

    OSA_DisableIRQGlobal();
    pMutexStruct->isLocked = false;
    OSA_EnableIRQGlobal();
    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexDestroy
 * Description   : This function is used to destroy a mutex.
 * Return        : KOSA_StatusSuccess if the lock object is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexDestroy(osa_mutex_handle_t mutexHandle)
{
    assert(mutexHandle);
    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventCreate
 * Description   : This function is used to create a event object.
 * Return        : Event handle of the new event, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventCreate(osa_event_handle_t eventHandle, uint8_t autoClear)
{
    event_t *pEventStruct = eventHandle;
    assert(sizeof(event_t) == OSA_EVENT_HANDLE_SIZE);
    assert(eventHandle);

    pEventStruct->isWaiting   = false;
    pEventStruct->flags       = 0;
    pEventStruct->autoClear   = (bool)autoClear;
    pEventStruct->time_start  = 0u;
    pEventStruct->timeout     = 0u;
    pEventStruct->waitingTask = NULL;
    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventSet
 * Description   : Set one or more event flags of an event object.
 * Return        : KOSA_StatusSuccess if set successfully, KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventSet(osa_event_handle_t eventHandle, osa_event_flags_t flagsToSet)
{
    event_t *pEventStruct;
    pEventStruct = (event_t *)eventHandle;
    /* Set flags ensuring atomic operation */
    OSA_DisableIRQGlobal();
    pEventStruct->flags |= flagsToSet;
    if (pEventStruct->waitingTask != NULL)
    {
        pEventStruct->waitingTask->haveToRun = true;
    }
    OSA_EnableIRQGlobal();

    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventClear
 * Description   : Clear one or more event flags of an event object.
 * Return        :KOSA_StatusSuccess if clear successfully, KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventClear(osa_event_handle_t eventHandle, osa_event_flags_t flagsToClear)
{
    event_t *pEventStruct;
    pEventStruct = (event_t *)eventHandle;
    /* Clear flags ensuring atomic operation */
    OSA_DisableIRQGlobal();
    pEventStruct->flags &= ~flagsToClear;
    if (pEventStruct->flags != 0x00U)
    {
        if (pEventStruct->waitingTask != NULL)
        {
            pEventStruct->waitingTask->haveToRun = true;
        }
    }
    OSA_EnableIRQGlobal();

    return KOSA_StatusSuccess;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventWait
 * Description   : This function checks the event's status, if it meets the wait
 * condition, return KOSA_StatusSuccess, otherwise, timeout will be used for
 * wait. The parameter timeout indicates how long should wait in milliseconds.
 * Pass osaWaitForever_c to wait indefinitely, pass 0 will return the value
 * KOSA_StatusTimeout immediately if wait condition is not met. The event flags
 * will be cleared if the event is auto clear mode. Flags that wakeup waiting
 * task could be obtained from the parameter setFlags.
 * This function returns KOSA_StatusSuccess if wait condition is met, returns
 * KOSA_StatusTimeout if wait condition is not met within the specified
 * 'timeout', returns KOSA_StatusError if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_EventWait(osa_event_handle_t eventHandle,
                           osa_event_flags_t flagsToWait,
                           uint8_t waitAll,
                           uint32_t millisec,
                           osa_event_flags_t *pSetFlags)
{
    event_t *pEventStruct;
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
#endif
    osa_status_t retVal = KOSA_StatusIdle;
    if (pSetFlags == NULL)
    {
        return KOSA_StatusError;
    }

    pEventStruct = (event_t *)eventHandle;

    OSA_DisableIRQGlobal();
#if (TASK_MAX_NUM > 0)
    pEventStruct->waitingTask = OSA_TaskGetCurrentHandle();
#endif

    *pSetFlags = pEventStruct->flags & flagsToWait;

    /* Check the event flag first, if does not meet wait condition, deal with timeout. */
    if (((waitAll == 0U) && (*pSetFlags != 0U)) || (*pSetFlags == flagsToWait))
    {
        pEventStruct->isWaiting = false;
        if (true == pEventStruct->autoClear)
        {
            pEventStruct->flags &= ~flagsToWait;
        }
        retVal = KOSA_StatusSuccess;
    }
    else
    {
        if (0U == millisec)
        {
            /* If timeout is 0 and wait condition is not met, return kStatus_OSA_Timeout. */
            retVal = KOSA_StatusTimeout;
        }
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
        else if (pEventStruct->isWaiting != false)
        {
            /* Check for timeout */
            currentTime = OSA_TimeGetMsec();
            if (pEventStruct->timeout < OSA_TimeDiff(pEventStruct->time_start, currentTime))
            {
                pEventStruct->isWaiting = false;
                retVal                  = KOSA_StatusTimeout;
            }
        }
        else if (millisec != osaWaitForever_c) /* If no timeout, don't start the timer */
        {
            /* Start the timeout counter */
            pEventStruct->isWaiting  = true;
            pEventStruct->time_start = OSA_TimeGetMsec();
            pEventStruct->timeout    = millisec;
        }
#endif
        else
        {
            pEventStruct->waitingTask->haveToRun = false;
        }
    }

    OSA_EnableIRQGlobal();

    return retVal;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventDestroy
 * Description   : This function is used to destroy a event object. Return
 * KOSA_StatusSuccess if the event object is destroyed successfully, otherwise
 * return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_EventDestroy(osa_event_handle_t eventHandle)
{
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQCreate
 * Description   : This function is used to create a message queue.
 * Return        : the handle to the message queue if create successfully, otherwise
 * return NULL.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQCreate(osa_msgq_handle_t msgqHandle, uint32_t msgNo)
{
    msg_queue_t *pMsgQStruct = msgqHandle;
    assert(sizeof(msg_queue_t) == OSA_MSGQ_HANDLE_SIZE);
    assert(msgqHandle);

    pMsgQStruct->max      = (uint16_t)msgNo;
    pMsgQStruct->number   = 0;
    pMsgQStruct->head     = 0;
    pMsgQStruct->tail     = 0;
    pMsgQStruct->queueMem = (uint32_t *)((uint32_t)(uint32_t *)msgqHandle + sizeof(msg_queue_t));
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQPut
 * Description   : This function is used to put a message to a message queue.
 * Return         : KOSA_StatusSuccess if the message is put successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQPut(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage)
{
    assert(msgqHandle);
    msg_queue_t *pQueue;
    osa_status_t status = KOSA_StatusSuccess;
    if (msgqHandle == NULL)
    {
        return KOSA_StatusError;
    }

    pQueue = (msg_queue_t *)msgqHandle;

    OSA_DisableIRQGlobal();
    if (pQueue->number >= pQueue->max)
    {
        status = KOSA_StatusError;
    }
    else
    {
        pQueue->queueMem[pQueue->tail] = *((uint32_t *)pMessage);
        pQueue->number++;
        pQueue->tail++;

        if (pQueue->tail >= pQueue->max)
        {
            pQueue->tail = 0;
        }

        if (pQueue->waitingTask != NULL)
        {
            pQueue->waitingTask->haveToRun = true;
        }
    }
    OSA_EnableIRQGlobal();
    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQGet
 * Description   : This function checks the queue's status, if it is not empty,
 * get message from it and return KOSA_StatusSuccess, otherwise, timeout will
 * be used for wait. The parameter timeout indicates how long should wait in
 * milliseconds. Pass osaWaitForever_c to wait indefinitely, pass 0 will return
 * KOSA_StatusTimeout immediately if queue is empty.
 * This function returns KOSA_StatusSuccess if message is got successfully,
 * returns KOSA_StatusTimeout if message queue is empty within the specified
 * 'timeout', returns KOSA_StatusError if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQGet(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage, uint32_t millisec)
{
    assert(msgqHandle);
    msg_queue_t *pQueue;
    osa_status_t status = KOSA_StatusSuccess;

#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
#endif

    pQueue              = (msg_queue_t *)msgqHandle;
    pQueue->waitingTask = OSA_TaskGetCurrentHandle();

    OSA_DisableIRQGlobal();
    if (pQueue->number != 0U)
    {
        *((uint32_t *)pMessage) = pQueue->queueMem[pQueue->head];
        pQueue->number--;
        pQueue->head++;
        pQueue->isWaiting = false;

        if (pQueue->head >= pQueue->max)
        {
            pQueue->head = 0;
        }
        status = KOSA_StatusSuccess;
    }
    else
    {
        if (0U == millisec)
        {
            /* If timeout is 0 and wait condition is not met, return kStatus_OSA_Timeout. */
            status = KOSA_StatusTimeout;
        }
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
        else if (pQueue->isWaiting != false)
        {
            /* Check for timeout */
            currentTime = OSA_TimeGetMsec();
            if (pQueue->timeout < OSA_TimeDiff(pQueue->time_start, currentTime))
            {
                pQueue->isWaiting = false;
                status            = KOSA_StatusTimeout;
            }
        }
        else if (millisec != osaWaitForever_c) /* If no timeout, don't start the timer */
        {
            /* Start the timeout counter */
            pQueue->isWaiting  = true;
            pQueue->time_start = OSA_TimeGetMsec();
            pQueue->timeout    = millisec;
        }
#endif
        else
        {
            pQueue->waitingTask->haveToRun = false;
            status                         = KOSA_StatusTimeout;
        }
    }
    OSA_EnableIRQGlobal();

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EXT_MsgQDestroy
 * Description   : This function is used to destroy the message queue.
 * Return        : KOSA_StatusSuccess if the message queue is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQDestroy(osa_msgq_handle_t msgqHandle)
{
    assert(msgqHandle);
    msg_queue_t *pQueue;
    osa_status_t status = KOSA_StatusSuccess;

    pQueue = (msg_queue_t *)msgqHandle;

    if (pQueue->waitingTask != NULL)
    {
        pQueue->waitingTask->haveToRun = true;
        pQueue->waitingTask            = NULL;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InterruptEnable
 * Description   : self explanatory.
 *
 *END**************************************************************************/
void OSA_InterruptEnable(void)
{
    OSA_EnableIRQGlobal();
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InterruptDisable
 * Description   : self explanatory.
 *
 *END**************************************************************************/
void OSA_InterruptDisable(void)
{
    OSA_DisableIRQGlobal();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InstallIntHandler
 * Description   : This function is used to install interrupt handler.
 *
 *END**************************************************************************/
void OSA_InstallIntHandler(uint32_t IRQNumber, void (*handler)(void))
{
#if defined(__IAR_SYSTEMS_ICC__)
    _Pragma("diag_suppress = Pm138")
#endif
#if defined(ENABLE_RAM_VECTOR_TABLE)
        (void) InstallIRQHandler((IRQn_Type)IRQNumber, (uint32_t) * (uint32_t *)&handler);
#endif /* ENABLE_RAM_VECTOR_TABLE. */
#if defined(__IAR_SYSTEMS_ICC__)
    _Pragma("diag_remark = PM138")
#endif
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#if (osCustomStartup == 0)

static OSA_TASK_DEFINE(main_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, 0);

int main(void)
{
    extern void BOARD_InitHardware(void);
    (void)OSA_Init();
    /* Initialize MCU clock */
    BOARD_InitHardware();
    OSA_TimeInit();
    (void)OSA_TaskCreate((osa_task_handle_t)s_osaState.mainTaskHandle, OSA_TASK(main_task), NULL);
    OSA_Start();

    return 0;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Init
 * Description   : This function is used to setup the basic services, it should
 * be called first in function main. Return kStatus_OSA_Success if services
 * are initialized successfully, otherwise return kStatus_OSA_Error.
 *
 *END**************************************************************************/
osa_status_t OSA_Init(void)
{
    LIST_Init((&s_osaState.taskList), 0);
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Start
 * Description   : This function is used to start RTOS scheduler.
 *
 *END**************************************************************************/
void OSA_Start(void)
{
    list_element_handle_t list_element;
    task_control_block_t *tcb;

    for (;;)
    {
        list_element = LIST_GetHead(&s_osaState.taskList);
        while (list_element != NULL)
        {
            tcb                       = (task_control_block_t *)(void *)list_element;
            s_osaState.curTaskHandler = (osa_task_handle_t)tcb;
            if (tcb->haveToRun != false)
            {
                if (tcb->p_func != NULL)
                {
                    tcb->p_func(tcb->param);
                }
            }
            list_element = LIST_GetNext(list_element);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SysTick_Handler
 * Description   : This ISR of the SYSTICK timer.
 *
 *END**************************************************************************/
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
void SysTick_Handler(void);
void SysTick_Handler(void)
{
    s_osaState.tickCounter++;
}
#endif
