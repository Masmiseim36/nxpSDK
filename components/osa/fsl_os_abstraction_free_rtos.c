/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* ile
*
* This is the source file for the OS Abstraction layer for freertos. 
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#include "fsl_os_abstraction_free_rtos.h"
#include <string.h>
#include "generic_list.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define millisecToTicks(millisec) ((millisec * configTICK_RATE_HZ + 999)/1000)

#ifdef DEBUG_ASSERT
#define OS_ASSERT(condition) if(!(condition))while(1);
#else
#define OS_ASSERT(condition) (void)(condition);
#endif

/*! @brief Converts milliseconds to ticks*/
#define MSEC_TO_TICK(msec)  (((uint32_t)(msec)+500uL/(uint32_t)configTICK_RATE_HZ) \
                             *(uint32_t)configTICK_RATE_HZ/1000uL)
#define TICKS_TO_MSEC(tick) ((uint64_t)(tick)*1000uL/(uint32_t)configTICK_RATE_HZ)
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct osa_freertos_task
{
    list_element_t     link;
    osa_task_handle_t taskHandle;
} osa_freertos_task_t;

typedef struct _osa_event_struct
{
    EventGroupHandle_t handle;            /* The event handle */
    uint8_t autoClear;                   /*!< Auto clear or manual clear   */
} osa_event_struct_t;

/*! @brief State structure for bm osa manager. */
typedef struct _osa_state
{ 
    list_t taskList;                                     
    uint8_t mainTaskHandle[OSA_TASK_HANDLE_SIZE];
    uint32_t basePriorityArray[OSA_MAX_ISR_CRITICAL_SECTION_DEPTH];
    int32_t  basePriorityTop;
    uint32_t interruptDisableCount;
} osa_state_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
extern void main_task(void const *argument);
void startup_task(void* argument);

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
const uint8_t gUseRtos_c = USE_RTOS;  // USE_RTOS = 0 for BareMetal and 1 for OS

static osa_state_t s_osaState = {0};
/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*FUNCTION**********************************************************************
 *
 * Function Name : startup_task
 * Description   : Wrapper over main_task..
 *
 *END**************************************************************************/
