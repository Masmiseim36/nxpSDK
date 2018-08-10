/**
* @file ax_scp.c
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
* This file implements the setting up of the SCP03 communication channel.
* @note Execution flow and Error messages can be sent to the console by defining
* FLOW_VERBOSE and ERROR_VERBOSE respectively at the start of the source code file.
* @par History
* 1.0   26-march-2014 : Initial version
*
*/
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "ax_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
//#include "axHostCrypto.h"
#include "HostCryptoAPI.h"
#include "ax_util.h"

#include "global_platf.h"

static ScpState_t scpState[2];

// #define FLOW_VERBOSE   //!< Define to enable execution flow messages
#define ERROR_VERBOSE  //!< Define to enable reporting of error messages

/// @cond Logging
#if defined(FLOW_VERBOSE)
static void localPrintByteArray(const char *pName, const U8 *pData, U16 dataLen)
{
    U16 i = 0;
    assert(pName != NULL);
    assert(pData != NULL);

    printf("%s (LEN=%d):\r\n", pName, dataLen);

    for (i = 0; i < dataLen ; i++)
    {
        printf("%02X", pData[i]);
    }
    printf("\r\n");
}
#endif

#ifdef FLOW_VERBOSE
#define FPRINTF(...) printf (__VA_ARGS__)
#define PRINT_BYTE_STRING(A,B,C)  localPrintByteArray(A,B,C)
#else
#define FPRINTF(...)
#define PRINT_BYTE_STRING(A,B,C)
#endif

#ifdef ERROR_VERBOSE
#define EPRINTF(...) printf (__VA_ARGS__)
#else
#define EPRINTF(...)
#endif
/// @endcond

void DEV_setDataDerivationArray(U8 ddA[], U16 *pDdALen,
                                   U8 ddConstant, U16 ddL, U8 iCounter, U8 *context, U16 contextLen)
{
    // SCPO3 spec p9&10
    memset(ddA, 0, DD_LABEL_LEN-1);
    ddA[DD_LABEL_LEN-1] = ddConstant;
    ddA[DD_LABEL_LEN] = 0x00; // Separation Indicator
    ddA[DD_LABEL_LEN+1] = (U8)(ddL >> 8);
    ddA[DD_LABEL_LEN+2] = (U8)ddL;
    ddA[DD_LABEL_LEN+3] = iCounter;
    memcpy(&ddA[DD_LABEL_LEN+4], context, contextLen);
    *pDdALen = DD_LABEL_LEN+4 + contextLen;
}

