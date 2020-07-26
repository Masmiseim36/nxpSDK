/*
 *         Copyright (c), NXP Semiconductors Bangalore / India
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** \file
 * Generic OSAL Component of Reader Library Framework.
 * Functions defined here are abstracting the OS Features.
 *  Created on: May 4, 2016
 *      Author: nxp79569
 *
 */

#ifndef PHOSAL_H
#define PHOSAL_H

#ifdef  __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef linux
#include <sys/types.h>
#endif

#if defined(__GNUC__ ) /* Toolchain with StdInt */
#    include <stdint.h>
#elif defined(__ICCARM__)
#   include "intrinsics.h"
#   include <stdint.h>
#elif defined(__CC_ARM)
#   include <stdint.h>
#elif defined(_WIN32)
#   include <stdint.h>
#endif
#include <string.h>

#if defined (PH_TYPEDEFS_H)
#else
#ifdef _WIN32
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef long int32_t;
#endif

#ifndef __float32_t_defined
typedef float float32_t;
#endif

/* NULL pointer */
#ifndef NULL
#	define NULL ((void *) 0)
#endif
#endif /* PH_TYPEDEFS_H */

#include "phOsal_Config.h"

/** \defgroup phOsal Operating System Abstraction Layer (OSAL)
 *
 * \brief This component implements the OSAL APIs.
 *
 * OSAL provides the interface for the different RTOS's and can be easily ported to a new RTOS.
 * It supports Thread Management, Semaphore, Mutex, Event and Timers.
 *
 * APIs Supported
 *
 * | Thread APIs
 * |-----------------------------------------
 * | \ref phOsal_ThreadCreate
 * | \ref phOsal_ThreadDelete
 * | \ref phOsal_ThreadChangePrio
 * | \ref phOsal_ThreadDelay
 * | \ref phOsal_ThreadExit
 * |-----------------------------------------
 * | Semaphore APIs
 * |-----------------------------------------
 * | \ref phOsal_SemCreate
 * | \ref phOsal_SemDelete
 * | \ref phOsal_SemPost
 * | \ref phOsal_SemPend
 * |-----------------------------------------
 * | Mutex APIs
 * |-----------------------------------------
 * | \ref phOsal_MutexCreate
 * | \ref phOsal_MutexDelete
 * | \ref phOsal_MutexUnLock
 * | \ref phOsal_MutexLock
 * |-----------------------------------------
 * | Event APIs
 * |-----------------------------------------
 * | \ref phOsal_EventCreate
 * | \ref phOsal_EventPend
 * | \ref phOsal_EventPost
 * | \ref phOsal_EventClear
 * | \ref phOsal_EventGet
 * | \ref phOsal_EventDelete
 * |-----------------------------------------
 * | Timer APIs
 * |-----------------------------------------
 * | \ref phOsal_TimerCreate
 * | \ref phOsal_TimerDelete
 * | \ref phOsal_TimerStart
 * | \ref phOsal_TimerStop
 * | \ref phOsal_TimerGetCurrent
 * | \ref phOsal_TimerModify
 *
 * Any application which wants to use the OSAL should include the file "phOsal.h" in the project.
 * User should enable the support of RTOS from the available options from header phOsal_Config.h.
 * If user wants to add the support of new RTOS they should implement/support mentioned OSAL APIs.
 *
 * @{
 */

/**
 * @{
 */

#define PH_OSAL_SUCCESS             0x0000U /**< Returned in case of no error. */
#define PH_OSAL_IO_TIMEOUT          0x0001U /**< Returned in case of timeOut*/
#define PH_OSAL_ERROR               0x0027U /**< Returned in case of Invalid Parameter,buffer overflow  or the configuration error. */
#define PH_OSAL_FAILURE             0x0028U /**< Returned in case OSAL failed to perform the requested operation e.g. due to insufficient memory. */
#define PH_OSAL_UNSUPPORTED_COMMAND 0x0024U /**< Returned in case Command is not supported. */
/* @} */

/**
* Macro for error handling in the OSAL layer and exit.
* @{
*/
#define PH_OSAL_ADD_COMPCODE(stat, code)     ((phStatus_t)(((stat) == PH_OSAL_SUCCESS) || \
    ((stat) & PH_COMP_MASK) ? (stat) : (((stat) & PH_ERR_MASK) | ((code) & PH_COMP_MASK))))
#define PH_OSAL_CHECK_SUCCESS(status)         {if ((status) != PH_OSAL_SUCCESS) {return (status);}}
/* @} */


