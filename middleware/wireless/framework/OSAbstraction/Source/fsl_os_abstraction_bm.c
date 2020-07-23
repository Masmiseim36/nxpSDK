/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the OS Abstraction layer for MQXLite.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "fsl_os_abstraction_bm.h"
#include "fsl_common.h"
#include "board.h"
#include <string.h>
#include "GenericList.h"

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
#elif defined( __CC_ARM )
#define __WEAK_FUNC __weak
#endif

#ifdef DEBUG_ASSERT
#define OS_ASSERT(condition) if(!(condition))while(TRUE);
#else
#define OS_ASSERT(condition) (void)(condition);
#endif

#if (osNumberOfSemaphores || osNumberOfMutexes || osNumberOfEvents || osNumberOfMessageQs)
#define osObjectAlloc_c 1
#else
#define osObjectAlloc_c 0
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef struct osMutexStruct_tag
  {
    uint32_t inUse;
    mutex_t mutex;
  }osMutexStruct_t;

typedef struct osEventStruct_tag
  {
    uint32_t inUse;
    event_t event;
  }osEventStruct_t;

typedef struct osSemaphoreStruct_tag
  {
    uint32_t inUse;
    semaphore_t semaphore;
  }osSemaphoreStruct_t;

typedef struct osMsgQStruct_tag
  {
    uint32_t inUse;
    msg_queue_t queue;
  }osMsgQStruct_t;

typedef struct osObjStruct_tag
  {
    uint32_t inUse;
    uint32_t osObj;
  }osObjStruct_t;

typedef struct osObjectInfo_tag
{
    void* pHeap;
    uint32_t objectStructSize;
    uint32_t objNo;
}osObjectInfo_t;


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#if osObjectAlloc_c
static void* osObjectAlloc(const osObjectInfo_t* pOsObjectInfo);
static bool_t osObjectIsAllocated(const osObjectInfo_t* pOsObjectInfo, void* pObjectStruct);
static osaStatus_t osObjectFree(const osObjectInfo_t* pOsObjectInfo, void* pObjectStruct);
#endif

extern void main_task(osaTaskParam_t arg);
void task_init(void);
__WEAK_FUNC void OSA_TimeInit(void);
__WEAK_FUNC uint32_t OSA_TimeDiff(uint32_t time_start, uint32_t time_end);
osaStatus_t OSA_Init(void);
void OSA_Start(void);
static void OSA_InsertTaskBefore(task_handler_t newTCB, task_handler_t currentTCB);

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
#define main_taskStack NULL

const uint8_t gUseRtos_c = USE_RTOS;  /* USE_RTOS = 0 for BareMetal and 1 for OS */

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

#if defined(osNumberOfSemaphores) && (osNumberOfSemaphores > 0)
static osSemaphoreStruct_t osSemaphoreHeap[osNumberOfSemaphores];
static const osObjectInfo_t osSemaphoreInfo = {osSemaphoreHeap, sizeof(osSemaphoreStruct_t),osNumberOfSemaphores};
#endif

#if defined(osNumberOfMutexes) && (osNumberOfMutexes > 0)
static osMutexStruct_t osMutexHeap[osNumberOfMutexes];
static const osObjectInfo_t osMutexInfo = {osMutexHeap, sizeof(osMutexStruct_t),osNumberOfMutexes};
#endif

#if defined(osNumberOfEvents) && (osNumberOfEvents > 0)
static osEventStruct_t osEventHeap[osNumberOfEvents];
static const osObjectInfo_t osEventInfo = {osEventHeap, sizeof(osEventStruct_t),osNumberOfEvents};
#endif

#if defined(osNumberOfMessageQs) && (osNumberOfMessageQs > 0)
static osMsgQStruct_t osMsgQHeap[osNumberOfMessageQs];
static const osObjectInfo_t osMsgQInfo = {osMsgQHeap, sizeof(osMsgQStruct_t),osNumberOfMessageQs};
#endif

#if (TASK_MAX_NUM > 0)

/* Global variales for task. */
static task_handler_t g_curTask; /* Current task. */
/*
 * All task control blocks in g_taskControlBlockPool will be linked as a
 * list, and the list is managed by the pointer g_freeTaskControlBlock.
 */
static task_control_block_t g_taskControlBlockPool[TASK_MAX_NUM];

/*
 * Pointer to the free task control blocks. To create a task, we should get
 * task control block from this pointer. When task is destroyed, the control
 * block will be returned and managed by this pointer.
 */
static task_control_block_t *g_freeTaskControlBlock;

