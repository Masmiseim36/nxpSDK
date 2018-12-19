/**
* @file ax_api.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2016 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*
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