#if defined (PH_STATUS_H)
#else
/**
* Osal Default Return Type
*/
typedef uint16_t phStatus_t;

/**
 * @{
 */

#define PH_COMP_OSAL                0xF000U /**< OS AL component code. */
#define PH_COMP_MASK                0xFF00U /**< Component Mask for status code and component ID. */
#define PH_COMPID_MASK              0x00FFU /**< ID Mask for component ID. */
#define PH_ERR_MASK                 0x00FFU /**< Error Mask for status code. */
/* @} */

#endif


/**
 * Osal Thread Handle.
 * @{
 */

typedef void* phOsal_Thread_t;
/* @} */

/**
 * Osal Thread Object.
 * @{
 */
typedef struct /* phOsal_ThreadObj */
{
    phOsal_Thread_t ThreadHandle;
    uint8_t *pTaskName;
    uint8_t priority;
    uint32_t stackSizeInNum;
    uint32_t *pStackBuffer;
    void *ppriv;
    uint32_t dwThreadIndex;
} phOsal_ThreadObj_t, *pphOsal_ThreadObj_t;
/* @} */

/**
 * Amount of Time : specified in Ticks
 */
typedef uint32_t phOsal_Ticks_t;

typedef void (*pphOsal_StartFunc_t)(void* pCtx);

/**
 * Osal Event Handle.
 * @{
 */
typedef void* phOsal_Event_t;
/* @} */

/**
 * Osal Event Object.
 * @{
 */
typedef struct /* phOsal_EventObj */
{
    phOsal_Event_t EventHandle;
    uint8_t *pEvtName;
    uint32_t intialValue;
    uint32_t dwEventIndex;
} phOsal_EventObj_t, *pphOsal_EventObj_t;
/* @} */

/**
 * OS Event Bits
 * @{
 */
typedef uint32_t phOsal_EventBits_t;
/* @} */

/**
 * OS Event  Options Enum
 * @{
 */
typedef enum
{
    E_OS_EVENT_OPT_NONE               = 0x00,
    E_OS_EVENT_OPT_PEND_SET_ANY       = 0x01,
    E_OS_EVENT_OPT_PEND_SET_ALL       = 0x02,
    E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT = 0x04,
    E_OS_EVENT_OPT_POST_ISR           = 0x08,
    E_OS_EVENT_OPT_CLR_ISR            = 0x10
} phOsal_EventOpt_t;
/* @} */

/**
 * OS Event  Options Position Enum
 * @{
 */
typedef enum
{
    E_OS_EVENT_OPT_POS_PEND_SET_ANY,
    E_OS_EVENT_OPT_POS_PEND_SET_ALL,
    E_OS_EVENT_OPT_POS_PEND_CLEAR_ON_EXIT,
    E_OS_EVENT_OPT_POS_POST_ISR,
    E_OS_EVENT_OPT_POS_CLR_ISR
} phOsal_EventOptPos_t;
/* @} */

/**
 * Osal Timer Handle.
 * @{
 */
typedef void* phOsal_Timer_t;
/* @} */

/**
 * OS Timer Options Enum
 * @{
 */
typedef enum
{
    OS_TIMER_OPT_SINGLE_SHOT        = 0x00,
    OS_TIMER_OPT_PERIODIC           = 0x01,
    E_OS_TIMER_OPT_MAX,
    E_PHOSAL_TIMER_OPT_SIZEADJUST = 0x7FFFFFFFU
} phOsal_TimerOpt_t;
/* @} */

/**
 * OS Timer Callback
 * @{
 */
typedef void (*pphOsal_TimerCallback_t )(void* pCtx) ;
/* @}*/

/**
 * OS Timer Unit Enum
 * @{
 */
typedef enum
{
    OS_TIMER_UNIT_MSEC              = 0x00,
    OS_TIMER_UNIT_SEC               = 0x01,
    E_OS_TIMER_UNIT_MAX,
    E_PHOSAL_TIMER_UNIT_SIZEADJUST = 0x7FFFFFFFU
} phOsal_TimerUnit_t;
/* @}*/

/**
 * Timer Period Object
 * @{
 */
typedef struct /* phOsal_TimePeriodObj */
{
    phOsal_TimerUnit_t unitPeriod;
    uint32_t period;

} phOsal_TimerPeriodObj_t;
/* @} */

/**
 * Osal Timer Object.
 * @{
 */
