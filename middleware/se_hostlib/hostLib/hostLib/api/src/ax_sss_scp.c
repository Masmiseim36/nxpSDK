/**
* @file ax_scp.c
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
#include <nxLog_scp.h>
#include <nxEnsure.h>


#include "ax_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "HostCryptoAPI.h"
#include "ax_util.h"
#include "sm_printf.h"
#include "global_platf.h"
#include "ax_sss_scp.h"

#include <fsl_sss_api.h>

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_SCP_SCP03_SSS
static Scp_SSS_State_t scp_sss_State[2];
#endif
// #define FLOW_VERBOSE   //!< Define to enable execution flow messages
#define ERROR_VERBOSE  //!< Define to enable reporting of error messages


/**
* Performs an SCP03 authentication with the SM and - when successful - computes
* the SCP03 session keys and initializes the current Session state.
*
* @param[in] keyObj SCP03 channel encryption base key Objects (aka static key) (16 bytes)
* @param[in,out] sCounter SCP03 sequence counter (3 bytes)
* @param[in,out] sCounterLen
*/
#if SSS_HAVE_SCP_SCP03_SSS
U16 SCP_SSS_Authenticate(SM_SECURE_SCP03_KEYOBJ * scp03Params, U8 *sCounter, U16 *sCounterLen)
{
    ChannelId_t channelId;
    U8 hostChallenge[] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
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
    sss_rng_context_t rngctx;
    scp_CommandType_t dummy;
    U16 err = 0;
    sss_status_t status = kStatus_SSS_Fail;
    channelId = DEV_GetSelectedChannel(&dummy);


    status = sss_rng_context_init(&rngctx, scp03Params->pKeyEnc.keyStore->session /* Session */);
    if (status == kStatus_SSS_Success)
    {
        status = sss_rng_get_random(&rngctx, hostChallenge,8);
    }
    sss_rng_context_free(&rngctx);
    if (status != kStatus_SSS_Success)
        return ERR_CRYPTO_ENGINE_FAILED;

    err = SCP_GP_InitializeUpdate(channelId, hostChallenge, sizeof(hostChallenge),
        keyDivData, &keyDivDataLen,
        keyInfo, &keyInfoLen,
        cardChallenge, &cardChallengeLen,
        cardCryptoGram, &cardCryptoGramLen,
        seqCounter, &seqCounterLen);
    if (err != SW_OK)
    {
        LOG_E("SCP_GP_InitializeUpdate fails with status: 0x%04X\r\n", err);
        return err;
    }
    LOG_MAU8_D("keyDivData", keyDivData, keyDivDataLen);
    LOG_MAU8_D("keyInfo", keyInfo, keyInfoLen);
    LOG_MAU8_D("cardChallenge", cardChallenge, cardChallengeLen);
    LOG_MAU8_D("cardCryptoGram", cardCryptoGram, cardCryptoGramLen);
    if (seqCounterLen == SCP_GP_IU_SEQ_COUNTER_LEN)
    {
        LOG_MAU8_D("seqCounter", seqCounter, seqCounterLen);
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

    err = SCP_SSS_HostLocal_CalculateSessionKeys(channelId, scp03Params, hostChallenge, cardChallenge);
    if (err != SW_OK)
    {
        LOG_E("SCP_HostLocal_CalculateSessionKeys fails with status: 0x%04X\r\n", err);
        return err;
    }

    err = SCP_SSS_HostLocal_VerifyCardCryptogram(channelId, hostChallenge, cardChallenge, cardCryptoGram);
    if (err != SW_OK)
    {
        LOG_E("SCP_HostLocal_VerifyCardCryptogram fails with status: 0x%04X\r\n", err);
        return err;
    }

    err = SCP_SSS_HostLocal_CalculateHostCryptogram(channelId, hostChallenge, cardChallenge, hostCryptogram);
    LOG_MAU8_D("hostCryptogram", hostCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);
    if (err != SW_OK)
    {
        LOG_E("SCP_HostLocal_CalculateHostCryptogram fails with status: 0x%04X\r\n", err);
        return err;
    }

    err = SCP_SSS_GP_ExternalAuthenticate(channelId, hostCryptogram);
    if (err != SW_OK)
    {
        LOG_E("SCP_GP_ExternalAuthenticate fails with status: 0x%04X\r\n", err);
        return err;
    }

    // At this stage we have authenticated successfully.
    SCP_SSS_HostLocal_SetDefaultValueIcvCCounter(channelId);
    DEV_SetChannelCommandType(channelId, C_MAC_C_ENC_R_MAC_R_ENC);

    return err;
}

U16 SCP_SSS_HostLocal_CalculateSessionKeys(ChannelId_t channelId,SM_SECURE_SCP03_KEYOBJ *scp03Params, U8 *hostChallenge, U8 *cardChallenge)
{
    int stateIdx = 0;
    U8 ddA[128];
    U16 ddALen = sizeof(ddA);
    U8 context[128];
    U16 contextLen = 0;
    U8 sessionEncKey[AES_KEY_LEN_nBYTE];
    U8 sessionMacKey[AES_KEY_LEN_nBYTE];
    U8 sessionRmacKey[AES_KEY_LEN_nBYTE];
    U32 signatureLen = sizeof(sessionMacKey);
    S32 ret;
    sss_status_t status;

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

    LOG_I("HOST: Calculate session keys\r\n");

    // Calculate the Derviation data
    memcpy(context, hostChallenge, SCP_GP_HOST_CHALLENGE_LEN);
    memcpy(&context[SCP_GP_HOST_CHALLENGE_LEN], cardChallenge, SCP_GP_CARD_CHALLENGE_LEN);
    contextLen = SCP_GP_HOST_CHALLENGE_LEN + SCP_GP_CARD_CHALLENGE_LEN;

    // Calculate the Session-ENC key
    DEV_setDataDerivationArray(ddA, &ddALen,
        DATA_DERIVATION_SENC, DATA_DERIVATION_L_128BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);
    ret = SCP_SSS_CalculateCMAC(scp03Params->pKeyEnc, ddA, ddALen, sessionEncKey, &signatureLen);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }
    LOG_MAU8_D("sessionEncKey", sessionEncKey, AES_KEY_LEN_nBYTE);
    /* Init ENC  session key object */
    status = sss_key_object_init(&scp_sss_State[stateIdx].session.sEnckeyObj, scp03Params->pKeyEnc.keyStore);
    if (status != kStatus_SSS_Success)
    {
        return SCP_FAIL;
    }
    ret = SCP_SSS_Save_SessionKeyObj(&scp_sss_State[stateIdx].session.sEnckeyObj, sessionEncKey);
    if (ret == HOST_CRYPTO_ERROR)
    {
        return SCP_FAIL;
    }

    // Calculate the Session-MAC key
    DEV_setDataDerivationArray(ddA, &ddALen,
        DATA_DERIVATION_SMAC, DATA_DERIVATION_L_128BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);
    ret = SCP_SSS_CalculateCMAC(scp03Params->pKeyMac, ddA, ddALen, sessionMacKey, &signatureLen);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }
    LOG_MAU8_D("sessionMacKey", sessionMacKey, AES_KEY_LEN_nBYTE);
    /* Init MAC  session key object */
    status = sss_key_object_init(&scp_sss_State[stateIdx].session.sMackeyObj, scp03Params->pKeyMac.keyStore);
    if (status != kStatus_SSS_Success)
    {
        return SCP_FAIL;
    }
    ret = SCP_SSS_Save_SessionKeyObj(&scp_sss_State[stateIdx].session.sMackeyObj, sessionMacKey);
    if (ret == HOST_CRYPTO_ERROR)
    {
        return SCP_FAIL;
    }

    // Calculate the Session-RMAC key
    DEV_setDataDerivationArray(ddA, &ddALen,
        DATA_DERIVATION_SRMAC, DATA_DERIVATION_L_128BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);
    ret = SCP_SSS_CalculateCMAC(scp03Params->pKeyMac, ddA, ddALen, sessionRmacKey, &signatureLen);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }
    LOG_MAU8_D("sessionRmacKey", sessionRmacKey, AES_KEY_LEN_nBYTE);
    /* Init RMAC  session key object */
    status = sss_key_object_init(&scp_sss_State[stateIdx].session.sRMackeyObj, scp03Params->pKeyDek.keyStore);
    if (status != kStatus_SSS_Success)
    {
        return SCP_FAIL;
    }
    ret = SCP_SSS_Save_SessionKeyObj(&scp_sss_State[stateIdx].session.sRMackeyObj, sessionRmacKey);
    if (ret == HOST_CRYPTO_ERROR)
    {
        return SCP_FAIL;
    }

    return SCP_OK;
}

