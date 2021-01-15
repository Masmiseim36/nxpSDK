/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_SEM_H
#define OSA_SEM_H

/*!
 * @file    osa_sem.h
 * @brief   Contains semaphore function prototypes for Impresario libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Create a semaphore
 * @details Create a semaphore object and return to the caller.  A value must be
 *          specified as the initial value for the semaphore.
 * @param   semaphore A semaphore object that will be created and returned to the
 *          caller
 * @param   initial_value Unsigned int value specifying the initial value for this semaphore
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid semaphore object passed in
 * @retval  ERRCODE_TRY_AGAIN   System limits reached (Linux only)
 * @retval  ERRCODE_PERMISSION_DENIED    Caller does not have sufficient
 *                                       privilege (Linux only)
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_sem_create(OsaSem *semaphore, uint32_t initial_value);

/*!
 * @ingroup libosa
 * @brief   Destroy a semaphore
 * @details Free the resources associated with a semaphore object. On MQX, all blocking threads
 *             are put in ready status.
 *             On linux, these blocking threads produces undefined behavior.
 * @param   semaphore A valid semaphore object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid semaphore object passed in
*/
int osa_sem_destroy(OsaSem *semaphore);

/*!
 * @ingroup libosa
 * @brief   Post a semaphore
 * @details Increase the semaphore object or unblock it.  If the semaphore is value becomes
 *          greater than or equal zero, then another thread blocked on this semaphore will
 *          be woken up and lock the semaphore.
 * @param   semaphore A valid semaphore object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid semaphore object passed in
 * @retval  ERRCODE_OVERFLOW  The semaphore be posted more that its number (linux only)
*/
int osa_sem_post(OsaSem *semaphore);

/*!
 * @ingroup libosa
 * @brief   Wait a semaphore
 * @details Decrease the semaphore object or lock it. If the semaphore's value is greater
 *            than zero, the decrement proceeds, and the function returns immediately. If the
 *            semaphore value is zero, the call blocks until another task posts the semaphore or
 *            the timeout shoots.
 * @param   semaphore A valid semaphore object
 * @param   timeout A number of milliseconds to wait for a semaphore. If the value is 0,
 *             then the timeout will be infinite.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid semaphore object passed in or thread
 *                       does not own the semaphore
 * @retval  ERRCODE_TIMED_OUT The call timed out before the semaphore could be got.
 * @retval  ERRCODE_INTERNAL System gets a error in creating timer for timeout (linux only)
*/
int osa_sem_wait(OsaSem *semaphore, uint32_t timeout);

int osa_sem_poll(OsaSem *semaphore);

#endif