typedef struct /* phOsal_TimerObj */
{
    phOsal_Timer_t TimerHandle;
    uint8_t *pTimerName;
    phOsal_TimerOpt_t opt;
    phOsal_TimerPeriodObj_t timePeriod;
    pphOsal_TimerCallback_t  timerCb;
    void    *arg;
    uint32_t dwTimerIndex;
} phOsal_TimerObj_t, *pphOsal_TimerObj_t;
/* @} */

/**
 * OS Semaphore Handle.
 * @{
 */
typedef void* phOsal_Semaphore_t;
/* @} */

/**
 * OS Semaphore Object.
 * @{
 */
typedef struct /* phOsal_SemObj */
{
    phOsal_Semaphore_t SemHandle;
    uint8_t *pSemName;
    uint32_t  semInitialCount;
    uint32_t  semMaxCount;
    uint32_t dwSemIndex;
} phOsal_SemObj_t, *pphOsal_SemObj_t;
/* @} */

/**
 * OS Event  Options Enum
 * @{
 */
typedef enum
{
    E_OS_SEM_OPT_NONE                   = 0x0,
    E_OS_SEM_OPT_SIGNALLING_SEM         = 0x01,
    E_OS_SEM_OPT_BINARY_SEM             = 0x02,
    E_OS_SEM_OPT_COUNTING_SEM           = 0x04,
    E_OS_SEM_OPT_POST_FROM_ISR          = 0x08,
    E_OS_SEM_OPT_MAX,
    E_OS_SEM_OPT_SIZEADJUST = 0x7FFFFFFFU
} phOsal_SemOpt_t;
/* @} */

/**
 * Mutex Handle
 * @{
 */
typedef void * phOsal_Mutex_t;
/* @} */

/**
 * Mutex Object
 * @{
 */
typedef struct /* phOsal_MutexObj */
{
    phOsal_Mutex_t MutexHandle;
    uint8_t *pMutexName;
    uint32_t dwMutexIndex;
} phOsal_MutexObj_t, *pphOsal_MutexObj_t;
/* @} */

/**
 * \brief This function initializes the osal attributes with the default values.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS              Operation successful.
 */
extern phStatus_t phOsal_Init(void);

/**
 * \brief This function starts a new thread in the calling process. The new thread starts execution
 * by invoking phOsal_StartScheduler().
 *
 * Before returning, a successful call to phOsal_ThreadCreate() stores the ID of the new thread in the buffer pointed
 * to by thread; this identifier is used to refer to the thread in subsequent calls to other functions.
 *
 * Each task requires RAM that is used to hold the task state, and used by the task as its stack.
 * If stackSizeInNum(threadObj.stackSizeInNum) is NULL, then the Task stack will be allocated dynamically
 * otherwise it will allocated the static stack and this static stack should be allocated and provided by the user in
 * threadObj(threadObj.pStackBuffer) structure.
 *
 * Instead of using any proprietary function use phOsal_ThreadCreate() to create the task/thread.
 *
 * @param[out]  threadHandle    Thread Handle, stores the ID of the new thread
 * @param[in]   threadObj       Thread Object whose contents are used at thread creation time
 * @param[in]   startFunc       The new thread starts execution by invoking Start function (startFunc).
 * @param[in]   arg             The argument passed for the Start function (startFunc)
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS         Operation successful.
 * @retval #PH_OSAL_ERROR           Parameter passed is invalid.
 * @retval #PH_OSAL_FAILURE         Failure not available incase of Static allocation.
 */
extern phStatus_t phOsal_ThreadCreate(
    phOsal_Thread_t *threadHandle,
    pphOsal_ThreadObj_t threadObj,
    pphOsal_StartFunc_t startFunc,
    void *arg);

/**
 * \brief This function will delete the thread.
 *
 * Instead of using any proprietary function use phOsal_ThreadDelete() to delete the task/thread.
 *
 * @param[in]  threadHandle    Handle to the therad.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_ThreadDelete(
    phOsal_Thread_t * threadHandle);

/**
 * \brief This function changes the priority of the thread.
 * \ref phOsal_ThreadCreate should be called before using this Api.
 *
 * @param[in]   threadHandle    Thread Handle.
 * @param[in]   newPrio         New Priority of the thread.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_ThreadChangePrio(
    phOsal_Thread_t * threadHandle,
    uint32_t newPrio);

/**
 * \brief This function exits the thread itself.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS              Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND  Command not supported.
 */
extern phStatus_t phOsal_ThreadExit(void);

