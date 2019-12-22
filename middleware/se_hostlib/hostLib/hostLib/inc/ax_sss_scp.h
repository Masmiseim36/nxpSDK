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

#ifndef _AX_SSS_SCP_H_
#define _AX_SSS_SCP_H_


#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "sm_types.h"
#include <HLSETypes.h>
#include <ax_common.h>
#include "nxScp03_Types.h"

#if SSS_HAVE_SCP_SCP03_SSS

#define     SM_SCP03  0x01        //!< Global Platform's Secure Channel Protocol 03
#define     SM_SCP02  0x02        //!< Global Platform's Secure Channel Protocol 02



U16 SCP_SSS_Authenticate(SM_SECURE_SCP03_KEYOBJ * params, U8 *sCounter, U16 *sCounterLen);
U16 SCP_SSS_HostLocal_CalculateSessionKeys(ChannelId_t channelId,SM_SECURE_SCP03_KEYOBJ * params, U8 *hostChallenge, U8 *cardChallenge);
U16 SCP_SSS_HostLocal_VerifyCardCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *cardCryptoGram);
U16 SCP_SSS_HostLocal_CalculateHostCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *hostCryptogram);
U16 SCP_SSS_GP_ExternalAuthenticate(ChannelId_t channelId, U8* hostCryptogram);
U16 SCP_SSS_HostLocal_SetDefaultValueIcvCCounter(ChannelId_t channelId);
U16 SCP_SSS_HostLocal_GetSessionState(ChannelId_t channelId, Scp03_SSS_SessionState_t *pSession);
U16 SCP_SSS_HostLocal_SetMacChainingValue(ChannelId_t channelId, U8 *mcv);
U16 SCP_SSS_HostLocal_ResetMacChainingValue(ChannelId_t channelId);
U16 SCP_SSS_HostLocal_IncIcvCCounter(ChannelId_t channelId);
HLSE_RET_CODE   SCP_SSS_CalculateCMAC(sss_object_t key,
    U8* inData, U32 inDataLen,
    U8* outSignature, U32* outSignatureLen);
U16 SCP_SSS_Save_SessionKeyObj(sss_object_t *keyObj, U8 *Sessionkey);
U16 SM_ChannelAuthenticate(SM_SECURE_SCP03_KEYOBJ *scp03Params, U8 *cCounter);

#endif //#if SSS_HAVE_SCP_SCP03_SSS
#endif
