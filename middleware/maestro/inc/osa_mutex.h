/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_MUTEX_H
#define OSA_MUTEX_H

/*!
 * @file    osa_mutex.h
 * @brief   Contains mutex function prototypes for Impresario libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Create a mutex
 * @details Create a mutex object and return to the caller.  Mutex must be
 *          specified as recursive or non-recursive on creation.
 * @param   mutex A mutex object that will be created and returned to the
 *          caller
 * @param   recursive Boolean value specifying if the mutex is recursive
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid mutex object passed in
 * @retval  ERRCODE_TRY_AGAIN   System limits reached (Linux only)
 * @retval  ERRCODE_PERMISSION_DENIED    Caller does not have sufficient
 *                                       privilege (Linux only)
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_mutex_create(OsaMutex *mutex, bool recursive);

/*!
 * @ingroup libosa
 * @brief   Destroy a mutex
 * @details Free the resources associated with a mutex object.
 * @param   mutex A valid mutex object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid mutex object passed in
 * @retval  ERRCODE_BUSY    Mutex is currently locked (Linux only)
*/
int osa_mutex_destroy(OsaMutex *mutex);

/*!
 * @ingroup libosa
 * @brief   Lock a mutex
 * @details Retrieve a lock on a mutex object or block until the mutex is
 *          available.  If the mutex is recursive, a thread can lock it more
 *          than once.
 * @param   mutex A valid mutex object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid mutex object passed in
 * @retval  ERRCODE_DEADLOCK  System has detected a deadlock (MQX only)
*/
int osa_mutex_lock(OsaMutex *mutex);

/*!
 * @ingroup libosa
 * @brief   Unlock a mutex
 * @details Release the lock on a held mutex.
 * @param   mutex A valid mutex object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid mutex object passed in or thread
 *                       does not own the mutex
 * @todo    Add ERRCODE_PERMISSION_DENIED for no-owner
*/
int osa_mutex_unlock(OsaMutex *mutex);

/*!
 * @ingroup libosa
 * @brief   Try a mutex lock
 * @details Attempt to lock a mutex but don't block if it's already locked.
 * @param   mutex A valid mutex object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_BUSY    Mutex is already locked by another thread
 * @retval  ERRCODE_DEADLOCK  Mutex is already locked by the calling thread (MQX
 *                       only)
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid mutex object passed in or thread
 *                       does not own the mutex
 * @todo    Add ERRCODE_PERMISSION_DENIED for no-owner
*/
int osa_mutex_trylock(OsaMutex *mutex);

#endif