U16 SCP_HostLocal_SetDefaultValueIcvCCounter(ChannelId_t channelId)
{
    U8 commandCounter[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(scpState[stateIdx].session.cCounter, commandCounter, 16);

    return SCP_OK;
}

// No check on overflow
U16 SCP_HostLocal_IncIcvCCounter(ChannelId_t channelId)
{
    int stateIdx = 0;
    int i = 15;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    i = 15;
    while (i > 0)
    {
        if (scpState[stateIdx].session.cCounter[i] < 255)
        {
            scpState[stateIdx].session.cCounter[i] += 1;
            break;
        }
        else
        {
            scpState[stateIdx].session.cCounter[i] = 0;
            i--;
        }
    }

    return SCP_OK;
}

U16 SCP_HostLocal_SetKeysScp(ChannelId_t channelId, U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes)
{
    int stateIdx = 0;

    // Only 128 bit AES keys supported
    assert(keyBytes == SCP_KEY_SIZE);

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(scpState[stateIdx].keyEnc, keyEnc, keyBytes);
    memcpy(scpState[stateIdx].keyMac, keyMac, keyBytes);
    memcpy(scpState[stateIdx].keyDek, keyDek, keyBytes);

    return SCP_OK;
}

U16 SCP_HostLocal_GetKeyDek(ChannelId_t channelId, U8 *keyDek)
{
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(keyDek, scpState[stateIdx].keyDek, AES_KEY_LEN_nBYTE);

    return SCP_OK;
}

U16 SCP_HostLocal_GetKeyEnc(ChannelId_t channelId, U8 *keyEnc)
{
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(keyEnc, scpState[stateIdx].keyEnc, AES_KEY_LEN_nBYTE);

    return SCP_OK;
}

U16 SCP_HostLocal_GetKeyMac(ChannelId_t channelId, U8 *keyMac)
{
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(keyMac, scpState[stateIdx].keyMac, AES_KEY_LEN_nBYTE);

    return SCP_OK;
}

U16 SCP_HostLocal_ResetMacChainingValue(ChannelId_t channelId)
{
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memset(scpState[stateIdx].session.mcv, 0, SCP_MCV_LEN);

    return SCP_OK;
}

U16 SCP_HostLocal_SetMacChainingValue(ChannelId_t channelId, U8 *mcv)
{
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(scpState[stateIdx].session.mcv, mcv, SCP_MCV_LEN);

    return SCP_OK;
}

/**
 * Copy the session state into \p pSession. Caller must allocate memory of \p pSession.
 * @param[in] channelId Either ::AX_HOST_CHANNEL or ::AX_ADMIN_CHANNEL. Must be ::AX_HOST_CHANNEL in case of A71CH.
 * @param[in,out] pSession IN: pointer to allocated ::Scp03SessionState_t structure; OUT: retrieved state
 * @retval ::SW_OK Upon successful execution
 * @retval ::SCP_UNDEFINED_CHANNEL_ID In case an undefined ::ChannelId_t type was passed as parameter
 */
U16 SCP_HostLocal_GetSessionState(ChannelId_t channelId, Scp03SessionState_t *pSession)
{
    int stateIdx = 0;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    memcpy(pSession, &(scpState[stateIdx].session), sizeof(Scp03SessionState_t));

    return SCP_OK;
}

U16 SCP_HostLocal_CalculateSessionKeys(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge)
{
    int stateIdx = 0;
    U8 ddA[128];
    U16 ddALen = sizeof(ddA);
    U8 context[128];
    U16 contextLen = 0;
    U8 sessionEncKey[AES_KEY_LEN_nBYTE];
    U8 sessionMacKey[AES_KEY_LEN_nBYTE];
    U8 sessionRmacKey[AES_KEY_LEN_nBYTE];
    U8 masterEncKey[AES_KEY_LEN_nBYTE];
    U8 masterMacKey[AES_KEY_LEN_nBYTE];
    HLSE_MECHANISM_INFO mechInfo;
    U32 signatureLen = sizeof(sessionMacKey);
    S32 ret;

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_AES_CMAC;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    FPRINTF("HOST: Calculate session keys\r\n");

    // Calculate the S-ENC key
    memcpy(context, hostChallenge, SCP_GP_HOST_CHALLENGE_LEN);
    memcpy(&context[SCP_GP_HOST_CHALLENGE_LEN], cardChallenge, SCP_GP_CARD_CHALLENGE_LEN);
    contextLen = SCP_GP_HOST_CHALLENGE_LEN + SCP_GP_CARD_CHALLENGE_LEN;

    DEV_setDataDerivationArray(ddA, &ddALen,
            DATA_DERIVATION_SENC, DATA_DERIVATION_L_128BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);
    SCP_HostLocal_GetKeyEnc(channelId, masterEncKey);

    ret = HLCRYPT_Sign(&mechInfo, masterEncKey, AES_KEY_LEN_nBYTE, ddA, ddALen, sessionEncKey, &signatureLen);

    //ret = HOST_CMAC_Get(masterEncKey, AES_KEY_LEN_nBYTE, ddA, ddALen, sessionEncKey);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }
    PRINT_BYTE_STRING("sessionEncKey", sessionEncKey, AES_KEY_LEN_nBYTE);

    // Calculate the S-MAC key
    SCP_HostLocal_GetKeyMac(channelId, masterMacKey);

    DEV_setDataDerivationArray(ddA, &ddALen,
            DATA_DERIVATION_SMAC, DATA_DERIVATION_L_128BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);

    ret = HLCRYPT_Sign(&mechInfo, masterMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, sessionMacKey, &signatureLen);

    //    ret = HOST_CMAC_Get(masterMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, sessionMacKey);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }
    PRINT_BYTE_STRING("sessionMacKey", sessionMacKey, AES_KEY_LEN_nBYTE);

    // Calculate the S-RMAC key
    DEV_setDataDerivationArray(ddA, &ddALen,
            DATA_DERIVATION_SRMAC, DATA_DERIVATION_L_128BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);

    ret = HLCRYPT_Sign(&mechInfo, masterMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, sessionRmacKey, &signatureLen);
    //    ret = HOST_CMAC_Get(masterMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, sessionRmacKey);

    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }
    PRINT_BYTE_STRING("sessionRmacKey", sessionRmacKey, AES_KEY_LEN_nBYTE);

    // Store the Session Keys in the appropriate Channel Session State
    memcpy(scpState[stateIdx].session.sEnc, sessionEncKey, AES_KEY_LEN_nBYTE);
    memcpy(scpState[stateIdx].session.sMac, sessionMacKey, AES_KEY_LEN_nBYTE);
    memcpy(scpState[stateIdx].session.sRMac, sessionRmacKey, AES_KEY_LEN_nBYTE);

    return SCP_OK;
}

