/*******************************************************************************
 * Copyright 2016, 2017 ARM Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/


#ifndef _PAL_PLAT_RTOS_H
#define _PAL_PLAT_RTOS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pal.h"


/*! \file pal_plat_rtos.h
 *  \brief PAL RTOS - platform.
 *   This file contains the real-time OS APIs that need to be implemented in the platform layer.
 */



/*! \brief Initiate a system reboot.
*/
void pal_plat_osReboot(void);

/*! \brief Application provided implementation to replace default `pal_osReboot()` functionality.
*/
void pal_plat_osApplicationReboot(void);

/*! \brief Initialize all data structures (semaphores, mutexes, memory pools, message queues) at system initialization.
 *
 *   In case of a failure in any of the initializations, the function returns an error and stops the rest of the initializations.
 * @param[in] opaqueContext The context passed to the initialization (not required for generic CMSIS, pass NULL in this case).
 * \return PAL_SUCCESS(0) in case of success, PAL_ERR_CREATION_FAILED in case of failure.
 */
palStatus_t pal_plat_RTOSInitialize(void* opaqueContext);

/*! \brief De-initialize thread objects.
*/
palStatus_t pal_plat_RTOSDestroy(void);

/*! \brief Get the RTOS kernel system timer counter.
 *
 * \return The RTOS kernel system timer counter.
 *
 * \note The required tick counter is the OS (platform) kernel system tick counter.
 *
 * \note If the platform supports 64-bit tick counter, please implement it. If the platform supports only 32 bit, note
 *       that this counter wraps around very often, once every 42 sec for 100Mhz, for example.
 */
uint64_t pal_plat_osKernelSysTick(void);

/*! \brief Convert the value from microseconds to kernel system ticks.
 *
 * This is the same as CMSIS macro `osKernelSysTickMicroSec`.
 * @param microseconds The value in microseconds to be converted to kernel system ticks.
 */
uint64_t pal_plat_osKernelSysTickMicroSec(uint64_t microseconds);

/*! \brief Get the system tick frequency.
 * \return The system tick frequency.
 *
 * \note The system tick frequency MUST be more than 1KHz, so at least one tick per millisecond.
 */
uint64_t pal_plat_osKernelSysTickFrequency(void);

/*! \brief Create and run the thread.
 *
 * @param[in] function A function pointer to the thread callback function.
 * @param[in] funcArgument An argument for the thread function.
 * @param[in] priority The priority of the thread.
 * @param[in] stackSize The stack size of the thread in bytes, can NOT be 0.
 * @param[out] threadID: The created thread ID handle. In case of error, this value is NULL.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 *
 */
palStatus_t pal_plat_osThreadCreate(palThreadFuncPtr function, void* funcArgument, palThreadPriority_t priority, uint32_t stackSize, palThreadID_t* threadID);

/*! \brief Terminate and free allocated data for the thread.
 *
 * @param[in] threadID A pointer to a palThreadData_t structure containing information about the thread.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 *
 */
palStatus_t pal_plat_osThreadTerminate(palThreadID_t* threadID);

/*! \brief Get the ID of the current thread.
 * \return The ID of the current thread. In case of error, returns PAL_MAX_UINT32.
 */
palThreadID_t pal_plat_osThreadGetId(void);

/*! \brief Wait for a specified period of time in milliseconds.
 *
 * @param[in] milliseconds The number of milliseconds to wait before proceeding.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 */
palStatus_t pal_plat_osDelay(uint32_t milliseconds);

/*! \brief Create a timer.
 *
 * @param[in] function A function pointer to the timer callback function.
 * @param[in] funcArgument An argument for the timer callback function.
 * @param[in] timerType The timer type to be created, periodic or `oneShot`.
 * @param[out] timerID The ID of the created timer. Zero value indicates an error.
 *
 * \return PAL_SUCCESS when the timer was created successfully. A specific error in case of failure.
 * \return PAL_ERR_NO_MEMORY: No memory resource available to create a timer object.
 *
 * \note The timer callback function runs according to the platform resources of stack size and priority.
 * \note The create function MUST not wait for platform resources and it should return PAL_ERR_RTOS_RESOURCE, unless the platform API is blocking.
 */
