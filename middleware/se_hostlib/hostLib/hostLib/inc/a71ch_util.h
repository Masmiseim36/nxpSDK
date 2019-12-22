/**
* @file a71ch_util.h
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
 *****************************************************************************/
#ifndef _A71CH_UTIL_H_
#define _A71CH_UTIL_H_

#include "ax_api.h"

#ifdef __cplusplus
extern "C" {
#endif

U16 SCP_HostLocal_GetKeyDek(ChannelId_t channelId, U8 *keyDek);
U16 SCP_HostLocal_GetKeyEnc(ChannelId_t channelId, U8 *keyEnc);
U16 SCP_HostLocal_GetKeyMac(ChannelId_t channelId, U8 *keyMac);
void DEV_SetChannelCommandType(ChannelId_t channelId, scp_CommandType_t commandType);
void DEV_ClearChannelState(void);
void DEV_SetResumeHostChannelState(void);

#ifdef __cplusplus
}
#endif
#endif //_A71CH_UTIL_H_