/**
 * \brief This function delays the calling thread.
 *
 * Earlier phOsal_TaskDelay() functions was provided to delay the thread.
 * Function phOsal_TaskDelay() is changed to phOsal_ThreadDelay() function to delay the thread.
 *
 * @param[in]   ticksToSleep    Time to delay(in Ticks)
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS              Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND  Command not supported.
 */
extern phStatus_t phOsal_ThreadDelay(
    phOsal_Ticks_t ticksToSleep);

/**
 * \brief This function will create the event group and returns the newly created event group.
 *
 * Each event requires RAM that is used to hold the event group state.
 *
 * Earlier phOsal_Event_Init() function was provided which was creating the event group.
 * Functions phOsal_Event_Init() and phOsal_Event_GetEventHandle() are depreciated.
 * Now the user has to use phOsal_EventCreate() function to create the event group.
 *
 * @param[out]  eventHandle     Event handle
 * @param[in]   eventObj        Event object used to create the event.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_FAILURE             Resource not available incase of Static allocation.
 */
extern phStatus_t phOsal_EventCreate(
    phOsal_Event_t *eventHandle,
    pphOsal_EventObj_t eventObj);

/**
 * \brief This function will wait for the events.
 *
 * @param[in]   eventHandle     Event handle
 * @param[in]   options         Event options \ref phOsal_EventOpt_t.
 * @param[in]   ticksToWait     Time to wait(in Ticks)
 * @param[in]   FlagsToWait     Events to wait.
 * @param[out]  pCurrFlags      Events at the time either the event bits being waited for became set or the block time expired
 *
 * Earlier phOsal_Event_WaitAny()/phOsal_Event_WaitAll() functions was provided to wait for the events.
 * Functions phOsal_Event_WaitAny()/phOsal_Event_WaitAll() are depreciated.
 * Now the user has to use phOsal_EventPend() function to wait/pend on events.
 *
 * To achieve the functionality of phOsal_Event_WaitAny() : options value has to be E_OS_EVENT_OPT_PEND_SET_ANY
 * To achieve the functionality of phOsal_Event_WaitAll() : options value has to be E_OS_EVENT_OPT_PEND_SET_ALL
 * To clear set event which are passed as FlagsToWait on exit : options value has to be E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT
 *
 * To achieive multiple functionality options can be ORed and provided.
 * Example : To wait for any event generated and clear the generated events on exit,
 * then the options value has be : (E_OS_EVENT_OPT_PEND_SET_ANY | E_OS_EVENT_OPT_PEND_CLEAR_ON_EXIT)
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_IO_TIMEOUT          Desired events didn't occur during the wait time(ticksToWait).
 */
extern phStatus_t phOsal_EventPend(
    volatile phOsal_Event_t * eventHandle,
    phOsal_EventOpt_t options,
    phOsal_Ticks_t ticksToWait,
    phOsal_EventBits_t FlagsToWait,
    phOsal_EventBits_t *pCurrFlags);

/**
 * \brief This function will post the events.
 *
 * Earlier phOsal_Event_Post() function was taking three arguments.
 * which are (phOsal_EventType_t eEvtType, phOsal_EventSource_t eSrc, phOsal_EventDestination_t eDest)
 * Functions phOsal_Event_PostwithDetail() and phOsal_Event_SetDetail() are depreciated.
 * And Function phOsal_Event_Post() is changed t phOsal_EventPost  which will accept four arguments.
 * Now the user has to use phOsal_EventPost() function to post the events.
 *
 * @param[in]   eventHandle     Event handle
 * @param[in]   options         Event options \ref phOsal_EventOpt_t.
 * @param[in]   FlagsToPost     Events to post.
 * @param[out]  pCurrFlags      Events at the time either the event bits being posted.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_EventPost(
    phOsal_Event_t * eventHandle,
    phOsal_EventOpt_t options,
    phOsal_EventBits_t FlagsToPost,
    phOsal_EventBits_t *pCurrFlags);

/**
 * \brief This function will clear the events.
 *
 * Earlier phOsal_Event_Consume() functions was provided to clear the events.
 * Function phOsal_Event_Consume() is changed t phOsal_EventClear() which will accept four arguments.
 * Now the user has to use phOsal_EventClear() function to clear the events.
 *
 * @param[in]   eventHandle     Event handle
 * @param[in]   options         Event options \ref phOsal_EventOpt_t.
 * @param[in]   FlagsToClear    Events to clear.
 * @param[out]  pCurrFlags      Events at the time either the event bits being cleared.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_EventClear(
    phOsal_Event_t * eventHandle,
    phOsal_EventOpt_t options,
    phOsal_EventBits_t FlagsToClear,
    phOsal_EventBits_t *pCurrFlags);

/**
 * \brief This function will get the events which are already there.
 *
 * Earlier phOsal_Event_GetDetail() functions was provided to get the event details.
 * Function phOsal_Event_GetDetail() is depreciated.
 * Now the user has to use phOsal_EventGet() function to get the events.
 *
 * @param[in]   eventHandle     Event handle
 * @param[out]  pCurrFlags      Events at the time either the event bits being cleared.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_EventGet(
    phOsal_Event_t * eventHandle,
    phOsal_EventBits_t *pCurrFlags);

/**
 * \brief This function will delete the events.
 *
 * Earlier phOsal_Event_Deinit() functions was provided to delete the event group.
 * Function phOsal_Event_Deinit() is depreciated.
 * Now the user has to use phOsal_EventDelete() function to delete the event group.
 *
 * @param[in]   eventHandle     Event handle.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_EventDelete(
    phOsal_Event_t * eventHandle);

/**
 * \brief This function will create the Semaphore and return the handle.
 *
 * Earlier phOsal_Semaphore_Create() functions was provided to create the semaphore.
 * Function phOsal_Semaphore_Create() is depreciated.
 * Now the user has to use phOsal_SemCreate() function to create the semaphore.
 *
 * @param[out]  semHandle   Newly created Semaphore Handle.
 * @param[in]   semObj      Semaphore Object. This will be used to create the semaphore.
 * @param[in]   opt         Option for Semaphore creation \ref phOsal_SemOpt_t
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_FAILURE             Resource not available incase of Static allocation.
 */
