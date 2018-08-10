/**
 * @file a71_debug.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
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
 * @par Description
 * @par History
 * 1.0   20-feb-2012 : Initial version
 *
 *****************************************************************************/

#ifndef _A71_DEBUG_
#define _A71_DEBUG_

#include "ax_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @cond
#define P2_PERSISTENT_MEM      0x01
#define P2_TRANS_MEM_DESELECT  0x02
#define P2_TRANS_MEM_RESET     0x03

#define P2_USER_UNAUTH      0x01
#define P2_USER_ADMIN       0x02
#define P2_USER_HOST        0x03
/// @endcond

// #define DBG_KEY_DEK_ADMIN_INDEX 0x21
// #define DBG_KEY_DEK_HOST_INDEX  0x22

/** \name Debug functions (only available in case the Applet is in Debug Mode)
   @{ */
U16 A71_DbgDisableDebug(void);
U16 A71_DbgEraseCounter(SST_Index_t index);
U16 A71_DbgGetFreePersistentMemory(S16 *freeMem);
U16 A71_DbgGetFreeTransientMemory(S16 *freeMem);
U16 A71_DbgReflect(U8 *sndBuf, U16 sndBufLen, U8 *rcvBuf, U16 *rcvBufLen);
U16 A71_DbgReset(void);
/** @}*/

#ifdef __cplusplus
}
#endif
#endif //_SM_DEBUG_
