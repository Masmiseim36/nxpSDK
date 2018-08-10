/**
 * @file ax_scp.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017
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
 * API to setup an SCP03 communication channel
 * @par History
 * 1.0   26-march-2014 : Initial version
 *
 *****************************************************************************/

#ifndef _AX_SCP_H_
#define _AX_SCP_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "sm_types.h"

#define SST_HOST_SCP_KEYSET 0x2100
typedef void (*SCP_SignalFunction)  (ScpEvent_t event, void *context);

U16 SCP_Subscribe(SCP_SignalFunction callback, void *context);
U16 SCP_Authenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes, U8 *sCounter, U16 *sCounterLen);
U16 SCP_GetScpSessionState(Scp03SessionState_t *scp03state);
void SCP_SetScpSessionState(Scp03SessionState_t *scp03state);

/// @cond
U16 SCP_HostLocal_GetSessionState(ChannelId_t channelId, Scp03SessionState_t *pSession);
U16 SCP_HostLocal_SetDefaultValueIcvCCounter(ChannelId_t channelId);
U16 SCP_HostLocal_IncIcvCCounter(ChannelId_t channelId);
U16 SCP_HostLocal_SetMacChainingValue(ChannelId_t channelId, U8 *mcv);
U16 SCP_HostLocal_CalculateSessionKeys(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge);
U16 SCP_HostLocal_CalculateHostCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *hostCryptogram);
U16 SCP_HostLocal_SetKeysScp(ChannelId_t channelId, U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes);
U16 SCP_HostLocal_VerifyCardCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *cardCryptogram);
U16 SCP_GP_ExternalAuthenticate(ChannelId_t channelId, U8* hostCryptogram);
U16 SCP_GP_InitializeUpdate(ChannelId_t channelId, U8 *hostChallenge, U16 hostChallengeLen, U8 *keyDivData, U16 *pKeyDivDataLen, U8 *keyInfo, U16 *pKeyInfoLen, U8 *cardChallenge, U16 *pCardChallengeLen, U8 *cardCryptoGram, U16 *pCardCryptoGramLen, U8 *seqCounter, U16 *pSeqCounterLen);
/// @endcond
U16 SCP_GP_PutKeys(U8 keyVersion, U8 *keyEnc, U8 *keyMac, U8 *keyDek, U8 *currentKeyDek, U16 keyBytes);

#endif