HLSE_RET_CODE   SCP_SSS_CalculateCMAC(sss_object_t keyObj,
    U8* inData, U32 inDataLen,
    U8* outSignature, U32* outSignatureLen)
{
    sss_mac_t macCtx;
    sss_status_t status;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    algorithm = kAlgorithm_SSS_CMAC_AES;
    mode = kMode_SSS_Mac;
    HLSE_RET_CODE ret = HOST_CRYPTO_ERROR;

    // Init Context
    status = sss_mac_context_init(
        &macCtx, keyObj.keyStore->session, &keyObj, algorithm, mode);

    if (status == kStatus_SSS_Success)
    {
        // Calculate CMAC
        status = sss_mac_one_go(
            &macCtx, inData, (size_t)inDataLen,
            (uint8_t*)outSignature, (size_t*)outSignatureLen);
    }
    if (status == kStatus_SSS_Success)
    {
        ret = HOST_CRYPTO_OK;
    }
    return ret;
}

U16 SCP_SSS_Save_SessionKeyObj(sss_object_t *keyObj, U8 *Sessionkey)
{
    sss_status_t status;
    static int i = 0;
    U16 ret = HOST_CRYPTO_ERROR;
    /* Allocate handle for SSS object */
    status = sss_key_object_allocate_handle(keyObj,
        0xAA58 + i,
        kSSS_KeyPart_Default,
        kSSS_CipherType_AES,
        16,
        kKeyObject_Mode_Transient);
    if (status == kStatus_SSS_Success) {
        /* Set the key */
        status = sss_key_store_set_key(
            keyObj->keyStore, keyObj, Sessionkey, 16, (16) * 8, NULL, 0);
    }
    if (status == kStatus_SSS_Success) {
        /* Save the key store */
        status = sss_key_store_save(keyObj->keyStore);
    }

    if (status == kStatus_SSS_Success) {
        i++;
        ret = HOST_CRYPTO_OK;
    }
    return ret;
}

