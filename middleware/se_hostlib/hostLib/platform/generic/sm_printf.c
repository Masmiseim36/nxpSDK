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

#include <stdio.h>
#include <stdarg.h>

#include "sm_printf.h"

#ifdef FREEDOM
#   include "fsl_device_registers.h"
#   include "fsl_debug_console.h"
#   include "board.h"
#else
#   define PRINTF printf
#endif

#define MAX_SER_BUF_SIZE 	(1024)

void sm_printf(uint8_t dev, const char * format, ...)
{
    uint8_t  buffer[MAX_SER_BUF_SIZE + 1];
    va_list   vArgs;

    dev = dev; // avoids warning; dev can be used to determine output channel

    va_start(vArgs, format);
#ifdef _WIN32
    vsnprintf_s((char *)buffer, MAX_SER_BUF_SIZE, MAX_SER_BUF_SIZE, (char const *)format, vArgs);
#else
    vsnprintf((char *)buffer, MAX_SER_BUF_SIZE, (char const *)format, vArgs);
#endif
    va_end(vArgs);

    PRINTF("%s", buffer);
}
