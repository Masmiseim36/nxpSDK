/**
* @file ax_util.h
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
* Host Library Helper functions.
* @par History
* 1.0   26-march-2014 : Initial version
*        9-march-2016 : Update
*        7-may-2017   : Unified version for A70CM, CI and A71CH
 *****************************************************************************/
#ifndef _AX_UTIL_H_
#define _AX_UTIL_H_

#include "ax_api.h"

#if defined(TGT_A71CH)
#include "a71ch_util.h"
#include "a71ch_const.h"
#elif defined TGT_A70CI
#include "a70ci_util.h"
#elif defined TGT_A70CM
// No A70CM specific utils required.
#else
#error "Define TGT_X (the secure module target, either TGT_A71CH, TGT_A70CI or TGT_A70CM) as a preprocessor constant"
#endif

#ifdef __cplusplus
extern "C" {
#endif

ChannelId_t DEV_GetSelectedChannel(scp_CommandType_t *commandType);

#ifdef __cplusplus
}
#endif
#endif //_AX_UTIL_H_