/* Head node of task list, all tasks will be linked to this head node. */
static task_control_block_t *p_taskListHead = NULL;
#endif
static uint32_t gInterruptDisableCount = 0;
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
static uint32_t gTickCounter = 0;
#endif
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
  if (gInterruptDisableCount > 0U)
  {
    gInterruptDisableCount--;

    if (gInterruptDisableCount == 0U)
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
  gInterruptDisableCount++;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetId
 * Description   : This function is used to get current active task's handler.
 *
 *END**************************************************************************/

osaTaskId_t OSA_TaskGetId(void)
{
    return (osaTaskId_t)g_curTask;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EXT_TaskYield
 * Description   : When a task calls this function, it will give up CPU and put
 * itself to the tail of ready list.
 *
 *END**************************************************************************/
osaStatus_t OSA_TaskYield(void)
{
    return osaStatus_Success;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetPriority
 * Description   : This function returns task's priority by task handler.
 *
 *END**************************************************************************/

osaTaskPriority_t OSA_TaskGetPriority(osaTaskId_t taskId)
{
  task_handler_t handler;
  assert(taskId);

  handler = (task_handler_t)taskId;
  return handler->priority;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskSetPriority
 * Description   : This function sets task's priority by task handler.
 *
 *END**************************************************************************/
osaStatus_t OSA_TaskSetPriority(osaTaskId_t taskId, osaTaskPriority_t taskPriority)
{
    task_handler_t handler;
    task_handler_t p;
    assert(taskId);

    handler = (task_handler_t)taskId;

    /* Remove task control block from task list. */
    handler->prev->next = handler->next;
    handler->next->prev = handler->prev;
    if( handler == p_taskListHead )
    {
        p_taskListHead = handler->next;
    }

    handler->priority = taskPriority;
    /* Insert task control block into the task list. */
    if( handler->priority <= p_taskListHead->priority )
    {
        /* New task has the highest priority */
        OSA_InsertTaskBefore(handler, p_taskListHead);
        p_taskListHead = handler;
    }
    else if( handler->priority >= p_taskListHead->prev->priority )
    {
        /* New task has the lowest priority */
        OSA_InsertTaskBefore(handler, p_taskListHead);
    }
    else
    {
        p = p_taskListHead->next;

        while (TRUE)
        {
            if (handler->priority <= p->priority)
            {
                OSA_InsertTaskBefore(handler, p);
                break;
            }
            p = p->next;
        }
    }

    return osaStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : task_init
 * Description   : This function is used to initialize bare metal's task system,
 * it will prepare task control block pool and initialize corresponding
 * structures. This function should be called before creating any tasks.
 *
 *END**************************************************************************/
void task_init(void)
{
    int32_t i = TASK_MAX_NUM-1;

    g_taskControlBlockPool[i].next = NULL;

    while ((i--) != 0)
    {
        /* Link all task control blocks to a list. */
        g_taskControlBlockPool[i].next = &g_taskControlBlockPool[i+1];
    }

    g_freeTaskControlBlock = g_taskControlBlockPool;
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
osaTaskId_t OSA_TaskCreate(osaThreadDef_t *thread_def,osaTaskParam_t task_param)
{
    osaTaskId_t taskId = NULL;
    task_control_block_t *p_newTaskControlBlock;
    task_control_block_t *p_currentTaskControlBlock;

    if (g_freeTaskControlBlock != NULL)
    {
        /* Get new task control block from pool. */
        p_newTaskControlBlock         = g_freeTaskControlBlock;
        g_freeTaskControlBlock        = g_freeTaskControlBlock->next;
        /* Set task entry and parameter.*/
        p_newTaskControlBlock->p_func = thread_def->pthread;
        p_newTaskControlBlock->haveToRun = true;
        p_newTaskControlBlock->priority = (osaTaskPriority_t)PRIORITY_OSA_TO_RTOS(thread_def->tpriority);
        p_newTaskControlBlock->param  = task_param;
        p_newTaskControlBlock->next = NULL;
        p_newTaskControlBlock->prev = NULL;

        if (p_taskListHead == NULL)
        {
            p_taskListHead = p_newTaskControlBlock;
            p_taskListHead->next = p_taskListHead;
            p_taskListHead->prev = p_taskListHead;
        }
        else if (p_newTaskControlBlock->priority <= p_taskListHead->priority)
        {
            OSA_InsertTaskBefore(p_newTaskControlBlock, p_taskListHead);
            p_taskListHead = p_newTaskControlBlock;
        }
        else if (p_newTaskControlBlock->priority >= p_taskListHead->prev->priority)
        {
            OSA_InsertTaskBefore(p_newTaskControlBlock, p_taskListHead);
        }
        else
        {
            p_currentTaskControlBlock = p_taskListHead->next;

            for ( ;; )
            {
                if (p_newTaskControlBlock->priority <= p_currentTaskControlBlock->priority)
                {
                    OSA_InsertTaskBefore(p_newTaskControlBlock, p_currentTaskControlBlock);
                    break;
                }
                p_currentTaskControlBlock = p_currentTaskControlBlock->next;
            }

        }
        /* Task handler is pointer of task control block. */
        taskId = (osaTaskId_t)p_newTaskControlBlock;
    }

    return taskId;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskDestroy
 * Description   : This function destroy a task.
 * Param[in]     :taskId - Thread handle.
 * Return osaStatus_Success if the task is destroied, otherwise return osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_TaskDestroy(osaTaskId_t taskId)
{
  task_handler_t handler;
  osaStatus_t status = osaStatus_Success;
  if(taskId == NULL)
  {
    status = osaStatus_Error;
  }
  else
  {
    handler = (task_handler_t)taskId;
    if( handler == p_taskListHead )
    {
      p_taskListHead = handler->next;
    }

    /* Remove task control block from task list. */
    handler->prev->next = handler->next;
    handler->next->prev = handler->prev;

    /*
    * If current task is destroyed, then g_curTask will point to the previous
    * task, so that the subsequent tasks could be called. Check the function
    * OSA_Start for more details.
    */
    if (handler == g_curTask)
    {
      g_curTask = handler->prev;
    }

    /* Put task control block back to pool. */
    handler->prev = NULL;
    handler->next = g_freeTaskControlBlock;
    g_freeTaskControlBlock = handler;
  }
  return status;
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
    SysTick->LOAD = SystemCoreClock/1000U - 1U;
    SysTick->VAL = 0;
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
    uint32_t result;
    if (time_end >= time_start)
    {
        result = time_end - time_start;
    }
    else
    {
        result = FSL_OSA_TIME_RANGE - time_start + time_end + 1U;
    }
    return result;
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

    do {
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
    return gTickCounter;
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

osaSemaphoreId_t OSA_SemaphoreCreate(uint32_t initValue)
{
#if defined(osNumberOfSemaphores) && (osNumberOfSemaphores > 0)
  osaSemaphoreId_t semId = NULL;
  osSemaphoreStruct_t* pSemStruct;
  OSA_InterruptDisable();
  pSemStruct = (osSemaphoreStruct_t *)osObjectAlloc(&osSemaphoreInfo);
  semId = (osaSemaphoreId_t)pSemStruct;
  OSA_InterruptEnable();
  if(NULL != semId )
  {
    pSemStruct->semaphore.semCount  = (uint8_t)initValue;
    pSemStruct->semaphore.isWaiting = false;
    pSemStruct->semaphore.time_start = 0u;
    pSemStruct->semaphore.timeout = 0u;
  }
  return semId;
#else
  initValue=initValue;
  return NULL;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreDestroy
 * Description   : This function is used to destroy a semaphore.
 * Return        : osaStatus_Success if the semaphore is destroyed successfully, otherwise return osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_SemaphoreDestroy(osaSemaphoreId_t semId)
{
#if defined(osNumberOfSemaphores) && (osNumberOfSemaphores > 0)
  osaStatus_t err;
  OSA_InterruptDisable();
  err = osObjectFree(&osSemaphoreInfo, semId);
  OSA_InterruptEnable();
  return err;
#else
  semId=semId;
  return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreWait
 * Description   : This function checks the semaphore's counting value, if it is
 * positive, decreases it and returns osaStatus_Success, otherwise, timeout
 * will be used for wait. The parameter timeout indicates how long should wait
 * in milliseconds. Pass osaWaitForever_c to wait indefinitely, pass 0 will
 * return osaStatus_Timeout immediately if semaphore is not positive.
 * This function returns osaStatus_Success if the semaphore is received, returns
 * osaStatus_Timeout if the semaphore is not received within the specified
 * 'timeout', returns osaStatus_Error if any errors occur during waiting.
 *
 *END**************************************************************************/
osaStatus_t OSA_SemaphoreWait(osaSemaphoreId_t semId, uint32_t millisec)
{
#if defined(osNumberOfSemaphores) && (osNumberOfSemaphores > 0)
    osSemaphoreStruct_t* pSemStruct;
    osaStatus_t status = osaStatus_Idle;
    #if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
    #endif
    if(osObjectIsAllocated(&osSemaphoreInfo, semId) == FALSE)
    {
        status = osaStatus_Error;
    }
    else
    {
        pSemStruct = (osSemaphoreStruct_t*)semId;
        /* Check the sem count first. Deal with timeout only if not already set */

        if (pSemStruct->semaphore.semCount != 0U)
        {
            OSA_DisableIRQGlobal();
            pSemStruct->semaphore.semCount --;
            pSemStruct->semaphore.isWaiting = false;
            OSA_EnableIRQGlobal();
            status = osaStatus_Success;
        }
        else
        {
            if (0U == millisec)
            {
                /* If timeout is 0 and semaphore is not available, return kStatus_OSA_Timeout. */
                status = osaStatus_Timeout;
            }
            #if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
            else if (pSemStruct->semaphore.isWaiting)
            {
                /* Check for timeout */
                currentTime = OSA_TimeGetMsec();
                if (pSemStruct->semaphore.timeout < OSA_TimeDiff(pSemStruct->semaphore.time_start, currentTime))
                {
                    OSA_DisableIRQGlobal();
                    pSemStruct->semaphore.isWaiting = false;
                    OSA_EnableIRQGlobal();
                    status = osaStatus_Timeout;
                }
            }
            else if (millisec != osaWaitForever_c)    /* If don't wait forever, start the timer */
            {
                /* Start the timeout counter */
                OSA_DisableIRQGlobal();
                pSemStruct->semaphore.isWaiting = true;
                OSA_EnableIRQGlobal();
                pSemStruct->semaphore.time_start = OSA_TimeGetMsec();
                pSemStruct->semaphore.timeout = millisec;
            }
            #endif
            else
            {
                ;
            }
        }
    }
    return status;
#else
  semId=semId;
  millisec=millisec;
  return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphorePost
 * Description   : This function is used to wake up one task that wating on the
 * semaphore. If no task is waiting, increase the semaphore. The function returns
 * osaStatus_Success if the semaphre is post successfully, otherwise returns
 * osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_SemaphorePost(osaSemaphoreId_t semId)
{
#if defined(osNumberOfSemaphores) &&(osNumberOfSemaphores > 0)
  osSemaphoreStruct_t* pSemStruct;
  osaStatus_t status = osaStatus_Error;
  if(TRUE == osObjectIsAllocated(&osSemaphoreInfo, semId))
  {
      pSemStruct = (osSemaphoreStruct_t*)semId;
      /* The max value is 0xFF */
      if (0xFFU != pSemStruct->semaphore.semCount)
      {
          OSA_DisableIRQGlobal();
          ++pSemStruct->semaphore.semCount;
          OSA_EnableIRQGlobal();
          status = osaStatus_Success;
      }
  }
  return status;
#else /*defined(osNumberOfSemaphores) &&(osNumberOfSemaphores > 0)*/
  semId=semId;
  return osaStatus_Error;
#endif /*defined(osNumberOfSemaphores) &&(osNumberOfSemaphores > 0)*/
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexCreate
 * Description   : This function is used to create a mutex.
 * Return        : Mutex handle of the new mutex, or NULL if failed.
 *
 *END**************************************************************************/
osaMutexId_t OSA_MutexCreate(void)
{
#if defined(osNumberOfMutexes) && (osNumberOfMutexes > 0)
  osaMutexId_t mutexId = NULL;
  osMutexStruct_t* pMutexStruct;
  OSA_InterruptDisable();
  pMutexStruct = (osMutexStruct_t*)osObjectAlloc(&osMutexInfo);
  mutexId = (osaMutexId_t)pMutexStruct;
  OSA_InterruptEnable();
  if( NULL != mutexId )
  {
    pMutexStruct->mutex.isLocked  = false;
    pMutexStruct->mutex.isWaiting = false;
    pMutexStruct->mutex.time_start = 0u;
    pMutexStruct->mutex.timeout = 0u;
  }
  return mutexId;
#else
  return NULL;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexLock
 * Description   : This function checks the mutex's status, if it is unlocked,
 * lock it and returns osaStatus_Success, otherwise, wait for the mutex.
 * MQX does not support timeout to wait for a mutex.
 * This function returns osaStatus_Success if the mutex is obtained, returns
 * osaStatus_Error if any errors occur during waiting. If the mutex has been
 * locked, pass 0 as timeout will return osaStatus_Timeout immediately.
 *
 *END**************************************************************************/
osaStatus_t OSA_MutexLock(osaMutexId_t mutexId, uint32_t millisec)
{
#if defined(osNumberOfMutexes) && (osNumberOfMutexes > 0)
    osMutexStruct_t* pMutexStruct;
    osaStatus_t status = osaStatus_Idle;
    #if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
    #endif
    if(osObjectIsAllocated(&osMutexInfo, mutexId) == FALSE)
    {
        status = osaStatus_Error;
    }
    else
    {
        pMutexStruct = (osMutexStruct_t*)mutexId;

        /* Always check first. Deal with timeout only if not available. */
        if (pMutexStruct->mutex.isLocked == false)
        {
            /* Get the lock and return success */
            OSA_DisableIRQGlobal();
            pMutexStruct->mutex.isLocked = true;
            pMutexStruct->mutex.isWaiting = false;
            OSA_EnableIRQGlobal();
            status = osaStatus_Success;
        }
        else
        {
            if (0U == millisec)
            {
                /* If timeout is 0 and mutex is not available, return kStatus_OSA_Timeout. */
                status = osaStatus_Timeout;
            }
            #if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
            else if (pMutexStruct->mutex.isWaiting)
            {
                /* Check for timeout */
                currentTime = OSA_TimeGetMsec();
                if (pMutexStruct->mutex.timeout < OSA_TimeDiff(pMutexStruct->mutex.time_start, currentTime))
                {
                    OSA_DisableIRQGlobal();
                    pMutexStruct->mutex.isWaiting = false;
                    OSA_EnableIRQGlobal();
                    status = osaStatus_Timeout;
                }
            }
            else if (millisec != osaWaitForever_c)    /* If dont't wait forever, start timer. */
            {
              /* Start the timeout counter */
              OSA_DisableIRQGlobal();
              pMutexStruct->mutex.isWaiting = true;
              OSA_EnableIRQGlobal();
              pMutexStruct->mutex.time_start = OSA_TimeGetMsec();
              pMutexStruct->mutex.timeout = millisec;
            }
            #endif
            else
            {
                ;
            }
        }
    }
    return status;
#else
  mutexId=mutexId;
  millisec=millisec;
  return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexUnlock
 * Description   : This function is used to unlock a mutex.
 *
 *END**************************************************************************/
osaStatus_t OSA_MutexUnlock(osaMutexId_t mutexId)
{
#if defined(osNumberOfMutexes) &&(osNumberOfMutexes > 0)
  osaStatus_t status = osaStatus_Error;
  osMutexStruct_t* pMutexStruct;
  if(osObjectIsAllocated(&osMutexInfo, mutexId) == TRUE)
  {
    pMutexStruct = (osMutexStruct_t*)mutexId;
    OSA_DisableIRQGlobal();
    pMutexStruct->mutex.isLocked = false;
    OSA_EnableIRQGlobal();
    status = osaStatus_Success;
  }
  return status;
#else
  mutexId=mutexId;
  return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexDestroy
 * Description   : This function is used to destroy a mutex.
 * Return        : osaStatus_Success if the lock object is destroyed successfully, otherwise return osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_MutexDestroy(osaMutexId_t mutexId)
{
#if defined(osNumberOfMutexes) && (osNumberOfMutexes > 0)
  osaStatus_t err;
  OSA_InterruptDisable();
  err = osObjectFree(&osMutexInfo, mutexId);
  OSA_InterruptEnable();
  return err;
#else
  mutexId=mutexId;
  return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventCreate
 * Description   : This function is used to create a event object.
 * Return        : Event handle of the new event, or NULL if failed.
 *
 *END**************************************************************************/
osaEventId_t OSA_EventCreate(bool_t autoClear)
{
#if defined(osNumberOfEvents) && (osNumberOfEvents > 0)
  osaEventId_t eventId = NULL;
  osEventStruct_t* pEventStruct;
  OSA_InterruptDisable();
  pEventStruct = osObjectAlloc(&osEventInfo);
  eventId = pEventStruct;
  OSA_InterruptEnable();
  if(NULL != eventId)
  {
    pEventStruct->event.isWaiting = false;
    pEventStruct->event.flags     = 0;
    pEventStruct->event.autoClear = autoClear;
    pEventStruct->event.time_start = 0u;
    pEventStruct->event.timeout = 0u;
    pEventStruct->event.waitingTask = NULL;
  }
  return eventId;
#else
  (void)autoClear;
  return NULL;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventSet
 * Description   : Set one or more event flags of an event object.
 * Return        : osaStatus_Success if set successfully, osaStatus_Error if failed.
 *
 *END**************************************************************************/
osaStatus_t OSA_EventSet(osaEventId_t eventId, osaEventFlags_t flagsToSet)
{
#if defined(osNumberOfEvents) && (osNumberOfEvents > 0)
  osEventStruct_t* pEventStruct;
  osaStatus_t status = osaStatus_Error;
  if(osObjectIsAllocated(&osEventInfo, eventId) == TRUE)
  {
    pEventStruct = (osEventStruct_t*)eventId;
    /* Set flags ensuring atomic operation */
    OSA_DisableIRQGlobal();
    pEventStruct->event.flags |= flagsToSet;
    if (pEventStruct->event.waitingTask != NULL)
    {
      pEventStruct->event.waitingTask->haveToRun = true;
    }
    OSA_EnableIRQGlobal();
    status = osaStatus_Success;
  }
  return status;
#else
  (void)eventId;
  (void)flagsToSet;
  return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventClear
 * Description   : Clear one or more event flags of an event object.
 * Return        :osaStatus_Success if clear successfully, osaStatus_Error if failed.
 *
 *END**************************************************************************/
osaStatus_t OSA_EventClear(osaEventId_t eventId, osaEventFlags_t flagsToClear)
{
#if defined(osNumberOfEvents) && (osNumberOfEvents > 0)
  osEventStruct_t* pEventStruct;
  osaStatus_t status = osaStatus_Error;
  if(TRUE == osObjectIsAllocated(&osEventInfo, eventId))
  {
    pEventStruct = (osEventStruct_t*)eventId;
    /* Clear flags ensuring atomic operation */
    OSA_DisableIRQGlobal();
    pEventStruct->event.flags &= ~flagsToClear;
    if (pEventStruct->event.flags != 0x00U)
    {
      if (pEventStruct->event.waitingTask != NULL)
      {
        pEventStruct->event.waitingTask->haveToRun = true;
      }
    }
    OSA_EnableIRQGlobal();
    status = osaStatus_Success;
  }
  return status;
#else /*defined(osNumberOfEvents) && (osNumberOfEvents > 0)*/
  /*no events, return directly*/
  (void)eventId;
  (void)flagsToClear;
  return osaStatus_Error;
#endif /*defined(osNumberOfEvents) && (osNumberOfEvents > 0)*/
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventWait
 * Description   : This function checks the event's status, if it meets the wait
 * condition, return osaStatus_Success, otherwise, timeout will be used for
 * wait. The parameter timeout indicates how long should wait in milliseconds.
 * Pass osaWaitForever_c to wait indefinitely, pass 0 will return the value
 * osaStatus_Timeout immediately if wait condition is not met. The event flags
 * will be cleared if the event is auto clear mode. Flags that wakeup waiting
 * task could be obtained from the parameter setFlags.
 * This function returns osaStatus_Success if wait condition is met, returns
 * osaStatus_Timeout if wait condition is not met within the specified
 * 'timeout', returns osaStatus_Error if any errors occur during waiting.
 *
 *END**************************************************************************/
osaStatus_t OSA_EventWait(osaEventId_t eventId, osaEventFlags_t flagsToWait, bool_t waitAll, uint32_t millisec, osaEventFlags_t *pSetFlags)
{
#if defined(osNumberOfEvents) && (osNumberOfEvents > 0)
    osEventStruct_t* pEventStruct;
    #if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
    #endif
    osaStatus_t retVal = osaStatus_Idle;
    if(pSetFlags == NULL)
    {
        retVal = osaStatus_Error;
    }
    else
    {
        if(osObjectIsAllocated(&osEventInfo, eventId) == FALSE)
        {
            retVal = osaStatus_Error;
        }
        else
        {
            pEventStruct = (osEventStruct_t*)eventId;

            OSA_DisableIRQGlobal();
            #if (TASK_MAX_NUM > 0)
            pEventStruct->event.waitingTask = OSA_TaskGetId();
            #endif

            *pSetFlags = pEventStruct->event.flags & flagsToWait;

            /* Check the event flag first, if does not meet wait condition, deal with timeout. */
            if ( (((!waitAll) && (*pSetFlags != 0U))) || (*pSetFlags == flagsToWait) )
            {
                pEventStruct->event.isWaiting = false;
                if(TRUE == pEventStruct->event.autoClear)
                {
                    pEventStruct->event.flags &= ~flagsToWait;
                                pEventStruct->event.waitingTask->haveToRun = false;
                }
                retVal = osaStatus_Success;
            }
            else
            {
                if (0U == millisec)
                {
                    /* If timeout is 0 and wait condition is not met, return kStatus_OSA_Timeout. */
                    retVal = osaStatus_Timeout;
                }
                #if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
                else if (pEventStruct->event.isWaiting)
                {
                    /* Check for timeout */
                    currentTime = OSA_TimeGetMsec();
                    if (pEventStruct->event.timeout < OSA_TimeDiff(pEventStruct->event.time_start, currentTime))
                    {
                        pEventStruct->event.isWaiting = false;
                        retVal = osaStatus_Timeout;
                    }
                }
                else if(millisec != osaWaitForever_c)    /* If no timeout, don't start the timer */
                {
                    /* Start the timeout counter */
                    pEventStruct->event.isWaiting = true;
                    pEventStruct->event.time_start = OSA_TimeGetMsec();
                    pEventStruct->event.timeout = millisec;
                }
                #endif
                else
                {
                    pEventStruct->event.waitingTask->haveToRun = false;
                }
            }
            OSA_EnableIRQGlobal();
        }
    }
    return retVal;
#else /*defined(osNumberOfEvents) && (osNumberOfEvents > 0)*/
    (void)eventId;
    (void)flagsToWait;
    (void)waitAll;
    (void)millisec;
    (void)pSetFlags;
    return osaStatus_Error;
#endif /*defined(osNumberOfEvents) && (osNumberOfEvents > 0)*/
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventDestroy
 * Description   : This function is used to destroy a event object. Return
 * osaStatus_Success if the event object is destroyed successfully, otherwise
 * return osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_EventDestroy(osaEventId_t eventId)
{
#if defined(osNumberOfEvents) && (osNumberOfEvents > 0)
  osaStatus_t err;
  OSA_InterruptDisable();
  err = osObjectFree(&osEventInfo, eventId);
  OSA_InterruptEnable();
  return err;
#else
  (void)eventId;
  return osaStatus_Error;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQCreate
 * Description   : This function is used to create a message queue.
 * Return        : the handle to the message queue if create successfully, otherwise
 * return NULL.
 *
 *END**************************************************************************/
osaMsgQId_t OSA_MsgQCreate(uint32_t  msgNo)
{
#if defined(osNumberOfMessageQs) && (osNumberOfMessageQs > 0)
  osaMsgQId_t msgQId = NULL;
  osMsgQStruct_t* pMsgQStruct;

  if(msgNo <= (uint32_t)osNumberOfMessages)
  {
      OSA_InterruptDisable();
      pMsgQStruct = (osMsgQStruct_t *)osObjectAlloc(&osMsgQInfo);
      msgQId = (osaMsgQId_t)pMsgQStruct;
      OSA_InterruptEnable();

      if(msgQId != NULL)
      {
          pMsgQStruct->queue.max = (uint16_t)msgNo;
          pMsgQStruct->queue.number = 0;
          pMsgQStruct->queue.head = 0;
          pMsgQStruct->queue.tail = 0;
      }
  }

  return msgQId;
#else
  msgNo=msgNo;
  return NULL;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQPut
 * Description   : This function is used to put a message to a message queue.
* Return         : osaStatus_Success if the message is put successfully, otherwise return osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_MsgQPut(osaMsgQId_t msgQId, osaMsg_t pMessage)
{
#if defined(osNumberOfMessageQs) && (osNumberOfMessageQs > 0)
    msg_queue_t* pQueue;
    osaStatus_t status = osaStatus_Success;

    if(osObjectIsAllocated(&osMsgQInfo, msgQId) == FALSE)
    {
        status = osaStatus_Error;
    }
    else
    {
        pQueue = &((osMsgQStruct_t*)msgQId)->queue;

        OSA_DisableIRQGlobal();
        if( pQueue->number >= pQueue->max )
        {
            status = osaStatus_Error;
        }
        else
        {
            pQueue->queueMem[pQueue->tail] = *((uint32_t*)pMessage);
            pQueue->number++;
            pQueue->tail++;

            if( pQueue->tail >= pQueue->max )
            {
                pQueue->tail = 0;
            }

            if( pQueue->waitingTask != NULL)
            {
                pQueue->waitingTask->haveToRun = TRUE;
            }
        }
        OSA_EnableIRQGlobal();
    }

    return status;
#else
    msgQId=msgQId;
    pMessage=pMessage;
    return osaStatus_Error;
#endif
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQGet
 * Description   : This function checks the queue's status, if it is not empty,
 * get message from it and return osaStatus_Success, otherwise, timeout will
 * be used for wait. The parameter timeout indicates how long should wait in
 * milliseconds. Pass osaWaitForever_c to wait indefinitely, pass 0 will return
 * osaStatus_Timeout immediately if queue is empty.
 * This function returns osaStatus_Success if message is got successfully,
 * returns osaStatus_Timeout if message queue is empty within the specified
 * 'timeout', returns osaStatus_Error if any errors occur during waiting.
 *
 *END**************************************************************************/
osaStatus_t OSA_MsgQGet(osaMsgQId_t msgQId, osaMsg_t pMessage, uint32_t millisec)
{
#if defined(osNumberOfMessageQs) && (osNumberOfMessageQs > 0)
    osaStatus_t status = osaStatus_Idle;
    msg_queue_t* pQueue;
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
    uint32_t currentTime;
#endif

    if(osObjectIsAllocated(&osMsgQInfo, msgQId) == FALSE)
    {
        status = osaStatus_Error;
    }
    else
    {
        pQueue = &((osMsgQStruct_t*)msgQId)->queue;
        pQueue->waitingTask = OSA_TaskGetId();

        OSA_DisableIRQGlobal();
        if( pQueue->number != 0U)
        {
            *((uint32_t*)pMessage) = pQueue->queueMem[pQueue->head];
            pQueue->number--;
            pQueue->head++;
            pQueue->isWaiting = FALSE;

            if( pQueue->head >= pQueue->max )
            {
                pQueue->head = 0;
            }
            status = osaStatus_Success;
        }
        else
        {
            if (0U == millisec)
            {
                /* If timeout is 0 and wait condition is not met, return kStatus_OSA_Timeout. */
                status = osaStatus_Timeout;
            }
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
            else if (pQueue->isWaiting)
            {
                /* Check for timeout */
                currentTime = OSA_TimeGetMsec();
                if (pQueue->timeout < OSA_TimeDiff(pQueue->time_start, currentTime))
                {
                    pQueue->isWaiting = FALSE;
                    status = osaStatus_Timeout;
                }
            }
            else if(millisec != osaWaitForever_c)    /* If no timeout, don't start the timer */
            {
                /* Start the timeout counter */
                pQueue->isWaiting = TRUE;
                pQueue->time_start = OSA_TimeGetMsec();
                pQueue->timeout = millisec;
            }
#endif
            else
            {
                pQueue->waitingTask->haveToRun = FALSE;
            }
        }
        OSA_EnableIRQGlobal();
    }

    return status;
#else
    msgQId=msgQId;
    pMessage=pMessage;
    millisec=millisec;
    return osaStatus_Error;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EXT_MsgQDestroy
 * Description   : This function is used to destroy the message queue.
 * Return        : osaStatus_Success if the message queue is destroyed successfully, otherwise return osaStatus_Error.
 *
 *END**************************************************************************/
osaStatus_t OSA_MsgQDestroy(osaMsgQId_t msgQId)
{
#if defined(osNumberOfMessageQs) && (osNumberOfMessageQs > 0)
    osaStatus_t status = osaStatus_Success;
    msg_queue_t* pQueue;

    if(osObjectIsAllocated(&osMsgQInfo, msgQId) == FALSE)
    {
        status = osaStatus_Error;
    }
    else
    {
        pQueue = &((osMsgQStruct_t*)msgQId)->queue;

        if( pQueue->waitingTask != NULL )
        {
            pQueue->waitingTask->haveToRun = TRUE;
            pQueue->waitingTask = NULL;
        }

        OSA_InterruptDisable();
        (void)osObjectFree(&osMsgQInfo, msgQId);
        OSA_InterruptEnable();
    }

    return status;
#else
  msgQId=msgQId;
  return osaStatus_Error;
#endif
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
#if defined ( __IAR_SYSTEMS_ICC__ )
    _Pragma ("diag_suppress = Pm138")
#endif
     union
     {
        handlerFucPtr_t funHandlePtr;
        uint32_t irqHandler;
     }osaIrqHandler;

    osaIrqHandler.funHandlePtr = handler;
    (void)InstallIRQHandler((IRQn_Type)IRQNumber, osaIrqHandler.irqHandler);
#if defined ( __IAR_SYSTEMS_ICC__ )
    _Pragma ("diag_remark = PM138")
#endif
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#if (osCustomStartup == 0)
static OSA_TASK_DEFINE(main_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, FALSE);

void main (void)
{
    (void)OSA_Init();
    /* Initialize MCU clock */
    hardware_init();
    OSA_TimeInit();
    (void)OSA_TaskCreate(OSA_TASK(main_task),NULL);
    OSA_Start();
}
#endif

/*! *********************************************************************************
* \brief     Allocates a osObjectStruct_t block in the osObjectHeap array.
* \param[in] pointer to the object info struct.
* Object can be semaphore, mutex, osTimer, message Queue, event
* \return Pointer to the allocated osObjectStruct_t, NULL if failed.
*
* \pre
*
* \post
*
* \remarks Function is unprotected from interrupts.
*
********************************************************************************** */
#if osObjectAlloc_c
static void* osObjectAlloc(const osObjectInfo_t* pOsObjectInfo)
{
  uint32_t i;
  void * value = NULL;
  uint8_t* pObj = (uint8_t*)pOsObjectInfo->pHeap;
  for( i=0 ; i < pOsObjectInfo->objNo ; i++)
  {
      if(((osObjStruct_t*)(uint32_t)pObj)->inUse == 0U)
      {
          ((osObjStruct_t*)(uint32_t)pObj)->inUse = 1;
          value = (void*)pObj;
          break;
      }

      pObj += pOsObjectInfo->objectStructSize;
  }
  return value;
}
#endif
/*! *********************************************************************************
* \brief     Verifies the object is valid and allocated in the osObjectHeap array.
* \param[in] the pointer to the object info struct.
* \param[in] the pointer to the object struct.
* Object can be semaphore, mutex, osTimer, message Queue, event
* \return TRUE if the object is valid and allocated, FALSE otherwise
*
* \pre
*
* \post
*
* \remarks Function is unprotected from interrupts.
*
********************************************************************************** */
#if osObjectAlloc_c
static bool_t osObjectIsAllocated(const osObjectInfo_t* pOsObjectInfo, void* pObjectStruct)
{
    uint32_t i;
    bool_t status = FALSE;
    uint8_t* pObj = (uint8_t*)pOsObjectInfo->pHeap;
    for( i=0 ; i < pOsObjectInfo->objNo ; i++)
    {
        if(pObj == pObjectStruct)
        {
            if(((osObjStruct_t*)(uint32_t)pObj)->inUse != 0U)
            {
                status = TRUE;
            }
            break;
        }
        pObj += pOsObjectInfo->objectStructSize;
    }
    return status;
}
#endif

/*! *********************************************************************************
* \brief     Frees an osObjectStruct_t block from the osObjectHeap array.
* \param[in] pointer to the object info struct.
* \param[in] Pointer to the allocated osObjectStruct_t to free.
* Object can be semaphore, mutex, osTimer, message Queue, event
* \return none.
*
* \pre
*
* \post
*
* \remarks Function is unprotected from interrupts.
*
********************************************************************************** */
#if osObjectAlloc_c
static osaStatus_t osObjectFree(const osObjectInfo_t* pOsObjectInfo, void* pObjectStruct)
{
    uint32_t i;
    osaStatus_t status = osaStatus_Error;
    uint8_t* pObj = (uint8_t*)pOsObjectInfo->pHeap;
    for( i=0; i < pOsObjectInfo->objNo; i++ )
    {
        if(pObj == pObjectStruct)
        {
            if (((osObjStruct_t*)(uint32_t)pObj)->inUse != 0U)
            {
                ((osObjStruct_t*)(uint32_t)pObj)->inUse = 0U;
                status = osaStatus_Success;
            }
            break;
        }

        pObj += pOsObjectInfo->objectStructSize;
    }
    return status;
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
osaStatus_t OSA_Init(void)
{
#if (TASK_MAX_NUM > 0)
    task_init();
#endif

    return osaStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Start
 * Description   : This function is used to start RTOS scheduler.
 *
 *END**************************************************************************/
void OSA_Start(void)
{
#if (TASK_MAX_NUM > 0)
    g_curTask = p_taskListHead;

    for(;;)
    {
        if(g_curTask->haveToRun)
        {
            if(g_curTask->p_func != NULL)
            {
                g_curTask->p_func(g_curTask->param);
            }
            /* restart from the first task */
            g_curTask = p_taskListHead;
        }
        else
        {
            g_curTask = g_curTask->next;
        }
    }
#else
    for(;;)
    {
    }
#endif
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InsertTaskBefore
 * Description   : Helper function to insert Task TCB into the task list.
 *
 *END**************************************************************************/
static void OSA_InsertTaskBefore(task_handler_t newTCB, task_handler_t currentTCB)
{
    newTCB->next = currentTCB;
    newTCB->prev = currentTCB->prev;
    currentTCB->prev->next = newTCB;
    currentTCB->prev = newTCB;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : SysTick_Handler
 * Description   : This ISR of the SYSTICK timer.
 *
 *END**************************************************************************/
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
void SysTick_Handler(void)
{
    gTickCounter++;
}

#endif