U16 SCP_SSS_HostLocal_VerifyCardCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge ,U8 *cardCryptogram)
{
    int stateIdx = 0;
    U8 ddA[128];
    U16 ddALen = sizeof(ddA);
    U8 context[128];
    U16 contextLen = 0;
    U8 cardCryptogramFullLength[AES_KEY_LEN_nBYTE];
    U16 rv = SCP_OK;
    U32 signatureLen = sizeof(cardCryptogramFullLength);
    S32 ret;

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

    LOG_I("HOST: Verify Card Cryptogram\r\n");

    memcpy(context, hostChallenge, SCP_GP_HOST_CHALLENGE_LEN);
    memcpy(&context[SCP_GP_HOST_CHALLENGE_LEN], cardChallenge, SCP_GP_CARD_CHALLENGE_LEN);
    contextLen = SCP_GP_HOST_CHALLENGE_LEN + SCP_GP_CARD_CHALLENGE_LEN;

    DEV_setDataDerivationArray(ddA, &ddALen,
        DATA_CARD_CRYPTOGRAM, DATA_DERIVATION_L_64BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);

     ret = SCP_SSS_CalculateCMAC(scp_sss_State[stateIdx].session.sMackeyObj, ddA, ddALen, cardCryptogramFullLength, &signatureLen);
    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }

    LOG_MAU8_D("cardCryptogramFullLength - Verify", cardCryptogramFullLength, AES_KEY_LEN_nBYTE);

    // Verify whether the 8 left most byte of cardCryptogramFullLength match cardCryptogram
    if (memcmp(cardCryptogramFullLength, cardCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN) != 0)
    {
        rv = SCP_CARD_CRYPTOGRAM_FAILS_TO_VERIFY;
    }

    return rv;
}

