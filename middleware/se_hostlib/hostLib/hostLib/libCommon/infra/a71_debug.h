/**
 * @file a71_debug.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2016,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
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

#if (SSS_HAVE_APPLET_A71CH || SSS_HAVE_APPLET_A71CL || SSS_HAVE_APPLET_A71CH_SIM || SSS_HAVE_SE050_L)
U16 A71_DbgDisableDebug(void);
U16 A71_DbgEraseCounter(SST_Index_t index);
U16 A71_DbgGetFreePersistentMemory(S16 *freeMem);
U16 A71_DbgGetFreeTransientMemory(S16 *freeMem);
U16 A71_DbgReflect(U8 *sndBuf, U16 sndBufLen, U8 *rcvBuf, U16 *rcvBufLen);
U16 A71_DbgReset(void);
#endif
/** @}*/

#ifdef __cplusplus
}
#endif
#endif //_SM_DEBUG_
