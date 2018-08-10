/**
* @file ax_util.h
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
