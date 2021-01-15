/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_TIME_H
#define OSA_TIME_H

/*!
 * @file    osa_time.h
 * @brief   Contains time function prototypes for libosa.
 */

/*!
 * Time since epoch, in seconds
 * Same as a UNIX time_t struct.  Represents UNIX type since epoch (Jan 1
 * 1970).  Will overflow in 2038 but shouldn't be a big deal for us.
 */
typedef uint32_t OsaTime;

/*!
 * Time since epoch, with microsecond resolution
 * Represents precise time.  Just like UNIX timeval struct.
 * tv_sec is time in seconds.
 * tv_usec is time in microseconds.
 */
struct _OsaTimeval {
    OsaTime tv_sec;
    uint32_t tv_usec;
};

/* Represents a calendar date. */
//struct OsaDate;

typedef struct _OsaTimeval OsaTimeval;

/* Retrieve the current time.  Just like UNIX gettimeofday. */

/*!
 * @ingroup libosa
 * @brief   Get the current time
 * @details Retrieve the current time.  This function behaves like POSIX
 *          gettimeofday(), without the timezone.
 *          For MQX, this returns the time since MQX started running.
 *
 * @param   time Output parameter of type OsaTimeval stores the current time.
 *          It contains tv_usec which will contain the time in microseconds.
 *          and tv_sec which will contain the time in seconds.
 *
 * @retval  ERRCODE_NO_ERROR  This function will always succeed for MQX and
 *                            Linux.
*/
int osa_time_get(OsaTimeval *time);

/* Pause the running thread for 'msec' number of milliseconds. */

/*!
 * @ingroup libosa
 * @brief   Suspend thread execution
 * @details Block the current task for the specified number of milliseconds
 * @param   msec Number of milliseconds to block.  This should not be below 5
 *          milliseconds since the resolution of the OS may not support less.
*/
void osa_time_delay(uint32_t msec);

/*!
 * @ingroup libosa
 * @brief   Add to time value
 * @details Add a positive or negative number of microseconds to 'time'.
 * @param   time Current time
 * @param   microseconds Number of usec to add to the time parameter
*/
void osa_time_add(OsaTimeval *time, int32_t microseconds);

/* Return the difference in time between t1 and t2 and store in diff. */
/*!
 * @ingroup libosa
 * @brief   Calculate time difference
 * @details Return the number of seconds elapsed between time 't1' and 't2'.
 * @param   t1 Starting time
 * @param   t2 Ending time
 * @param   diff Time difference
 * @retval  ERRCODE_NO_ERROR  This function will always succeed for MQX and
 *                            Linux.
 * @todo    fix this function.  should return an integer/double for timediff.
*/
int osa_time_diff(const OsaTimeval *t1, const OsaTimeval *t2, OsaTimeval *diff);

#endif

