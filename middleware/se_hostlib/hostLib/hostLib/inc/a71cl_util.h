/**
* @file a71cl_util.h
* @author NXP Semiconductors
* @version 1.0
* @par License
*
* Copyright 2016 NXP
* SPDX-License-Identifier: Apache-2.0
*
* @par Description
* Host Library Helper functions.
* @par History
* 1.0   26-march-2014 : Initial version
*        9-march-2016 : Update
 *****************************************************************************/
#ifndef _A71CL_UTIL_H_
#define _A71CL_UTIL_H_

#include "ax_api.h"

#ifdef __cplusplus
extern "C" {
#endif

U16 SCP_HostLocal_GetKeyDek(ChannelId_t channelId, U8 *keyDek);
U16 SCP_HostLocal_GetKeyEnc(ChannelId_t channelId, U8 *keyEnc);
U16 SCP_HostLocal_GetKeyMac(ChannelId_t channelId, U8 *keyMac);
void DEV_SetChannelCommandType(ChannelId_t channelId, scp_CommandType_t commandType);
void DEV_ClearChannelState();
void DEV_SetResumeHostChannelState();

#ifdef __cplusplus
}
#endif
#endif //_A71CL_UTIL_H_