U16 SCP_SSS_HostLocal_CalculateHostCryptogram(ChannelId_t channelId, U8 *hostChallenge, U8 *cardChallenge, U8 *hostCryptogram)
{
    int stateIdx = 0;
    U8 ddA[128];
    U16 ddALen = sizeof(ddA);
    U8 context[128];
    U16 contextLen = 0;
    U8 hostCryptogramFullLength[AES_KEY_LEN_nBYTE];
    U32 signatureLen = sizeof(hostCryptogramFullLength);
    S32 ret;

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

    LOG_I("HOST: Calculate Host Cryptogram\r\n");

    memcpy(context, hostChallenge, SCP_GP_HOST_CHALLENGE_LEN);
    memcpy(&context[SCP_GP_HOST_CHALLENGE_LEN], cardChallenge, SCP_GP_CARD_CHALLENGE_LEN);
    contextLen = SCP_GP_HOST_CHALLENGE_LEN + SCP_GP_CARD_CHALLENGE_LEN;

    DEV_setDataDerivationArray(ddA, &ddALen,
        DATA_HOST_CRYPTOGRAM, DATA_DERIVATION_L_64BIT, DATA_DERIVATION_KDF_CTR, context, contextLen);

    ret = SCP_SSS_CalculateCMAC(scp_sss_State[stateIdx].session.sMackeyObj, ddA, ddALen, hostCryptogramFullLength, &signatureLen);

    if (ret != HOST_CRYPTO_OK) { return ERR_CRYPTO_ENGINE_FAILED; }

    // Chop of the tail of the hostCryptogramFullLength
    memcpy(hostCryptogram, hostCryptogramFullLength, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    return SCP_OK;
}

U16 SCP_SSS_GP_ExternalAuthenticate(ChannelId_t channelId, U8* hostCryptogram)
{
    U32 st = 0;
    U8 txBuf[128];
    U8 cla = 0;
    U8 response[128];
    U32 responseLen = 128;
    U16 rv = 0;
    U8 mcv[AES_KEY_LEN_nBYTE];
    U8 macToAdd[AES_KEY_LEN_nBYTE];
    int stateIdx = 0;// ADMIN_CHANNEL_STATE_IDX;
    sss_mac_t macCtx;
    sss_algorithm_t algorithm;
    sss_mode_t mode;
    algorithm = kAlgorithm_SSS_CMAC_AES;
    mode = kMode_SSS_Mac;
    size_t signatureLen = sizeof(macToAdd);
    sss_status_t status;

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

    LOG_I(">> %s: Enter\r\n", "SCP_GP_ExternalAuthenticate");

    cla = 0x84;

    txBuf[0] = cla;
    txBuf[1] = INS_GP_EXTERNAL_AUTHENTICATE;
    txBuf[2] = SECLVL_CDEC_RENC_CMAC_RMAC;
    txBuf[3] = 0x00;
    txBuf[4] = 0x10; // The Lc valus is set as-if the MAC has already been appended (SCP03 spec p16. Fig.6-1)
    memcpy(&txBuf[5], hostCryptogram, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    // Calculate the MAC value
    status = sss_mac_context_init(
        &macCtx, scp_sss_State[stateIdx].session.sMackeyObj.keyStore->session,
        &scp_sss_State[stateIdx].session.sMackeyObj, algorithm, mode);

    if (status != kStatus_SSS_Success)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    status = sss_mac_init(&macCtx);
    if (status != kStatus_SSS_Success)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    /*
    * For the EXTERNAL AUTHENTICATE command MAC verification, the "MAC chaining value" is set to 16
    * bytes '00'. (SCP03 spec p16)
    */
    SCP_SSS_HostLocal_ResetMacChainingValue(channelId);
    memcpy(mcv, scp_sss_State[stateIdx].session.mcv, AES_KEY_LEN_nBYTE);

    status = sss_mac_update( &macCtx, mcv, AES_KEY_LEN_nBYTE);
    if (status != kStatus_SSS_Success)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    status = sss_mac_update(&macCtx, txBuf, 13);
    if (status != kStatus_SSS_Success)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    status = sss_mac_finish(&macCtx, macToAdd, &signatureLen);
    if (status != kStatus_SSS_Success)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    memcpy(scp_sss_State[stateIdx].session.mcv, macToAdd, AES_KEY_LEN_nBYTE);

    memcpy(&txBuf[5 + SCP_GP_IU_CARD_CRYPTOGRAM_LEN], macToAdd, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);

    LOG_MAU8_D("mcv", mcv, 16);
    LOG_MAU8_D("txBuf", txBuf, 13);
    LOG_MAU8_D("macToAdd", macToAdd, 16);

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
        LOG_E("SCP_GP_ExternalAuthenticate %lX\r\n", st);
        rv = ERR_GENERAL_ERROR;
    }
    else
    {
        rv = CheckNoResponseDataRaw(response, (U16)responseLen);
    }
    return rv;
}


//State maintain functions (scp_sss_State)

U16 SCP_SSS_HostLocal_SetDefaultValueIcvCCounter(ChannelId_t channelId)
{
    U8 commandCounter[16] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 };
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

    memcpy(scp_sss_State[stateIdx].session.cCounter, commandCounter, 16);

    return SCP_OK;
}


