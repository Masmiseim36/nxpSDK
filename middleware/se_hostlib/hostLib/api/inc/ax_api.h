/**
* @file ax_api.h
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
* This file is the communication specific Host API of the A70CM/CI and A71CH secure module.
* It also customizes the Generic Ax library for this specific product instance
* @par History
* 1.0   27-march-2014 : Initial version
*        7-may-2017   : Unified version for A70CM, CI and A71CH
*
*****************************************************************************/
#ifndef _AX_API_
#define _AX_API_

#include "sm_types.h"
#include "ax_common.h"
#if defined(TGT_A71CH)
#include "a71ch_api.h"
#include "ax_scp.h"
#elif defined TGT_A70CI
#include "a70ci_api.h"
#elif defined TGT_A70CM
#include "a70cm_api.h"
#elif defined TGT_NONE
/* Nothing specific to do */
#else
#error "Define TGT_X (the secure module target, either TGT_A71CH, TGT_A70CI or TGT_A70CM) as a preprocessor constant"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// @cond Dummy definition
typedef enum SST_Item
{
    SST_DUMMY        = 0xFFFF
} SST_Item_t;
/// @endcond

/**
 * Contains the information required to resume a connection with the Security Module.
 * Its content is only to be interpreted by the Host Library.
 * The semantics of the param1 and param2 fields depends on the link layer.
 */
typedef struct {
    U16 param1;          //!< Useage depends on link layer
    U16 param2;          //!< Useage depends on link layer
    U16 hostLibVersion;  //!< MSByte contains major version (::AX_HOST_LIB_MAJOR); LSByte contains minor version of HostLib (::AX_HOST_LIB_MINOR)
    U16 appletVersion;   /*!< MSByte contains major version;
                              3 leading bits of LSByte contains minor version of Applet;
                              Last bit of LSByte encodes whether Applet is in Debug Mode, a '1' means 'Debug Mode' is available */
    U16 sbVersion;       //!< Expected to be 0x0000
} SmCommState_t;

/** \name Communication functions
   @{ */
U16 SM_Close(U8 mode);
U16 SM_Connect(SmCommState_t *commState, U8 *atr, U16 *atrLen);
U16 SM_RjctConnect(const char *connectString, SmCommState_t *commState, U8 *atr, U16 *atrLen);
#if defined(TGT_A71CH)
U16 SM_ResumeConnection(SmCommState_t *commState, Scp03SessionState_t *scp03State);
#endif
U16 SM_SendAPDU(U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen);
/** @}*/

#if defined(TGT_A71CH)
#if defined(IPC)
U16 SM_LockChannel(void);
U16 SM_UnlockChannel(void);

#define SM_LOCK_CHANNEL() SM_LockChannel()
#define SM_UNLOCK_CHANNEL() SM_UnlockChannel()
#else
#define SM_LOCK_CHANNEL()
#define SM_UNLOCK_CHANNEL()
#endif // TGT_A71CH
#endif // IPC

#ifdef __cplusplus
}
#endif
#endif //_AX_API_
