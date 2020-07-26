/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_EVENT_H
#define OSA_EVENT_H

/*!
 * @file    osa_event.h
 * @brief   Contains timeout/event function prototypes for Impresario libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Create a timeout event
 * @details Create a timeout event object and associate it with the callback
 *          and callback argument.  This will start a timer thread.
 *
 * @param   ev Output parameter to store event object
 * @param   cb Callback function for timer event object.  Note that the
 *             callback function will be invoked from a separate thread on all
 *             platforms, so the writer of this should be careful when
 *             blocking or when using global data.
 * @param   arg Argument to be passed to callback function
 *
 * @retval  ERRCODE_NO_ERROR     Successfully created timeout event object.
 * @retval  ERRCODE_OUT_OF_MEMORY      No memory available.
 */
int osa_timeout_create(OsaEvent *ev, osa_event_cb cb, void *arg);

/*!
 * @ingroup libosa
 * @brief   Start a timeout event
 * @details Initialize the event timeout object to invoke the callback after
 *          the specified amount of time.  The time is relative to the current
 *          time, not absolute.  Only one event will be triggered - the timer
 *          can be reset by calling osa_timeout_create() again.
 *
 * @param   ev Valid event timeout object
 * @param   time Pointer to OsaTimeval object with relative time to invoke
 *               event callback.
 *
 * @retval  ERRCODE_NO_ERROR     Successfully created timeout event object.
 * @retval  ERRCODE_OUT_OF_MEMORY      No memory available.
 * @retval  ERRCODE_INVALID_ARGUMENT      Invalid time or event timeout object.
 */
int osa_timeout_start(OsaEvent *ev, const OsaTimeval *time);

/*!
 * @ingroup libosa
 * @brief   Cancel a pending timeout event
 * @details Cancel a pending timeout event if the ev object points to a valid
 *          event that has been started.
 *
 * @param   ev Valid event timeout object
 *
 * @retval  ERRCODE_NO_ERROR     Successfully created timeout event object.
 * @retval  ERRCODE_INVALID_ARGUMENT      Invalid time or event timeout object.
 */
int osa_timeout_cancel(OsaEvent *ev);

/*!
 * @ingroup libosa
 * @brief   Destroy a timeout event object
 * @details Free the memory associated with a timeout event object and cancel
 *          all outstanding event notifications that may have been started.
 *          On Linux this function also causes the timer thread to exit if
 *          there are no outstanding event timer objects in the process.
 *
 * @param   ev Valid event timeout object
 *
 * @retval  ERRCODE_NO_ERROR     Successfully created timeout event object.
 * @retval  ERRCODE_INVALID_ARGUMENT      Invalid time or event timeout object.
 */
int osa_timeout_destroy(OsaEvent *ev);

#endif