U16 SCP_HostLocal_CalculateHostCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *hostCryptogram)
{
    int stateIdx = 0;
    U8 ddA[128];
    U16 ddALen = sizeof(ddA);
    U8 context[128];
    U16 contextLen = 0;
    U8 sessionMacKey[AES_KEY_LEN_nBYTE];
    U8 hostCryptogramFullLength[AES_KEY_LEN_nBYTE];
    HLSE_MECHANISM_INFO mechInfo;
    U32 signatureLen = sizeof(hostCryptogramFullLength);
    S32 ret;

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_AES_CMAC;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    FPRINTF("HOST: Calculate Host Cryptogram\r\n");

    memcpy(context, hostChallenge, SCP_GP_HOST_CHALLENGE_LEN);
    memcpy(&context[SCP_GP_HOST_CHALLENGE_LEN], cardChallenge, SCP_GP_CARD_CHALLENGE_LEN);
    contextLen = SCP_GP_HOST_CHALLENGE_LEN + SCP_GP_CARD_CHALLENGE_LEN;

    DEV_setDataDerivationArray(ddA, &ddALen,
            DATA_HOST_CRYPTOGRAM, DATA_DERIVATION_L_64BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);
    memcpy(sessionMacKey, &(scpState[stateIdx].session.sMac), AES_KEY_LEN_nBYTE);

    ret = HLCRYPT_Sign(&mechInfo, sessionMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, hostCryptogramFullLength, &signatureLen);
//    ret = HOST_CMAC_Get(sessionMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, hostCryptogramFullLength);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }

    // Chop of the tail of the hostCryptogramFullLength
    memcpy(hostCryptogram, hostCryptogramFullLength, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);
    // PRINT_BYTE_STRING("hostCryptogram", hostCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    return SCP_OK;
}

U16 SCP_HostLocal_VerifyCardCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *cardCryptogram)
{
    int stateIdx = 0;
    U8 ddA[128];
    U16 ddALen = sizeof(ddA);
    U8 context[128];
    U16 contextLen = 0;
    U8 sessionMacKey[AES_KEY_LEN_nBYTE];
    U8 cardCryptogramFullLength[AES_KEY_LEN_nBYTE];
    U16 rv = SCP_OK;
    HLSE_MECHANISM_INFO mechInfo;
    U32 signatureLen = sizeof(cardCryptogramFullLength);
    S32 ret;

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_AES_CMAC;

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    FPRINTF("HOST: Verify Card Cryptogram\r\n");

    memcpy(context, hostChallenge, SCP_GP_HOST_CHALLENGE_LEN);
    memcpy(&context[SCP_GP_HOST_CHALLENGE_LEN], cardChallenge, SCP_GP_CARD_CHALLENGE_LEN);
    contextLen = SCP_GP_HOST_CHALLENGE_LEN + SCP_GP_CARD_CHALLENGE_LEN;

    DEV_setDataDerivationArray(ddA, &ddALen,
            DATA_CARD_CRYPTOGRAM, DATA_DERIVATION_L_64BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);
    memcpy(sessionMacKey, &(scpState[stateIdx].session.sMac), AES_KEY_LEN_nBYTE);

    ret = HLCRYPT_Sign(&mechInfo, sessionMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, cardCryptogramFullLength, &signatureLen);
//    ret = HOST_CMAC_Get(sessionMacKey, AES_KEY_LEN_nBYTE, ddA, ddALen, cardCryptogramFullLength);

    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }

    PRINT_BYTE_STRING("cardCryptogramFullLength - Verify", cardCryptogramFullLength, AES_KEY_LEN_nBYTE);

    // Verify whether the 8 left most byte of cardCryptogramFullLength match cardCryptogram
    if ( memcmp(cardCryptogramFullLength, cardCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN) !=  0)
    {
        rv = SCP_CARD_CRYPTOGRAM_FAILS_TO_VERIFY;
    }

    return rv;
}

/**
 * Retrieve the SCP03 session state of the host - secure module channel from the Host Library.
 *
 * @param[in,out] scp03state IN: pointer to allocated structure; OUT: datastructure contains SCP03 session state
 * @returns ::SW_OK
 */
U16 SCP_GetScpSessionState(Scp03SessionState_t *scp03state)
{
    U16 status = SW_OK;
    scp_CommandType_t channelCommandTypeDummy;
    ChannelId_t channelId;

    assert(scp03state != NULL);

    channelId = DEV_GetSelectedChannel(&channelCommandTypeDummy);
    SCP_HostLocal_GetSessionState(channelId, scp03state);

    return (U16) status;
}

/**
 * Sets SCP03 session state of the host - secure module channel of the Host Library.
 * Can be used in a scenario where e.g. the bootloader has established the SCP03 link between
 * host and secure module and the Host OS must re-establish the communication with the
 * secure module without breaking the SCP03 session.
 *
 * @param[in] scp03state IN: SCP03 session state
 */
void SCP_SetScpSessionState(Scp03SessionState_t *scp03state)
{
    memcpy(&(scpState[HOST_CHANNEL_STATE_IDX].session), scp03state, sizeof(Scp03SessionState_t));
}

