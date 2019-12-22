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

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include "sm_types.h"
#include "ax_common.h"
#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
#include "a71ch_api.h"
#include "ax_scp.h"
#elif (SSS_HAVE_A71CL || SSS_HAVE_SE050_L)
#include "a71cl_api.h"
#include "ax_scp.h"
#elif defined TGT_A70CI
#include "a70ci_api.h"
#elif defined TGT_A70CM
#include "a70cm_api.h"
#elif defined TGT_NONE
#include "ax_common_a71ch.h"
#include "a71ch_api.h"
/* Nothing specific to do */
#elif SSS_HAVE_SE05X
#if AX_EMBEDDED
#include "ax_scp.h"
#endif
#include "a71ch_api.h"
#include "ax_common_a71ch.h"
#elif (SSS_HAVE_MBEDTLS || SSS_HAVE_OPENSSL)
/* Nothing specific to do */
#else
#error "Define TGT_X (the secure module target, either TGT_A71CH, TGT_A70CI or TGT_A70CM) as a preprocessor constant"
#endif

#include <sm_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @cond Dummy definition
typedef enum SST_Item
{
    SST_DUMMY        = 0xFFFF
} SST_Item_t;
/// @endcond

#if (SSS_HAVE_A71CH || SSS_HAVE_A71CL || SSS_HAVE_SE050_EAR_CH || SSS_HAVE_SE050_L)
U16 SM_ResumeConnection(SmCommState_t *commState, Scp03SessionState_t *scp03State);
#endif

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
