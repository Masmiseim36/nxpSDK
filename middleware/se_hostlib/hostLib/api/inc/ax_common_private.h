/**
 * @file ax_common_private.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017
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
 * @par Description
 * Common data types and defines of the AX-family host library (with private scope).
 * @par History
 * 1.0   2017-may-5: Initial version
 *
 */
#ifndef _AX_COMMON_PRIVATE_
#define _AX_COMMON_PRIVATE_

#include "scp.h"
#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TGT_A70CI
// Nothing so far

#elif defined(TGT_A70CM)
// Nothing so far

#elif defined(TGT_A71CH)
#define AX_MODULE_INFO_RESPONSE_LEN 6

#else
// Nothing so far

#endif

#ifdef __cplusplus
}
#endif
#endif //_AX_COMMON_PRIVATE_