U16 SCP_GP_ExternalAuthenticate(ChannelId_t channelId, U8* hostCryptogram)
{
    S32 nRet;
    U32 st = 0;
    U8 txBuf[128];
    U8 cla = 0;
    U8 response[128];
    U32 responseLen = 128;
    U16 rv = 0;
    U8 sessionMacKey[AES_KEY_LEN_nBYTE];
    U8 mcv[AES_KEY_LEN_nBYTE];
    U8 macToAdd[AES_KEY_LEN_nBYTE];
    int stateIdx = ADMIN_CHANNEL_STATE_IDX;
    //axHcCmacCtx_t *cmacCtx;
    HLSE_CONTEXT_HANDLE hContext;

    HLSE_MECHANISM_INFO mechInfo;
    U32 signatureLen = sizeof(macToAdd);

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_AES_CMAC;

    FPRINTF(">> %s: Enter\r\n", "SCP_GP_ExternalAuthenticate");

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        stateIdx = HOST_CHANNEL_STATE_IDX;
        break;
    case AX_ADMIN_CHANNEL:
        stateIdx = ADMIN_CHANNEL_STATE_IDX;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }
    cla = 0x84;

    txBuf[0] = cla;
    txBuf[1] = INS_GP_EXTERNAL_AUTHENTICATE;
    txBuf[2] = SECLVL_CDEC_RENC_CMAC_RMAC;
    txBuf[3] = 0x00;
    txBuf[4] = 0x10; // The Lc valus is set as-if the MAC has already been appended (SCP03 spec p16. Fig.6-1)
    memcpy(&txBuf[5], hostCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    // Calculate the MAC value

    memcpy(sessionMacKey, scpState[stateIdx].session.sMac, AES_KEY_LEN_nBYTE);

    PRINT_BYTE_STRING(">> sessionMacKey", sessionMacKey, AES_KEY_LEN_nBYTE);

    nRet = HLCRYPT_SignInit(&mechInfo, sessionMacKey, AES_KEY_LEN_nBYTE, &hContext);
    //    nRet = HOST_CMAC_Init(&cmacCtx, sessionMacKey, AES_KEY_LEN_nBYTE);
    if (nRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    /*
     * For the EXTERNAL AUTHENTICATE command MAC verification, the "MAC chaining value" is set to 16
     * bytes '00'. (SCP03 spec p16)
     */
    SCP_HostLocal_ResetMacChainingValue(channelId);
    memcpy(mcv, scpState[stateIdx].session.mcv, AES_KEY_LEN_nBYTE);

    nRet = HLCRYPT_SignUpdate(hContext, mcv, AES_KEY_LEN_nBYTE);
    //    nRet = HOST_CMAC_Update(cmacCtx, mcv, AES_KEY_LEN_nBYTE);
    nRet &= HLCRYPT_SignUpdate(hContext, txBuf, 13);
    //    nRet &= HOST_CMAC_Update(cmacCtx, txBuf, 13);
    nRet &= HLCRYPT_SignFinal(hContext, macToAdd, &signatureLen);
    //    nRet &= HOST_CMAC_Finish(cmacCtx, macToAdd);
    if (nRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    memcpy(scpState[stateIdx].session.mcv, macToAdd, AES_KEY_LEN_nBYTE);

    memcpy(&txBuf[5+SCP_GP_IU_CARD_CRYPTOGRAM_LEN], macToAdd, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    PRINT_BYTE_STRING("sessionMacKey", sessionMacKey, 16);
    PRINT_BYTE_STRING("mcv", mcv, 16);
    PRINT_BYTE_STRING("txBuf", txBuf, 13);
    PRINT_BYTE_STRING("macToAdd", macToAdd, 16);

#ifdef TGT_EDEV
    if (channelId == AX_HOST_CHANNEL)
    {
        // Modify the CLA byte to tag this channel as a Host Channel
        cla = 0xE4;
    }
    txBuf[0] = cla;
#endif

    st = smCom_TransceiveRaw((U8*)txBuf, 5 + AES_KEY_LEN_nBYTE, response, &responseLen);

    if (st != SMCOM_OK)
    {
        EPRINTF("SCP_GP_ExternalAuthenticate %lX\r\n", st);
        rv = ERR_GENERAL_ERROR;
    }
    else
    {
        rv =  CheckNoResponseDataRaw(response, (U16)responseLen);
    }
    return rv;
}

U16 SCP_GP_InitializeUpdate(ChannelId_t channelId, U8 *hostChallenge, U16 hostChallengeLen,
                            U8 *keyDivData, U16 *pKeyDivDataLen,
                            U8 *keyInfo, U16 *pKeyInfoLen,
                            U8 *cardChallenge, U16 *pCardChallengeLen,
                            U8 *cardCryptoGram, U16 *pCardCryptoGramLen,
                            U8 *seqCounter, U16 *pSeqCounterLen)
{
    U32 st = 0;
    U8 txBuf[128];
    U8 cla = 0;
    U8 keyVersion = 0;
    U8 response[128];
    U32 responseLen = 128;
    U16 parsePos = 0;
    U16 sw = 0;
    U32 iuResponseLenSmall = SCP_GP_IU_KEY_DIV_DATA_LEN +
            SCP_GP_IU_KEY_INFO_LEN +
            SCP_GP_CARD_CHALLENGE_LEN +
            SCP_GP_IU_CARD_CRYPTOGRAM_LEN +
            SCP_GP_SW_LEN;
    U32 iuResponseLenBig = SCP_GP_IU_KEY_DIV_DATA_LEN +
            SCP_GP_IU_KEY_INFO_LEN +
            SCP_GP_CARD_CHALLENGE_LEN +
            SCP_GP_IU_CARD_CRYPTOGRAM_LEN +
            SCP_GP_IU_SEQ_COUNTER_LEN +
            SCP_GP_SW_LEN;

    FPRINTF(">> %s: Enter\r\n", "SCP_GP_InitializeUpdate");

    assert(hostChallengeLen == SCP_GP_HOST_CHALLENGE_LEN);
    assert(*pKeyDivDataLen == SCP_GP_IU_KEY_DIV_DATA_LEN);
    assert(*pKeyInfoLen == SCP_GP_IU_KEY_INFO_LEN);
    assert(*pCardChallengeLen == SCP_GP_CARD_CHALLENGE_LEN);
    assert(*pCardCryptoGramLen == SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
#ifdef TGT_EDEV
        keyVersion = (U8) SST_HOST_SCP_KEYSET;
        cla = 0xE0;
#else
        keyVersion = (U8) (SST_HOST_SCP_KEYSET >> 8);
        cla = 0x80;
#endif
        break;
    case AX_ADMIN_CHANNEL:
        keyVersion = (U8) 0x00; // Just use default key version
        cla = 0x80;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    txBuf[0] = cla;
    txBuf[1] = INS_GP_INITIALIZE_UPDATE;
    txBuf[2] = keyVersion;
#ifdef TGT_EDEV
    txBuf[3] = SCP03_KEY_ID;
#else
    txBuf[3] = 0x00; // InitializeUpdate as implemented in Applet 1.6 still expects an identifier equal to 0.
#endif
    txBuf[4] = (U8)hostChallengeLen;

    // (GP.p205 Table D-4)
    memcpy(&txBuf[5], hostChallenge, hostChallengeLen);
    txBuf[5+hostChallengeLen] = 0x00;

    st = smCom_TransceiveRaw((U8*)txBuf, 6+hostChallengeLen, response, &responseLen);

    if (st != SMCOM_OK)
    {
        EPRINTF("SCP_GP_InitializeUpdate. Failure on communication Link (0x%04lX)\r\n", st);
        return (U16) st;
    }

    // Parse Response
    // The expected result length depends on random (HOST-Channel) or pseudo-random (ADMIN-Channel) challenge type.
    // The pseudo-random challenge case also includes a 3 byte sequence counter
    if ( (responseLen != iuResponseLenSmall) && (responseLen != iuResponseLenBig) )
    {
        // Note: A response of length 2 (a proper SW) is also collapsed into return code SCP_FAIL
        EPRINTF("Unexpected amount of data returned: %ld\r\n", responseLen);
        return SCP_FAIL;
    }

    memcpy(keyDivData, response, SCP_GP_IU_KEY_DIV_DATA_LEN);
    parsePos = SCP_GP_IU_KEY_DIV_DATA_LEN;
    memcpy(keyInfo, &(response[parsePos]), SCP_GP_IU_KEY_INFO_LEN);
    parsePos += SCP_GP_IU_KEY_INFO_LEN;
    memcpy(cardChallenge, &(response[parsePos]), SCP_GP_CARD_CHALLENGE_LEN);
    parsePos += SCP_GP_CARD_CHALLENGE_LEN;
    memcpy(cardCryptoGram, &(response[parsePos]), SCP_GP_IU_CARD_CRYPTOGRAM_LEN);
    parsePos += SCP_GP_IU_CARD_CRYPTOGRAM_LEN;
    if (responseLen == iuResponseLenBig)
    {
        memcpy(seqCounter, &(response[parsePos]), SCP_GP_IU_SEQ_COUNTER_LEN);
        *pSeqCounterLen = SCP_GP_IU_SEQ_COUNTER_LEN;
    }
    else
    {
        memset(seqCounter, 0, SCP_GP_IU_SEQ_COUNTER_LEN);
        *pSeqCounterLen = 0;
    }

    // Construct Return Value
    sw = (response[responseLen-2] << 8) + response[responseLen-1];
    return sw;
}

/**
* Utility function used by ::SCP_GP_PutKeys
* @param[in] keyType
* @param[in] key      Pointer to the key
  @param[in] currentKeyDek Pointer to the current Dek key, in case the pointer is NULL no encryption will be applied (deviates from GP spec to enable initial provisioning)
* @param[in,out] targetStore
* @param[in,out] nCryptoStatus
* @return Length
*/
static U8 createKeyDataField(U8 keyType, U8 *key, U8 *currentKeyDek, U8 *targetStore, S32 *nCryptoStatus)
{
    U8 refOneArray[AES_KEY_LEN_nBYTE] = {0};
    U8 refOneArrayCiphered[AES_KEY_LEN_nBYTE] = {0};

    U8 encKey[AES_KEY_LEN_nBYTE];

    if (currentKeyDek == NULL)
    {
        memcpy(encKey, key, AES_KEY_LEN_nBYTE);
        *nCryptoStatus = HOST_CRYPTO_OK;
    }
    else
    {
        HLSE_MECHANISM_INFO mechInfo;
        U32 outLen = sizeof(encKey);

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_AES_ECB_ENCRYPT;

        *nCryptoStatus = HLCRYPT_Encrypt(&mechInfo, currentKeyDek, AES_KEY_LEN_nBYTE, key, AES_KEY_LEN_nBYTE, encKey, &outLen);

//        *nCryptoStatus = HOST_AES_ECB_ENCRYPT(key /*plaintext*/, encKey /*ciphertext out*/, currentKeyDek /*key*/, AES_KEY_LEN_nBYTE /*keyLen*/);
    }

    targetStore[0] = keyType;
    targetStore[1] = AES_KEY_LEN_nBYTE+1; // Length of the 'AES key data'
    targetStore[2] = AES_KEY_LEN_nBYTE;   // Length of 'AES key'
    memcpy(&targetStore[3], encKey, AES_KEY_LEN_nBYTE);
    targetStore[3+AES_KEY_LEN_nBYTE] = CRYPTO_KEY_CHECK_LEN;

    memset(refOneArray, 1, sizeof(refOneArray));
    if (*nCryptoStatus == HOST_CRYPTO_OK)
    {
        HLSE_MECHANISM_INFO mechInfo;
        U32 outLen = sizeof(refOneArrayCiphered);

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_AES_ECB_ENCRYPT;

        *nCryptoStatus = HLCRYPT_Encrypt(&mechInfo, key, AES_KEY_LEN_nBYTE, refOneArray, AES_KEY_LEN_nBYTE, refOneArrayCiphered, &outLen);

        //*nCryptoStatus = HOST_AES_ECB_ENCRYPT(refOneArray, refOneArrayCiphered, key, AES_KEY_LEN_nBYTE);
    }

    // Append key check value
    memcpy(&targetStore[3+AES_KEY_LEN_nBYTE+1], &refOneArrayCiphered[0],
            CRYPTO_KEY_CHECK_LEN);
    return (3+AES_KEY_LEN_nBYTE+1+CRYPTO_KEY_CHECK_LEN);
}

/**
 * Persistently stores the provided SCP03 base key set in the security module.
 *
 * This method must be called once before the Host - Secure Module SCP channel can be established.
 *
 * @param[in] keyVersion
 * @param[in] keyEnc  SCP03 channel encryption base key
 * @param[in] keyMac  SCP03 authentication base key
 * @param[in] keyDek  SCP03 data encryption base key
 * @param[in] currentKeyDek Value of the data encryption base key already stored in secure module, may be NULL in case no key is currently stored.
 * @param[in] keyBytes  Length (in byte) of the keys being set. Typically 16 (corresponding to 128 bits)
 * @returns ::SW_OK upon success
 */
U16 SCP_GP_PutKeys(U8 keyVersion, U8 *keyEnc, U8 *keyMac, U8 *keyDek, U8 *currentKeyDek, U16 keyBytes)
{
    U32 st = 0;
    S32 cryptoStatus = HOST_CRYPTO_ERROR;
    U8 txBuf[128];
    U8 len = 0;
    U8 response[128];
    U32 responseLen = 128;
    U16 rv = 0;

    assert(keyBytes == AES_KEY_LEN_nBYTE);

#ifdef TGT_EDEV
    txBuf[0] = (keyVersion == SST_ADMIN_SCP_KEYSET) ? 0x80 : 0xE0;
#else
    txBuf[0] = 0x80;
#endif
    txBuf[1] = INS_GP_PUT_KEY;
#ifdef TGT_A71CH  // For A71CH the SCP03 keys can only be set once
    txBuf[2] = 0x00;
#else
    txBuf[2] = keyVersion;
#endif
    txBuf[3] = PUT_KEYS_KEY_IDENTIFIER;
    // txBuf[4] = len;  // Fill in length

    // First byte of data field is "New version number" (GP.p157 Table 11-67)
    txBuf[5+len] = keyVersion;
    len += 1;
    // Construct the key data fields
    len += createKeyDataField(PUT_KEYS_KEY_TYPE_CODING_AES, keyEnc, currentKeyDek, &txBuf[5+len], &cryptoStatus);
    if (cryptoStatus != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    len += createKeyDataField(PUT_KEYS_KEY_TYPE_CODING_AES, keyMac, currentKeyDek, &txBuf[5+len], &cryptoStatus);
    if (cryptoStatus != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    len += createKeyDataField(PUT_KEYS_KEY_TYPE_CODING_AES, keyDek, currentKeyDek, &txBuf[5+len], &cryptoStatus);
    if (cryptoStatus != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    txBuf[4] = len;
    txBuf[5+len] = 0x00;

    // In case there is a DEK key, proper command encryption and MACing shall be applied.
    if (currentKeyDek == NULL)
    {
        st = smCom_TransceiveRaw((U8*)txBuf, 6+len, response, &responseLen);

        if (st != SMCOM_OK)
        {
            EPRINTF("SCP_GP_PutKeys %lx\r\n", st);
            rv = (U16) st;
        }
        else
        {
            rv =  CheckNoResponseDataRaw(response, (U16)responseLen);
        }
    }
    else
    {
        apdu_t apdu;
        apdu_t * pApdu = (apdu_t *) &apdu;

        // Construct APDU
        pApdu->cla   = 0x80;
        pApdu->ins   = INS_GP_PUT_KEY;
        pApdu->p1    = keyVersion;
        pApdu->p2    = PUT_KEYS_KEY_IDENTIFIER;

        AllocateAPDUBuffer(pApdu);

        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        smApduAppendCmdData(pApdu, txBuf+5, len);

        rv = (U16)scp_Transceive(pApdu, SCP_MODE);
        if (rv == SMCOM_OK)
        {
            // No response data expected
            rv = CheckNoResponseData(pApdu);
        }
    }

    return rv;
}

#ifndef TGT_A71CH
/**
* Retrieves the sequence counter for the default SCP keyset
* @param channelId The channel identifier associated with the SCP keyset for which the sequence counter value will be retrieved
* @param counter   Array (at least of size 16) that will contain the retrieved sequence counter
* @return status.
*/
U16 SCP_GetSequenceCounter(ChannelId_t channelId, U8 *counter, U16 *counterLen)
{
    U8 defaultKeyVersion = 0x00;
    U16 rv;

    rv = SCP_GetSequenceCounterKeyVersion(channelId, defaultKeyVersion, counter, counterLen);
    return rv;
}

/**
* @function SCP_GetSequenceCounterKeyVersion
* @description Retrieves the sequence counter for the SCP keyset with the requested keyVersion
* @param channelId The channel identifier associated with the SCP keyset for which the sequence counter value will be retrieved
* @param keyVersion keyVersion (of the keyset) for which the sequence number must be retrieved
* @param counter   Array (at least of size 16) that will contain the retrieved sequence counter
* @return status.
*/
U16 SCP_GetSequenceCounterKeyVersion(ChannelId_t channelId, U8 keyVersion, U8 *counter, U16 *counterLen)
{
    SST_Identifier_t scpKeySet = SST_APDU_ADMIN_SCP_KEYSET;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv;
    U8 isOk = 0;
    U8 identifier = 0;
    U8 index = 0;

    assert(*counterLen>=16);

    switch (channelId)
    {
    case AX_HOST_CHANNEL:
        scpKeySet = SST_APDU_HOST_SCP_KEYSET;
        break;
    case AX_ADMIN_CHANNEL:
        scpKeySet = SST_APDU_ADMIN_SCP_KEYSET;
        break;
    default:
        assert (1==0);
        break;
    }

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SCP;
    pApdu->p1    = P1_GET_SEQ_COUNTER;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    identifier = (U8)scpKeySet;
    index = keyVersion;

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

    scp_Transceive(pApdu, SCP_MODE);

    rv = smGetSw(pApdu, &isOk);
    if (isOk)
    {
        U16 retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, counterLen, counter);

        if (retval != SW_OK)
        {
            rv = ERR_WRONG_RESPONSE;
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif

/**
 * Performs an SCP03 authentication with the SM and - when successful - computes
 * the SCP03 session keys and initializes the current Session state.
 *
 * @param[in] keyEnc SCP03 channel encryption base key (aka static key) (16 bytes)
 * @param[in] keyMac  SCP03 authentication base key (aka static key) (16 bytes)
 * @param[in] keyDek  SCP03 data encryption base key (aka static key) (16 bytes)
 * @param[in] keyBytes Must be 16
 * @param[in,out] sCounter SCP03 sequence counter (3 bytes)
 * @param[in,out] sCounterLen
 */
U16 SCP_Authenticate(U8 *keyEnc, U8 *keyMac, U8 *keyDek, U16 keyBytes, U8 *sCounter, U16 *sCounterLen)
{
    ChannelId_t channelId;
    U8 hostChallenge[] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};

    U8 keyDivData[SCP_GP_IU_KEY_DIV_DATA_LEN];
    U16 keyDivDataLen = sizeof(keyDivData);
    U8 keyInfo[SCP_GP_IU_KEY_INFO_LEN];
    U16 keyInfoLen = sizeof(keyInfo);
    U8 cardChallenge[SCP_GP_CARD_CHALLENGE_LEN];
    U16 cardChallengeLen = sizeof(cardChallenge);
    U8 cardCryptoGram[SCP_GP_IU_CARD_CRYPTOGRAM_LEN];
    U16 cardCryptoGramLen = sizeof(cardCryptoGram);
    U8 seqCounter[SCP_GP_IU_SEQ_COUNTER_LEN];
    U16 seqCounterLen = sizeof(seqCounter);
    U8 hostCryptogram[SCP_GP_IU_CARD_CRYPTOGRAM_LEN];

    scp_CommandType_t dummy;
    U16 err = 0;
    S32 ret;

    assert(keyBytes==16);

    channelId = DEV_GetSelectedChannel(&dummy);

    // Storing Static Keys
    err = SCP_HostLocal_SetKeysScp(channelId, keyEnc, keyMac, keyDek, keyBytes);
    if (err != SW_OK)
    {
        EPRINTF("SCP_HostLocal_SetKeysScp fails with status: 0x%04X\r\n", err);
        return err;
    }

    ret = HLCRYPT_GetRandom(sizeof(hostChallenge), hostChallenge);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }

    err = SCP_GP_InitializeUpdate(channelId, hostChallenge, sizeof(hostChallenge),
                                keyDivData, &keyDivDataLen,
                                keyInfo, &keyInfoLen,
                                cardChallenge, &cardChallengeLen,
                                cardCryptoGram, &cardCryptoGramLen,
                                seqCounter, &seqCounterLen);
    if (err != SW_OK)
    {
        EPRINTF("SCP_GP_InitializeUpdate fails with status: 0x%04X\r\n", err);
        return err;
    }
    PRINT_BYTE_STRING("keyDivData", keyDivData, keyDivDataLen);
    PRINT_BYTE_STRING("keyInfo", keyInfo, keyInfoLen);
    PRINT_BYTE_STRING("cardChallenge", cardChallenge, cardChallengeLen);
    PRINT_BYTE_STRING("cardCryptoGram", cardCryptoGram, cardCryptoGramLen);
    if (seqCounterLen == SCP_GP_IU_SEQ_COUNTER_LEN)
    {
        PRINT_BYTE_STRING("seqCounter", seqCounter, seqCounterLen);
        if (*sCounterLen >= SCP_GP_IU_SEQ_COUNTER_LEN)
        {
            // Enough buffer space is provided by caller
            memcpy(sCounter, seqCounter, seqCounterLen);
            *sCounterLen = seqCounterLen;
        }
        else
        {
            return SCP_PARAMETER_ERROR;
        }
    }
    else
    {
        *sCounterLen = 0;
    }

    err = SCP_HostLocal_CalculateSessionKeys(channelId, hostChallenge, cardChallenge);
    if (err != SW_OK)
    {
        EPRINTF("SCP_HostLocal_CalculateSessionKeys fails with status: 0x%04X\r\n", err);
        return err;
    }

    err =  SCP_HostLocal_VerifyCardCryptogram(channelId, hostChallenge, cardChallenge, cardCryptoGram);
    if (err != SW_OK)
    {
        EPRINTF("SCP_HostLocal_VerifyCardCryptogram fails with status: 0x%04X\r\n", err);
        return err;
    }

    err = SCP_HostLocal_CalculateHostCryptogram(channelId, hostChallenge, cardChallenge, hostCryptogram);
    PRINT_BYTE_STRING("hostCryptogram", hostCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);
    if (err != SW_OK)
    {
        EPRINTF("SCP_HostLocal_CalculateHostCryptogram fails with status: 0x%04X\r\n", err);
        return err;
    }

    err = SCP_GP_ExternalAuthenticate(channelId, hostCryptogram);
    if (err != SW_OK)
    {
        EPRINTF("SCP_GP_ExternalAuthenticate fails with status: 0x%04X\r\n", err);
        return err;
    }

    // At this stage we have authenticated successfully.
    SCP_HostLocal_SetDefaultValueIcvCCounter(channelId);
    DEV_SetChannelCommandType(channelId, C_MAC_C_ENC_R_MAC_R_ENC);

    return err;
}

#ifdef TGT_EDEV
#ifndef AX_SF
/**
* Performs a soft-reset of the Security Module, effectively resetting the SCP channels.
*               This function is only available for the administrator
* @return status.
*/
U16 SM_SoftReset(void)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SM;
    pApdu->p1    = P1_SOFT_RESET;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
    scp_Transceive(pApdu, SCP_MODE);

    // no response data expected
    rv = CheckNoResponseData(pApdu);

    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif // AX_SF
#endif // TGT_EDEV