extern phStatus_t phOsal_SemCreate(
    phOsal_Semaphore_t *semHandle,
    pphOsal_SemObj_t semObj,
    phOsal_SemOpt_t opt);

/**
 * \brief This function will wait for the semaphore.
 * The semaphore must be created with a call to phOsal_SemCreate().
 *
 * Earlier phOsal_Semaphore_Take() functions was provided to take the semaphore.
 * Function phOsal_Semaphore_Take() is depreciated.
 * Now the user has to use phOsal_SemPend() function to pend/take the semaphore.
 *
 * @param[in]   semHandle           Semaphore handle
 * @param[in]   timePeriodToWait    Time to wait \ref phOsal_TimerPeriodObj_t.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_IO_TIMEOUT          Time expired without semaphore becoming available.
 */
extern phStatus_t phOsal_SemPend(
    phOsal_Semaphore_t * semHandle,
    phOsal_TimerPeriodObj_t timePeriodToWait);

/**
 * \brief This function will release the semaphore.
 *
 * Earlier phOsal_Semaphore_Give() functions was provided to give the semaphore.
 * Function phOsal_Semaphore_Give() is depreciated.
 * Now the user has to use phOsal_SemPost() function to post/give the semaphore.
 *
 * @param[in]   semHandle           Semaphore handle
 * @param[in]   opt                 Semaphore options \ref phOsal_SemOpt_t.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_SemPost(
    phOsal_Semaphore_t * semHandle,
    phOsal_SemOpt_t opt);

/**
 * \brief This function will delete the semaphore.
 *
 * Earlier phOsal_Semaphore_Delete() functions was provided to delete the semaphore.
 * Function phOsal_Semaphore_Delete() is changed to phOsal_SemDelete() function to delete the semaphore.
 *
 * @param[in]   semHandle       Semaphore handle.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_SemDelete(
    phOsal_Semaphore_t * semHandle);

/**
 * \brief This function will create the Mutex and return the handle.
 *
 * Earlier phOsal_Mutex_Create() functions was provided to create the mutex.
 * Function phOsal_Mutex_Create() is changed to phOsal_MutexCreate() function to create the mutex.
 *
 * @param[out]  mutexHandle         Newly created Mutex Handle.
 * @param[in]   mutexObj            Mutex Object. This will be used to create the mutex.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_FAILURE             Resource not available incase of Static allocation.
 */
extern phStatus_t phOsal_MutexCreate(
    phOsal_Mutex_t * mutexHandle,
    pphOsal_MutexObj_t mutexObj);

/**
 * \brief This function will lock the mutex.
 * The semaphore must be created with a call to phOsal_SemCreate().
 *
 * Earlier phOsal_Mutex_Take() functions was provided to take the semaphore.
 * Function phOsal_Mutex_Take() is depreciated.
 * Now the user has to use phOsal_MutexLock() function to Lock/take the semaphore.
 *
 * @param[in]   mutexHandle         Semaphore handle
 * @param[in]   timePeriodToWait    Time to wait for mutex lock \ref phOsal_TimerPeriodObj_t.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_IO_TIMEOUT          Time expired without mutex getting locked.
 */