U16 SCP_SSS_HostLocal_IncIcvCCounter(ChannelId_t channelId)
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
        if (scp_sss_State[stateIdx].session.cCounter[i] < 255)
        {
            scp_sss_State[stateIdx].session.cCounter[i] += 1;
            break;
        }
        else
        {
            scp_sss_State[stateIdx].session.cCounter[i] = 0;
            i--;
        }
    }

    return SCP_OK;
}

U16 SCP_SSS_HostLocal_ResetMacChainingValue(ChannelId_t channelId)
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

    memset(scp_sss_State[stateIdx].session.mcv, 0, SCP_MCV_LEN);

    return SCP_OK;
}

U16 SCP_SSS_HostLocal_SetMacChainingValue(ChannelId_t channelId, U8 *mcv)
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

    memcpy(scp_sss_State[stateIdx].session.mcv, mcv, SCP_MCV_LEN);

    return SCP_OK;
}

/**
* Copy the session state into \p pSession. Caller must allocate memory of \p pSession.
* @param[in] channelId Either ::AX_HOST_CHANNEL or ::AX_ADMIN_CHANNEL. Must be ::AX_HOST_CHANNEL in case of A71CH.
* @param[in,out] pSession IN: pointer to allocated ::Scp03SessionState_t structure; OUT: retrieved state
* @retval ::SW_OK Upon successful execution
* @retval ::SCP_UNDEFINED_CHANNEL_ID In case an undefined ::ChannelId_t type was passed as parameter
*/
U16 SCP_SSS_HostLocal_GetSessionState(ChannelId_t channelId, Scp03_SSS_SessionState_t *pSession)
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

    memcpy(pSession, &(scp_sss_State[stateIdx].session), sizeof(Scp03_SSS_SessionState_t));

    return SCP_OK;
}
U16 SM_ChannelAuthenticate(SM_SECURE_SCP03_KEYOBJ *scp03Params, U8 *cCounter)
{
    U16 counterLen = 16;
    U16 lReturn = SCP_FAIL;

    lReturn = SCP_SSS_Authenticate(scp03Params, cCounter, &counterLen);

    if (lReturn != SW_OK)
        return lReturn;


    return lReturn;
}

#endif

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
