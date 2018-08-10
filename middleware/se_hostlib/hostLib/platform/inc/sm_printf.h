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

#ifndef _SM_PRINTF_H_
#define _SM_PRINTF_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
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
