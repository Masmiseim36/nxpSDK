/*
 * Copyright 2016-2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <sm_timer.h>
#include <stdint.h>

#include "board.h"

volatile uint32_t gtimer_kinetis_msticks; // counter for 1ms SysTicks

volatile int gusleep_delay;

#define CORR_FRDM_K64F_ARMCC   (1000/100)
#define CORR_FRDM_K64F_ICCARM  (1000/108)
#define CORR_FRDM_K64F_GCC     (1000/100)

#define CORR_MIMXRT1050_EVK_ARMCC   (1000/125)
#define CORR_MIMXRT1050_EVK_ICCARM  (1000/125)
#define CORR_MIMXRT1050_EVK_GCC     (1000/125)

#define CORR_FRDM_K82F_ARMCC   (1000/76)
#define CORR_FRDM_K82F_ICCARM  (1000/83)
#define CORR_FRDM_K82F_GCC     (1000/76)

#define CORR_FRDM_KW41Z_ARMCC  (1000/81)
#define CORR_FRDM_KW41Z_ICCARM (1000/83)
#define CORR_FRDM_KW41Z_GCC    (1000/251)  //for consistency sake 4 is specified in same format

#define CORR_LPC54018_ARMCC  (1000/125)
#define CORR_LPC54018_ICCARM (1000/112)
#define CORR_LPC54018_GCC    (1000/125)  //for consistency sake 4 is specified in same format


#if defined(FRDM_KW41Z)
#    if defined(__ARMCC_VERSION)
#       define CORRECTION_TOLERENCE CORR_FRDM_KW41Z_ARMCC
#    elif defined(__ICCARM__)
#       define CORRECTION_TOLERENCE CORR_FRDM_KW41Z_ICCARM
#    else
#       define CORRECTION_TOLERENCE CORR_FRDM_KW41Z_GCC
#    endif
#elif defined(FRDM_K82F)
#    if defined(__ARMCC_VERSION)
#       define CORRECTION_TOLERENCE CORR_FRDM_K82F_ARMCC
#    elif defined(__ICCARM__)
#       define CORRECTION_TOLERENCE CORR_FRDM_K82F_ICCARM
#    else
#       define CORRECTION_TOLERENCE CORR_FRDM_K82F_GCC
#    endif

#elif defined(IMX_RT)
#    if defined(__ARMCC_VERSION)
#       define CORRECTION_TOLERENCE CORR_MIMXRT1050_EVK_ARMCC
#    elif defined(__ICCARM__)
#       define CORRECTION_TOLERENCE CORR_MIMXRT1050_EVK_ICCARM
#    else
#       define CORRECTION_TOLERENCE CORR_MIMXRT1050_EVK_GCC
#    endif
#elif defined(CPU_LPC54018)
#    if defined(__ARMCC_VERSION)
#       define CORRECTION_TOLERENCE CORR_LPC54018_ARMCC
#    elif defined(__ICCARM__)
#       define CORRECTION_TOLERENCE CORR_LPC54018_ICCARM
#    else
#       define CORRECTION_TOLERENCE CORR_LPC54018_GCC
#    endif
#else //defined(FRDM_K64F)
#    if defined(__ARMCC_VERSION)
#       define CORRECTION_TOLERENCE CORR_FRDM_K64F_ARMCC
#    elif defined(__ICCARM__)
#       define CORRECTION_TOLERENCE CORR_FRDM_K64F_ICCARM
#    else
#       define CORRECTION_TOLERENCE CORR_FRDM_K64F_GCC
#    endif
#endif


//for ARM6 taken care at file level
// #ifdef __ARMCC_VERSION
// #pragma O0
// #endif

#ifdef __ICCARM__
#pragma optimize=none
#endif
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
__attribute__((optimize("O0")))
#endif

void sm_usleep(uint32_t microsec) {
    gusleep_delay = microsec * CORRECTION_TOLERENCE;
    while (gusleep_delay-- ) {
        __NOP();
    }
}
