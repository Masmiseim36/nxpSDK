/**
 * @file ax_scp.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017 NXP
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
#include <HLSETypes.h>


/*
 * It is implicitly expected that ``keyEnc``, ``keyMac`` & ``keyDek`` points to a buffer
 * that holds an AES128 Key for SCP of appropriate length.
 *
 * ``mcv`` points to a buffer of 16 bytes. And similarly buffers and sizes as specified
 * by the Global Platform Specification for SCP03.
 *
 * Any violation to this rule would lead to nondeterministic
 * behaviour of the system.
 */


#define SST_HOST_SCP_KEYSET 0x2100
typedef void (*SCP_SignalFunction)  (ScpEvent_t event, void *context);

U16 SCP_Subscribe(SCP_SignalFunction callback, void *context);

U16 SCP_Authenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes, U8 *sCounter, U16 *sCounterLen);
U16 SCP02_Authenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes, U8 *sCounter, U16 *sCounterLen);
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
