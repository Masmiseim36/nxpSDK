/*
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _SM_TIMER_H_
#define _SM_TIMER_H_

#include <stdint.h>
#ifdef __gnu_linux__
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* function used for delay loops */
uint32_t sm_initSleep(void);
void sm_sleep(uint32_t msec);
void sm_usleep(uint32_t microsec);

#ifdef __cplusplus
}
#endif
#endif // _SM_TIMER_H_
