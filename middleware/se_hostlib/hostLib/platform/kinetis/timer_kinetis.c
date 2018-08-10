/*
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 */

#include <sm_timer.h>
#include <stdint.h>

#include "board.h"

volatile uint32_t gtimer_kinetis_msticks; // counter for 1ms SysTicks

volatile int gusleep_delay;

#define CORR_FRDM_K64F_ARMCC   (1000/100)
#define CORR_FRDM_K64F_ICCARM  (1000/108)
#define CORR_FRDM_K64F_GCC     (1000/100)

#define CORR_MIMXRT1050_EVK_ARMCC   (1000/11)
#define CORR_MIMXRT1050_EVK_ICCARM  (1000/11)
#define CORR_MIMXRT1050_EVK_GCC     (1000/11)

#define CORR_FRDM_K82F_ARMCC   (1000/76)
#define CORR_FRDM_K82F_ICCARM  (1000/83)
#define CORR_FRDM_K82F_GCC     (1000/76)

#define CORR_FRDM_KW41Z_ARMCC  (1000/81)
#define CORR_FRDM_KW41Z_ICCARM (1000/83)
#define CORR_FRDM_KW41Z_GCC    (1000/250)  //for consistency sake 4 is specified in same format


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

#elif defined(MIMXRT1050_EVK)
#    if defined(__ARMCC_VERSION)
#       define CORRECTION_TOLERENCE CORR_MIMXRT1050_EVK_ARMCC
#    elif defined(__ICCARM__)
#       define CORRECTION_TOLERENCE CORR_MIMXRT1050_EVK_ICCARM
#    else
#       define CORRECTION_TOLERENCE CORR_MIMXRT1050_EVK_GCC
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

#if defined(__ARMCC_VERSION)
#pragma O0
#endif

#ifdef __ICCARM__
#pragma optimize=none
#endif

#ifdef __GNUC__
__attribute__((optimize("O0")))
#endif

void sm_usleep(uint32_t microsec) {
    gusleep_delay = microsec * CORRECTION_TOLERENCE;
    while (gusleep_delay-- ) {
        __NOP();
    }
}