palStatus_t pal_plat_osTimerCreate(palTimerFuncPtr function, void* funcArgument, palTimerType_t timerType, palTimerID_t* timerID);

/*! \brief Start or restart a timer.
 *
 * @param[in] timerID The handle for the timer to start.
 * @param[in] millisec: The time in milliseconds to set the timer to, MUST be larger than 0.
 *                      In case the value is 0, the error PAL_ERR_RTOS_VALUE will be returned.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 */
palStatus_t pal_plat_osTimerStart(palTimerID_t timerID, uint32_t millisec);

/*! \brief Stop a timer.
 *
 * @param[in] timerID The handle for the timer to stop.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 */
palStatus_t pal_plat_osTimerStop(palTimerID_t timerID);

/*! \brief Delete the timer object.
 *
 * @param[inout] timerID The handle for the timer to delete. on success, `timerID` = NULL.
 *
 * \return PAL_SUCCESS when the timer was deleted successfully. PAL_ERR_RTOS_PARAMETER when the `timerID` is incorrect.
 * \note In case of a running timer, `pal_plat_osTimerDelete()` MUST stop the timer before deletion.
 */
palStatus_t pal_plat_osTimerDelete(palTimerID_t* timerID);

/*! \brief Create and initialize a mutex object.
 *
 * @param[out] mutexID The created mutex ID handle, zero value indicates an error.
 *
 * \return PAL_SUCCESS when the mutex was created successfully, a specific error in case of failure.
 * \return PAL_ERR_NO_MEMORY when there is no memory resource available to create a mutex object.
 * \note The create function MUST NOT wait for the platform resources and it should return PAL_ERR_RTOS_RESOURCE, unless the platform API is blocking.
 * \note By default, the mutex is created with a recursive flag set.
 */
palStatus_t pal_plat_osMutexCreate(palMutexID_t* mutexID);

/*! \brief Wait until a mutex becomes available.
 *
 * @param[in] mutexID The handle for the mutex.
 * @param[in] millisec The timeout for the waiting operation. If the timeout expires before the mutex is released, an error is returned from the function.
 *
 * \return PAL_SUCCESS(0) in case of success. One of the following error codes in case of failure: \n
 *         - PAL_ERR_RTOS_RESOURCE - Mutex not available but no timeout set. \n
 *         - PAL_ERR_RTOS_TIMEOUT - Mutex was not available until timeout expired. \n
 *         - PAL_ERR_RTOS_PARAMETER - The mutex ID is invalid. \n
 *         - PAL_ERR_RTOS_ISR - Cannot be called from interrupt service routines.
 */
palStatus_t pal_plat_osMutexWait(palMutexID_t mutexID, uint32_t millisec);

/*! \brief Release a mutex that was obtained by `osMutexWait`.
 *
 * @param[in] mutexID The handle for the mutex.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 */
palStatus_t pal_plat_osMutexRelease(palMutexID_t mutexID);

/*! \brief Delete a mutex object.
 *
 * @param[in,out] mutexID The ID of the mutex to delete. In success, `mutexID` = NULL.
 *
 * \return PAL_SUCCESS when the mutex was deleted successfully, one of the following error codes in case of failure: \n
 *         - PAL_ERR_RTOS_RESOURCE - Mutex already released. \n
 *         - PAL_ERR_RTOS_PARAMETER - Mutex ID is invalid. \n
 *         - PAL_ERR_RTOS_ISR - Cannot be called from interrupt service routines.
 * \note After this call, `mutexID` is no longer valid and cannot be used.
 */
palStatus_t pal_plat_osMutexDelete(palMutexID_t* mutexID);

/*! \brief Create and initialize a semaphore object.
 *
 * @param[in] count The number of available resources.
 * @param[out] semaphoreID The ID of the created semaphore, zero value indicates an error.
 *
 * \return PAL_SUCCESS when the semaphore was created successfully, a specific error in case of failure.
 * \return PAL_ERR_NO_MEMORY: No memory resource available to create a semaphore object.
 * \note The create function MUST not wait for the platform resources and it should return PAL_ERR_RTOS_RESOURCE, unless the platform API is blocking.
 */
palStatus_t pal_plat_osSemaphoreCreate(uint32_t count, palSemaphoreID_t* semaphoreID);

