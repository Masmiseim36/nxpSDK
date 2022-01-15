/**
* @file ax_util.h
* @author NXP Semiconductors
* @version 1.0
* @par License
*
* Copyright 2016,2020 NXP
* SPDX-License-Identifier: Apache-2.0
*
* @par Description
* Host Library Helper functions.
* @par History
* 1.0   26-march-2014 : Initial version
*        9-march-2016 : Update
*        7-may-2017   : Unified version for A70CM, CI and A71CH
 *****************************************************************************/
#ifndef _AX_UTIL_H_
#define _AX_UTIL_H_

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include "ax_api.h"

#if defined(TGT_A71CL)
#include "a71cl_util.h"
#include "sm_const.h"
#elif (SSS_HAVE_APPLET_A71CH || SSS_HAVE_APPLET_A71CH_SIM || SSS_HAVE_APPLET_SE05X_IOT)
#include "a71ch_util.h"
#include "sm_const.h"
#elif defined TGT_A70CI
#include "a70ci_util.h"
#elif defined TGT_A70CM
// No A70CM specific utils required.
#elif defined TGT_NONE
#include "a71ch_util.h"
#include "sm_const.h"
#else
#error "Define TGT_X (the secure module target, either TGT_A71CH, TGT_A70CI or TGT_A70CM) as a preprocessor constant"
#endif

#ifdef __cplusplus
extern "C" {
#endif

ChannelId_t DEV_GetSelectedChannel(scp_CommandType_t *commandType);
void DEV_setDataDerivationArray(U8 ddA[], U16 *pDdALen, U8 ddConstant, U16 ddL, U8 iCounter, U8 *context, U16 contextLen);
#ifdef __cplusplus
}
#endif
#endif //_AX_UTIL_H_