extern phStatus_t phOsal_MutexLock(
    phOsal_Mutex_t * mutexHandle,
    phOsal_TimerPeriodObj_t timePeriodToWait);

/**
 * \brief This function will unlock the semaphore.
 *
 * Earlier phOsal_Mutex_Give() functions was provided to give the semaphore.
 * Function phOsal_Mutex_Give() is depreciated.
 * Now the user has to use phOsal_MutexUnLock() function to UnLock/give the semaphore.
 *
 * @param[in]   mutexHandle           Mutex handle
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 */
extern phStatus_t phOsal_MutexUnLock(
    phOsal_Mutex_t * mutexHandle);

/**
 * \brief This function will delete the semaphore.
 *
 * Earlier phOsal_Mutex_Delete() functions was provided to delete the mutex.
 * Function phOsal_Mutex_Delete() is changed to phOsal_MutexDelete() function to delete the mutex.
 *
 * @param[in]   mutexHandle       Mutex handle.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_UNSUPPORTED_COMMAND Command not supported.
 */
extern phStatus_t phOsal_MutexDelete(
    phOsal_Mutex_t * mutexHandle);

/**
 * \brief This function will create the timer and return the handle.
 * This function will also configure the timer.
 *
 * Earlier phOsal_Timer_Create() functions was provided to create the timer.
 * Function phOsal_Timer_Create() is changed to phOsal_TimerCreate() function to create the timer.
 *
 * @param[out]  timerHandle         Newly created Timer Handle.
 * @param[in]   timerObj            Timer Object. This will be used to create the timer.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 * @retval #PH_OSAL_FAILURE             Resource not available incase of Static allocation.
 */
extern phStatus_t phOsal_TimerCreate(
    phOsal_Timer_t * timerHandle,
    pphOsal_TimerObj_t timerObj);

/**
 * \brief This function will start the timer.
 *
 * Earlier phOsal_Timer_Start() functions was provided to start the timer.
 * Function phOsal_Timer_Start() is changed to phOsal_TimerStart() function to start the timer.
 * And the function phOsal_Timer_Reset() is depreciated.
 *
 * @param[in]  timerHandle         Newly created Timer Handle.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_TimerStart(
    phOsal_Timer_t * timerHandle);

/**
 * \brief This function will stop the timer.
 *
 * Earlier phOsal_Timer_Stop() functions was provided to stop the timer.
 * Function phOsal_Timer_Stop() is changed to phOsal_TimerStop() function to stop the timer.
 *
 * @param[in]  timerHandle         Newly created Timer Handle.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_TimerStop(
    phOsal_Timer_t * timerHandle);

/**
 * \brief This function will give the elapsed timing of the timer.
 *
 * @param[in]  timerHandle          Newly created Timer Handle.
 * @param[out] pdwGetElapsedTime    Elapsed Time value(ms)
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_TimerGetCurrent(
    phOsal_Timer_t * timerHandle,
    uint32_t * pdwGetElapsedTime);

/**
 * \brief This function is used to modify the parameters of the timer.
 *
 * @param[in]  timerHandle          Newly created Timer Handle.
 * @param[in]  timerObj             Timer object used to modify the timer.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_TimerModify(
    phOsal_Timer_t * timerHandle,
    pphOsal_TimerObj_t timerObj);

/**
 * \brief This function is delete the timer.
 *
 * Earlier phOsal_Timer_Delete() functions was provided to delete the timer.
 * Function phOsal_Timer_Delete() is changed to phOsal_TimerDelete() function to delete the timer.
 *
 * @param[in]  timerHandle          Newly created Timer Handle.
 *
 * @return Status of the API
 * @retval #PH_OSAL_SUCCESS             Operation successful.
 * @retval #PH_OSAL_ERROR               Parameter passed is invalid.
 */
extern phStatus_t phOsal_TimerDelete(
    phOsal_Timer_t * timerHandle);

/**
 * \brief This function will start the scheduler.
 * If insufficient memory, this function will exit.
 * In normal case this function should not exit.
 *
 * Instead of using any proprietary function use phOsal_StartScheduler() to schedule the tasks/threads.
 *
 */
extern void phOsal_StartScheduler(void);

/** @} */

#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif /* PHOSAL_H */