/*! \brief Wait until a semaphore token becomes available.
 *
 * @param[in] semaphoreID The handle for the semaphore.
 * @param[in] millisec The timeout for the waiting operation. If the timeout expires before the semaphore is released, an error is returned from the function.
 * @param[out] countersAvailable The number of semaphores available. If semaphores are not available due to timeout or error, zero is returned.
 * \return PAL_SUCCESS(0) in case of success. One of the following error codes in case of failure: \n
 *       - PAL_ERR_RTOS_TIMEOUT - Semaphore was not available until timeout expired. \n
 *       - PAL_ERR_RTOS_PARAMETER - Semaphore ID is invalid.
 */
palStatus_t pal_plat_osSemaphoreWait(palSemaphoreID_t semaphoreID, uint32_t millisec, int32_t* countersAvailable);

/*! \brief Release a semaphore token.
 *
 * @param[in] semaphoreID The handle for the semaphore.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 */
palStatus_t pal_plat_osSemaphoreRelease(palSemaphoreID_t semaphoreID);

/*! \brief Delete a semaphore object.
 *
 * @param[in,out] semaphoreID: The ID of the semaphore to delete. On success, `semaphoreID` = NULL.
 *
 * \return PAL_SUCCESS when the semaphore was deleted successfully. One of the following error codes in case of failure: \n
 *         PAL_ERR_RTOS_RESOURCE - Semaphore already released. \n
 *         PAL_ERR_RTOS_PARAMETER - Semaphore ID is invalid.
 * \note After this call, the `semaphoreID` is no longer valid and cannot be used.
 */
palStatus_t pal_plat_osSemaphoreDelete(palSemaphoreID_t* semaphoreID);


/*! \brief Perform an atomic increment for a signed 32-bit value.
 *
 * @param[in,out] valuePtr The address of the value to increment.
 * @param[in] increment The number by which to increment.
 *
 * \returns The value of the `valuePtr` after the increment operation.
 */
int32_t pal_plat_osAtomicIncrement(int32_t* valuePtr, int32_t increment);

/*! \brief Perform allocation from the heap according to the OS specification.
 *
 * @param[in] len The length of the buffer to be allocated in bytes.
 *
 * \returns `void *`, or the pointer of the malloc received from the OS if NULL error occurred
 */
void *pal_plat_malloc(size_t len);

/*! \brief Free memory back to the OS heap.
 *
 * @param[in] buffer A pointer to the buffer that should be freed.
 *
 * \returns `void`
 */
void pal_plat_free(void * buffer);


/*! \brief Retrieve platform Root of Trust (RoT) certificate.
 *
 * @param[in,out] keyBuf A pointer to the buffer that holds the RoT.
 * @param[in] keyLenBytes The size of the buffer to hold the 128 bit key, must be at least 16 bytes.
 *
 * \return PAL_SUCCESS(0) in case of success. A negative value indicating a specific error code in case of failure.
 */

palStatus_t pal_plat_osGetRoTFromHW(uint8_t *keyBuf, size_t keyLenBytes);

/*! \brief  This function calls the platform layer and sets the new real-time clock (RTC) to the hardware.
*
* @param[in] rtcSetTime the new RTC time
*
* \return PAL_SUCCESS when the RTC is returned correctly.
*
*/
palStatus_t pal_plat_osSetRtcTime(uint64_t rtcSetTime);

/*! \brief This function gets the real-time clock (RTC) from the platform.
 *
 * @param[out] rtcGetTime - Holds the RTC value
 *
 * \return PAL_SUCCESS when the RTC is returned correctly
 *
 */
palStatus_t pal_plat_osGetRtcTime(uint64_t *rtcGetTime);


/*! \brief This function deinitializes the realt-time clock (RTC) module.
 *
 * \return PAL_SUCCESS when the success or error upon failing
 *
 */
palStatus_t pal_plat_rtcDeInit(void);


/*! \brief This function initializes the real-time clock (RTC) module.
 *
 * \return PAL_SUCCESS when the success or error upon failing
 *
 */
palStatus_t pal_plat_rtcInit(void);

#ifdef __cplusplus
}
#endif
#endif //_PAL_PLAT_RTOS_H
