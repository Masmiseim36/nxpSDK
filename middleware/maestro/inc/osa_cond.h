/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_COND_H
#define OSA_COND_H

/*!
 * @file    osa_cond.h
 * @brief   Contains condition variable (CV) function prototypes
 */

/*!
 * @ingroup libosa
 * @brief   Create CV object
 * @details Create and return a new condition variable object
 * @param   cond A condition object that will be allocated and initialized.
 *               It is returned to the call on success.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_BUSY    Attempting to reinitialize an existing CV
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
 */
int osa_cond_create(OsaCondition *cond);

/*!
 * @ingroup libosa
 * @brief   Destroy CV object
 * @details Free the resources for a condition variable object
 * @param   cond A valid condition object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_BUSY    Attempting to reinitialize an existing CV
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
 */
int osa_cond_destroy(OsaCondition *cond);

/*!
 * @ingroup libosa
 * @brief   Wait on a condition
 * @details Release the mutex and wait on the condition argument.  The mutex
 *          is reacquired before the function returns.
 * @param   cond A valid condition object
 * @param   mutex A valid mutex object that the caller holds a lock on
 * @param   msec_timeout Optional timeout.  If zero, the wait will block
 *                       forever.  Any other value will cause the wait to
 *                       abort after the specified number of milliseconds.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid condition parameter
 * @retval  ERRCODE_PERMISSION_DENIED    Mutex not owned by the calling thread
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
 *
 * @todo    This can return error codes from osa_mutex_lock/unlock
*/
int osa_cond_wait(OsaCondition *cond, OsaMutex *mutex, uint32_t msec_timeout);

/*!
 * @ingroup libosa
 * @brief   Signal a condition
 * @details Unblock one thread waiting for the condition argument
 * @param   cond A valid condition object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid condition parameter
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_cond_signal(OsaCondition *cond);

/*!
 * @ingroup libosa
 * @brief   Broadcast a condition
 * @details Unblock all threads waiting for the condition argument
 * @param   cond A valid condition object
 * @warning On MQX this does the same as osa_cond_signal
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid condition parameter
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_cond_broadcast(OsaCondition *cond);

#endif

