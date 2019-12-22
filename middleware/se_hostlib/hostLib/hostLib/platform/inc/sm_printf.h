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

#ifndef _SM_PRINTF_H_
#define _SM_PRINTF_H_
#include <stdint.h>
#include <stdio.h>
#include "sm_types.h"
#ifdef __cplusplus
extern "C" {
#endif


#if AX_EMBEDDED
#   include "fsl_debug_console.h"
#else
#   define PRINTF printf
#   define SCANF scanf
#   define PUTCHAR putchar
#   define GETCHAR getchar
#endif




#define CONSOLE         (0x01)
#define MEMORY          (0x02)
#define LOGFILE         (0x04)
#define DBGOUT_ALL      (CONSOLE|MEMORY|LOGFILE)

#define DBGOUT          CONSOLE

void sm_printf(unsigned char dev, const char * format, ...);
void AssertZeroAllocation(void);

#ifdef __cplusplus
}
#endif
#endif // _SM_PRINTF_H_