void startup_task(void* argument)
{
    main_task(argument);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetCurrentHandle
 * Description   : This function is used to get current active task's handler.
 *
 *END**************************************************************************/
osa_task_handle_t OSA_TaskGetCurrentHandle(void)
{
    list_element_handle_t list_element;
    osa_freertos_task_t *ptask;
    
    list_element = LIST_GetHead(&s_osaState.taskList);
    while (list_element != NULL)
    {
        ptask = (osa_freertos_task_t*)list_element;
        if (ptask->taskHandle == xTaskGetCurrentTaskHandle())
        {
            return (osa_task_handle_t)ptask;
        }    
        list_element = LIST_GetNext(list_element);    
    }
    return NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskYield
 * Description   : When a task calls this function, it will give up CPU and put
 * itself to the tail of ready list.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskYield(void)
{
    taskYIELD();
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
    assert( taskHandle);
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    return (osa_task_priority_t)(PRIORITY_RTOS_TO_OSA(uxTaskPriorityGet(ptask->taskHandle)));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskSetPriority
 * Description   : This function sets task's priority by task handler.
 *
 *END**************************************************************************/
osa_status_t OSA_TaskSetPriority(osa_task_handle_t taskHandle, osa_task_priority_t taskPriority)
{
    assert( taskHandle);
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    vTaskPrioritySet((task_handler_t)ptask->taskHandle, PRIORITY_OSA_TO_RTOS(taskPriority));
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
osa_status_t OSA_TaskCreate(osa_task_handle_t taskHandle, osa_task_def_t *thread_def,osa_task_param_t task_param)
{
    assert( sizeof(osa_freertos_task_t) == OSA_TASK_HANDLE_SIZE );
    assert( taskHandle);
    TaskHandle_t pxCreatedTask;
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    if(taskHandle == NULL)
    {
        return KOSA_StatusError;
    }  
    if (xTaskCreate(
                    (TaskFunction_t)thread_def->pthread,  /* pointer to the task */
                    (char const*)thread_def->tname, /* task name for kernel awareness debugging */
                    thread_def->stacksize/sizeof(portSTACK_TYPE), /* task stack size */
                    (task_param_t)task_param, /* optional task startup argument */
                    PRIORITY_OSA_TO_RTOS(thread_def->tpriority),  /* initial priority */
                    &pxCreatedTask /* optional task handle to create */
                      ) == pdPASS)
    {
        ptask->taskHandle  = pxCreatedTask;
        OSA_InterruptDisable();
        LIST_AddTail(&s_osaState.taskList,(list_element_handle_t)&(ptask->link));
        OSA_InterruptEnable();
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
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
    assert( taskHandle);
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    osa_status_t status;
    uint16_t oldPriority;
    /*Change priority to avoid context switches*/
    oldPriority = OSA_TaskGetPriority(OSA_TaskGetCurrentHandle());
    (void)OSA_TaskSetPriority(OSA_TaskGetCurrentHandle(), OSA_PRIORITY_REAL_TIME);
#if INCLUDE_vTaskDelete /* vTaskDelete() enabled */
    vTaskDelete((task_handler_t)ptask->taskHandle);
    status = KOSA_StatusSuccess;
#else
    status = KOSA_StatusError; /* vTaskDelete() not available */
#endif
    (void)OSA_TaskSetPriority(OSA_TaskGetCurrentHandle(), oldPriority);
    OSA_InterruptDisable();
    LIST_RemoveElement(taskHandle);
    OSA_InterruptEnable();
  return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeDelay
 * Description   : This function is used to suspend the active thread for the given number of milliseconds.
 *
 *END**************************************************************************/
void OSA_TimeDelay(uint32_t millisec)
{
    vTaskDelay(millisecToTicks(millisec));
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeGetMsec
 * Description   : This function gets current time in milliseconds.
 *
 *END**************************************************************************/
uint32_t OSA_TimeGetMsec(void)
{
    TickType_t ticks;

    if (__get_IPSR())
    {
        ticks = xTaskGetTickCountFromISR();
    }
    else
    {
        ticks = xTaskGetTickCount();
    }

    return TICKS_TO_MSEC(ticks);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreCreate
 * Description   : This function is used to create a semaphore. 
 * Return         : Semaphore handle of the new semaphore, or NULL if failed. 
  *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreCreate(osa_semaphore_handle_t semaphoreHandle,uint32_t initValue)
{
    assert( sizeof(osa_semaphore_handle_t) == OSA_SEM_HANDLE_SIZE );
    assert( semaphoreHandle);
    if(semaphoreHandle == NULL)
    {
        return KOSA_StatusError;
    }

    QueueHandle_t sem;
    sem = xSemaphoreCreateCounting(0xFF, initValue);
    if (sem != NULL)
    {
        *(uint32_t*)semaphoreHandle  = (uint32_t) sem;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
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
    QueueHandle_t sem = (QueueHandle_t)*(uint32_t*)semaphoreHandle; 
    if(sem == NULL)
    {
        return KOSA_StatusError;
    }   
    vSemaphoreDelete(sem);
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
    uint32_t timeoutTicks;
    QueueHandle_t sem = (QueueHandle_t)*(uint32_t*)semaphoreHandle; 
    if(sem == NULL)
    {
        return KOSA_StatusError;
    } 

    /* Convert timeout from millisecond to tick. */
    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }

    if (xSemaphoreTake(sem, timeoutTicks) == pdFALSE)
    {
        return KOSA_StatusTimeout; /* timeout */
    }
    else
    {
        return KOSA_StatusSuccess; /* semaphore taken */
    }
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
    osa_status_t status = KOSA_StatusError;
    QueueHandle_t sem = (QueueHandle_t)*(uint32_t*)semaphoreHandle; 
    if(sem == NULL)
    {
        return KOSA_StatusError;
    } 
    if(semaphoreHandle)
    {
        if (__get_IPSR())
        {
            portBASE_TYPE taskToWake = pdFALSE;

            if (pdTRUE==xSemaphoreGiveFromISR(sem, &taskToWake))
            {
                if (pdTRUE == taskToWake)
                {
                    portYIELD_FROM_ISR(taskToWake);
                }
                status = KOSA_StatusSuccess;
            }
            else
            {
                status = KOSA_StatusError;
            }
        }
        else
        {
            if (pdTRUE == xSemaphoreGive(sem))
            {
                status = KOSA_StatusSuccess; /* sync object given */
            }
            else
            {
                status = KOSA_StatusError;
            }
        }    
    }
    return status;
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
    assert( sizeof(osa_mutex_handle_t) == OSA_MUTEX_HANDLE_SIZE );
    assert( mutexHandle);
    if(mutexHandle == NULL)
    {
        return KOSA_StatusError;
    } 
    QueueHandle_t mutex;
    mutex = xSemaphoreCreateMutex();
    if (mutex != NULL)
    {
        *(uint32_t*)mutexHandle  = (uint32_t) mutex;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;    
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexLock
 * Description   : This function checks the mutex's status, if it is unlocked,
 * lock it and returns KOSA_StatusSuccess, otherwise, wait for the mutex.
 * This function returns KOSA_StatusSuccess if the mutex is obtained, returns
 * KOSA_StatusError if any errors occur during waiting. If the mutex has been
 * locked, pass 0 as timeout will return KOSA_StatusTimeout immediately.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexLock(osa_mutex_handle_t mutexHandle, uint32_t millisec)
{
    assert( mutexHandle);
    QueueHandle_t mutex= (QueueHandle_t)*(uint32_t*)mutexHandle; 
    if(mutexHandle == NULL)
    {
        return KOSA_StatusError;
    } 
    uint32_t timeoutTicks;
    /* If pMutex has been locked by current task, return error. */
    if (xSemaphoreGetMutexHolder(mutex) == xTaskGetCurrentTaskHandle())
    {
        return KOSA_StatusError;
    }

    /* Convert timeout from millisecond to tick. */
    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }

    if (xSemaphoreTake(mutex, timeoutTicks)==pdFALSE)
    {
        return KOSA_StatusTimeout; /* timeout */
    }
    else
    {
        return KOSA_StatusSuccess; /* semaphore taken */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexUnlock
 * Description   : This function is used to unlock a mutex.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexUnlock(osa_mutex_handle_t mutexHandle)
{
    assert( mutexHandle);
    QueueHandle_t mutex= (QueueHandle_t)*(uint32_t*)mutexHandle; 
    if(mutexHandle == NULL)
    {
        return KOSA_StatusError;
    } 

    /* If pMutex is not locked by current task, return error. */
    if (xSemaphoreGetMutexHolder(mutex) != xTaskGetCurrentTaskHandle())
    {
        return KOSA_StatusError;
    }

    if (xSemaphoreGive(mutex)==pdPASS)
    {
        return KOSA_StatusSuccess;
    }
    else
    {
        return KOSA_StatusError;
    }
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
    assert( mutexHandle);
    QueueHandle_t mutex= (QueueHandle_t)*(uint32_t*)mutexHandle; 
    if(mutexHandle == NULL)
    {
        return KOSA_StatusError;
    } 
    vSemaphoreDelete(mutex);
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
    assert( eventHandle);
    osa_event_struct_t *pEventStruct= (osa_event_struct_t*)eventHandle; 
    if(eventHandle == NULL)
    {
        return KOSA_StatusError;
    } 
   
    pEventStruct->handle = xEventGroupCreate();
    if (pEventStruct->handle)
    {
        pEventStruct->autoClear = autoClear;
    }
    else
    {
        return KOSA_StatusError;
    }
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
    portBASE_TYPE taskToWake = pdFALSE;
    assert( eventHandle);
    osa_event_struct_t *pEventStruct= (osa_event_struct_t*)eventHandle; 
    if(eventHandle == NULL)
    {
        return KOSA_StatusError;
    } 
    if(pEventStruct->handle == NULL)
    {
        return KOSA_StatusError;
    }
    if (__get_IPSR())
    {
        if (pdPASS != xEventGroupSetBitsFromISR(pEventStruct->handle, (event_flags_t)flagsToSet, &taskToWake))
        {
            return KOSA_StatusError;
        }
        if (pdTRUE == taskToWake)
        {
            portYIELD_FROM_ISR(taskToWake);
        }
    }
    else
    {
        xEventGroupSetBits(pEventStruct->handle, (event_flags_t)flagsToSet);
    }
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
    assert( eventHandle);
    osa_event_struct_t *pEventStruct= (osa_event_struct_t*)eventHandle; 
    if(eventHandle == NULL)
    {
        return KOSA_StatusError;
    }    

    if(pEventStruct->handle == NULL)
    {
        return KOSA_StatusError;
    } 

    if (__get_IPSR())
    {
        xEventGroupClearBitsFromISR(pEventStruct->handle, (event_flags_t)flagsToClear);
    }
    else
    {
        xEventGroupClearBits(pEventStruct->handle, (event_flags_t)flagsToClear);
    }  
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
osa_status_t OSA_EventWait(osa_event_handle_t eventHandle, osa_event_flags_t flagsToWait, uint8_t waitAll, uint32_t millisec, osa_event_flags_t *pSetFlags)
{
    assert( eventHandle);
    BaseType_t clearMode;
    uint32_t timeoutTicks;
    event_flags_t flagsSave;
    osa_event_struct_t *pEventStruct= (osa_event_struct_t*)eventHandle; 
    if(eventHandle == NULL)
    {
        return KOSA_StatusError;
    }   

    /* Clean FreeRTOS cotrol flags */
    flagsToWait = flagsToWait & 0x00FFFFFF;
    if(pEventStruct->handle == NULL)
    {
        return KOSA_StatusError;
    }
  
    /* Convert timeout from millisecond to tick. */
    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = millisec/portTICK_PERIOD_MS;
    }
    
    clearMode = (pEventStruct->autoClear) ? pdTRUE: pdFALSE;
    
    flagsSave = xEventGroupWaitBits(pEventStruct->handle,(event_flags_t)flagsToWait,clearMode,(BaseType_t)waitAll,timeoutTicks);
    
    flagsSave &= (event_flags_t)flagsToWait;
    if(pSetFlags)
    {
        *pSetFlags = (osa_event_flags_t)flagsSave;
    }
    
    if (flagsSave)
    {
        return KOSA_StatusSuccess;
    }
    else
    {
        return KOSA_StatusTimeout;
    }
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
    assert( eventHandle);
    osa_event_struct_t *pEventStruct= (osa_event_struct_t*)eventHandle; 
    if(eventHandle == NULL)
    {
        return KOSA_StatusError;
    }  

    if(pEventStruct->handle == NULL)
    {
        return KOSA_StatusError;
    }
    vEventGroupDelete(pEventStruct->handle);
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
    assert(sizeof(osa_msgq_handle_t) == OSA_MSGQ_HANDLE_SIZE);
    assert(msgqHandle);
    if(msgqHandle == NULL)
    {
        return KOSA_StatusError;
    }

    QueueHandle_t msg_queue_handler; 

    /* Create the message queue where each element is a pointer to the message item. */
    msg_queue_handler = xQueueCreate(msgNo, sizeof(uint32_t*));
    if (msg_queue_handler != NULL)
    {
        *(uint32_t*)msgqHandle  = (uint32_t)msg_queue_handler;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQPut
 * Description   : This function is used to put a message to a message queue.
* Return         : KOSA_StatusSuccess if the message is put successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQPut(osa_msgq_handle_t msgqHandle, void* pMessage)
{
    osa_status_t osaStatus;
    assert( msgqHandle);
    portBASE_TYPE taskToWake = pdFALSE;
    QueueHandle_t handler= (QueueHandle_t)*(uint32_t*)msgqHandle; 
    if(handler == NULL)
    {
        return KOSA_StatusError;
    } 

    if (__get_IPSR())
    {    
        if (pdTRUE == xQueueSendToBackFromISR(handler, pMessage, &taskToWake))
        {
            if (pdTRUE == taskToWake)
            {
                portYIELD_FROM_ISR(taskToWake);
            }
            osaStatus = KOSA_StatusSuccess;
        }
        else
        {
            osaStatus =  KOSA_StatusError;
        }    
    }
    else
    {
        osaStatus = (xQueueSendToBack(handler, pMessage, 0)== pdPASS)?(KOSA_StatusSuccess):(KOSA_StatusError);
    }

   return osaStatus;
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
osa_status_t OSA_MsgQGet(osa_msgq_handle_t msgqHandle, void *pMessage, uint32_t millisec)
{
    osa_status_t osaStatus;
    assert( msgqHandle);
    QueueHandle_t handler= (QueueHandle_t)*(uint32_t*)msgqHandle; 
    if(handler == NULL)
    {
        return KOSA_StatusError;
    } 

    uint32_t timeoutTicks;

    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }
    if (xQueueReceive(handler, pMessage, timeoutTicks)!=pdPASS)
    {
        osaStatus =  KOSA_StatusTimeout; /* not able to send it to the queue? */
    }
    else
    {
        osaStatus = KOSA_StatusSuccess;
    }
    return osaStatus;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQDestroy
 * Description   : This function is used to destroy the message queue.
 * Return        : KOSA_StatusSuccess if the message queue is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQDestroy(osa_msgq_handle_t msgqHandle)
{
    assert( msgqHandle);
    QueueHandle_t handler= (QueueHandle_t)*(uint32_t*)msgqHandle; 
    if(handler == NULL)
    {
        return KOSA_StatusError;
    } 

    vQueueDelete(handler);
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InterruptEnable
 * Description   : self explanatory.
 *
 *END**************************************************************************/
void OSA_InterruptEnable(void)
{
    if (__get_IPSR())
    {
        if(s_osaState.basePriorityTop)
        {
            s_osaState.basePriorityTop--;
            portCLEAR_INTERRUPT_MASK_FROM_ISR(s_osaState.basePriorityArray[s_osaState.basePriorityTop]);
        }
    }
    else
    {
        portEXIT_CRITICAL();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InterruptDisable
 * Description   : self explanatory.
 *
 *END**************************************************************************/
void OSA_InterruptDisable(void)
{
    if (__get_IPSR())
    {
        if(s_osaState.basePriorityTop < OSA_MAX_ISR_CRITICAL_SECTION_DEPTH)
        {
            s_osaState.basePriorityArray[s_osaState.basePriorityTop] = portSET_INTERRUPT_MASK_FROM_ISR();
            s_osaState.basePriorityTop++;
        }
    }
    else
    {
        portENTER_CRITICAL();
    }
  
}


/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EnableIRQGlobal
 * Description   : enable interrupts using PRIMASK register.
 *
 *END**************************************************************************/
void OSA_EnableIRQGlobal(void)
{
    if (s_osaState.interruptDisableCount > 0)
    {
        s_osaState.interruptDisableCount--;
        
        if (s_osaState.interruptDisableCount == 0)
        {
            __enable_irq();
        }
        /* call core API to enable the global interrupt*/
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_DisableIRQGlobal
 * Description   : disable interrupts using PRIMASK register.
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
 * Function Name : OSA_InstallIntHandler
 * Description   : This function is used to install interrupt handler.
 *
 *END**************************************************************************/
void OSA_InstallIntHandler(uint32_t IRQNumber, void (*handler)(void))
{

#if defined ( __IAR_SYSTEMS_ICC__ )
    _Pragma ("diag_suppress = Pm138")
#endif
#if defined(ENABLE_RAM_VECTOR_TABLE)
    InstallIRQHandler((IRQn_Type)IRQNumber, (uint32_t)handler);
#endif /* ENABLE_RAM_VECTOR_TABLE. */
#if defined ( __IAR_SYSTEMS_ICC__ )
    _Pragma ("diag_remark = PM138")
#endif
}

/*!*********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
OSA_TASK_DEFINE(startup_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, 0)  ;
int main (void)
{
    extern void BOARD_InitHardware(void);
    /* Initialize MCU clock */
    BOARD_InitHardware();
     LIST_Init((&s_osaState.taskList), 0);
    s_osaState.basePriorityTop = 0;
    s_osaState.interruptDisableCount = 0;
    OSA_TaskCreate((osa_task_handle_t)s_osaState.mainTaskHandle,OSA_TASK(startup_task),NULL);
    vTaskStartScheduler();
    return 0;
}
