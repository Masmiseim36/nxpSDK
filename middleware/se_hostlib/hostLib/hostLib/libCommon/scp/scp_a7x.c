/**
 * @file scp_a7x.c
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
 * This file defines A7-series specific types
 * @par History
 *
 * Originally, this file was scp.c, but now renaming it to scp_a7x.c
 *
 */

#include <string.h>
#include "scp.h"
#include "smCom.h"
#include "sm_printf.h"
#include "sm_apdu.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_A71XX || SSS_HAVE_SE050_EAR || SSS_HAVE_SE050_L

#if !defined(TGT_A70CI) && !defined(TGT_A70CM) && !defined(NO_SECURE_CHANNEL_SUPPORT)
#define SECURE_CHANNEL_SUPPORTED
#endif

#ifdef SECURE_CHANNEL_SUPPORTED
#include "ax_util.h"
#include "HostCryptoAPI.h"
//#include "axHostCrypto.h"
#endif // SECURE_CHANNEL_SUPPORTED

#include <stdio.h>
#include <assert.h>
#ifdef LINUX
#if !defined(__APPLE__)
#include <malloc.h>
#endif
#endif // LINUX

#ifdef DBG_VERBOSE
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
#define DPRINTF(...) printf (__VA_ARGS__)
#define PRINT_BYTE_STRING(A, B, C) localPrintByteArray(A, B, C)
#else
#define DPRINTF(...)
#define PRINT_BYTE_STRING(A, B, C)
#endif

#define SCP_BUFFER_SIZE         (MAX_CHUNK_LENGTH_LINK+34) // TODO: What is the exact size (instead of 34) that must be added as margin

#ifdef SECURE_CHANNEL_SUPPORTED
/// @cond
#define CMAC_SIZE                   (8)

static S32 scp_GetCommandICV(ChannelId_t channelId, U8* pIcv);
static U32 scp_TransformCommand(apdu_t * pApdu);
static U32 scp_TransformResponse(apdu_t *pApdu);
static U16 scp_PadData(apdu_t * pApdu);
static U16 scp_AddMacToCommand(ChannelId_t channelId, apdu_t * pApdu);
static SCP_SignalFunction scp_SignalFunctionCb = NULL;
/// @endcond

U16 SCP_Subscribe(SCP_SignalFunction callback, void *context)
{
    AX_UNUSED_ARG(context);
    scp_SignalFunctionCb = callback;
    return SCP_OK;
}

#endif


U32 scp_Transceive(apdu_t * pApdu, scp_CommandType_t type)
{
    U32 rv = SMCOM_OK;

#ifdef SECURE_CHANNEL_SUPPORTED
    scp_CommandType_t channelCommandType;
#ifdef TGT_EDEV
    ChannelId_t channelId = DEV_GetSelectedChannel(&channelCommandType);
#else //TGT_EDEV
    DEV_GetSelectedChannel(&channelCommandType);
#endif //TGT_EDEV

    // Even if C_MAC or C_ENC is requested, but the channel has not been set up successfully,
    // it will not be granted.
    if (type != NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC)
    {
        type = channelCommandType;
    }

    // Transform command
    if (type != NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC)
    {
        rv = scp_TransformCommand(pApdu);
        if (rv != SW_OK)
        {
            pApdu->rxlen = 0;
            return rv;
        }
    }
    else
    {
        // scp_TransformCommand (if branch) already invokes smApduAdaptLc
        smApduAdaptLc(pApdu, pApdu->lc);
    }
    smApduAdaptLe(pApdu, 0);
#ifdef TGT_EDEV
    // EDEV Specific: Conditionally adapt CLA byte to label the command as a HOST channel command (as opposed to ADMIN channel)
    if (channelId == AX_HOST_CHANNEL)
    {
        pApdu->pBuf[0] |= 0xE0;
    }
#endif //TGT_EDEV
#else //SECURE_CHANNEL_SUPPORTED
    smApduAdaptLcLe(pApdu, pApdu->lc, 0);
#endif //SECURE_CHANNEL_SUPPORTED

    rv = smCom_Transceive(pApdu);

#ifdef SECURE_CHANNEL_SUPPORTED
    // transform response
    if ( (type != NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC) && (rv == SMCOM_OK) )
    {
        rv = scp_TransformResponse(pApdu);
        if (rv != SMCOM_OK)
        {
            pApdu->rxlen = 0;
        }
    }
#endif // SECURE_CHANNEL_SUPPORTED

    return rv;
}

#ifdef SECURE_CHANNEL_SUPPORTED
/*
 * scp_TransformCommand
 *
 * SCPP03 spec, section 6.2.6
 */

static U32 scp_TransformCommand(apdu_t *pApdu)
{
    S32 nRet = HOST_CRYPTO_OK;
    U16 rv;
    U16 Lcc = 0;
    U8 iv[16];
    U8* pIv = (U8*) iv;
    scp_CommandType_t ignoreChannelCommandTypeAtThisLevel;

    ChannelId_t channelId = DEV_GetSelectedChannel(&ignoreChannelCommandTypeAtThisLevel);

    U8 apduPayloadToEncrypt[SCP_BUFFER_SIZE];

    Scp03SessionState_t session;

    // Get the current security level. (should be set to AUTH_C_DECRYPTION_R_ENCRYPTION in the GP_ExternalAuthenticate command)

    /* The Off-Card Entity first encrypts the Command Data field and then computes the C-MAC on the
       command with the ciphered data field as described in section 6.2.4 APDU Command C-MAC Generation
       and Verification. No encryption shall be applied to a command where there is no command data field. */

    // Patch CLA byte to indicate we are using SCP03
    pApdu->pBuf[0] |= 0x04;

    if (pApdu->hasData ==  1)
    {
        int payloadOffset = 0;

        /* Prior to encrypting the data, the data shall be padded as defined in section 4.1.4. This padding becomes part of the data field.*/
        // DPRINTF("pApdu->lc: %d\r\n", pApdu->lc);
        Lcc = pApdu->lc;
        Lcc += scp_PadData(pApdu);
        // DPRINTF("Lcc: %d (@line %d)\r\n", Lcc, __LINE__);

        /* The final Lc value (Lcc) is the sum of: initial Lc + length of the padding + length of C-MAC */
        Lcc += CMAC_SIZE;
        // DPRINTF("Lcc: %d (@line %d)\r\n", Lcc, __LINE__);

        smApduAdaptLc(pApdu, Lcc);

        nRet = scp_GetCommandICV(channelId, pIv);
        if (nRet == HOST_CRYPTO_OK)
        {
            SCP_HostLocal_GetSessionState(channelId, &session);

            if (pApdu->extendedLength)
            {
                payloadOffset = APDU_OFFSET_LC + 3; // (3 bytes reserved for LC field)
            }
            else
            {
                payloadOffset = APDU_OFFSET_LC + 1; // (1 byte reserved for LC field)
            }
            memcpy(apduPayloadToEncrypt, &(pApdu->pBuf[payloadOffset]), (pApdu->buflen - payloadOffset));
            DPRINTF("SCP: Encrypting %d byte\r\n", pApdu->buflen - payloadOffset);


            {
                HLSE_MECHANISM_INFO mechInfo;
                U32 outLen = SCP_KEY_SIZE;

                memset(&mechInfo, 0, sizeof(mechInfo));
                mechInfo.mechanism = HLSE_AES_CBC_ENCRYPT;
                mechInfo.pParameter = pIv;
                mechInfo.ulParameterLen = 16;

                nRet = HLCRYPT_Encrypt(&mechInfo,
                                        session.sEnc, SCP_KEY_SIZE,
                                        apduPayloadToEncrypt, pApdu->buflen - payloadOffset,
                                        &(pApdu->pBuf[payloadOffset]), &outLen);
            }
//            nRet = HOST_AES_CBC_Process(session.sEnc, SCP_KEY_SIZE, pIv, HOST_ENCRYPT, apduPayloadToEncrypt,
//                pApdu->buflen - payloadOffset, &(pApdu->pBuf[payloadOffset]));
        }
    }
    else // Handle commands with no command data field.
    {
        /* spec:
         * The length of the command message (Lc) shall be incremented by 8 to indicate the inclusion of the
         * C-MAC in the data field of the command message.
         */
        Lcc = pApdu->lc + CMAC_SIZE;
        // The MAC will become the payload of the APDU. so indicate there is a datapayload
        pApdu->hasData = 1;
        pApdu->lcLength = 1;
        pApdu->buflen += 1;
        smApduAdaptLc(pApdu, Lcc);
    }

    if (nRet == HOST_CRYPTO_OK)
    {
        // Add the MAC
        rv = scp_AddMacToCommand(channelId, pApdu);
    }
    else
    {
        rv = ERR_CRYPTO_ENGINE_FAILED;
    }
    return rv;
}

/*
 * scp_PadData
 *
 * (spec 4.1.4) Unless specified otherwise, padding prior to performing an AES operation across a block of data is
 * achieved in the following manner:
 * - Append an '80' to the right of the data block;
 * - If the resultant data block length is a multiple of 16, no further padding is required;
 * - Append binary zeroes to the right of the data block until the data block length is a multiple of 16.
 */
static U16 scp_PadData(apdu_t * pApdu)
{
    U16 zeroBytesToPad = 0;
    U16 bytesToPad = 1;

    // pad the payload and adjust the length of the APDU
    if (pApdu->extendedLength == false)
    {
        // TODO: remove hard coded values
        if (pApdu->buflen > 5) // payload present => padding needed
        {
            pApdu->pBuf[pApdu->buflen++] = 0x80;

            zeroBytesToPad = (SCP_KEY_SIZE - ((pApdu->buflen - 5) % SCP_KEY_SIZE)) % SCP_KEY_SIZE;
        }
    }
    else
    {
        if (pApdu->buflen > 7) // payload present => padding needed
        {
            pApdu->pBuf[pApdu->buflen++] = 0x80;
            zeroBytesToPad = (SCP_KEY_SIZE - ((pApdu->buflen - 7) % SCP_KEY_SIZE)) % SCP_KEY_SIZE;
        }
    }

    bytesToPad += zeroBytesToPad;

    while (zeroBytesToPad > 0)
    {
        pApdu->pBuf[pApdu->buflen++] = 0x00;
        zeroBytesToPad--;
    }

    pApdu->offset = pApdu->buflen;

    // DPRINTF("Padded %d bytes.\r\n", bytesToPad);
    return bytesToPad;
}

/*
 * scp_AddMacToCommand
 *
 */
static U16 scp_AddMacToCommand(ChannelId_t channelId, apdu_t *pApdu)
{
    S32 nRet;
    Scp03SessionState_t session;
    U8 macToAdd[16];
    //axHcCmacCtx_t *cmacCtx;
    HLSE_CONTEXT_HANDLE hContext;

    HLSE_MECHANISM_INFO mechInfo;
    U32 signatureLen = sizeof(macToAdd);

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_AES_CMAC;


    SCP_HostLocal_GetSessionState(channelId, &session);

    nRet = HLCRYPT_SignInit(&mechInfo, session.sMac, SCP_KEY_SIZE, &hContext);
//    nRet = HOST_CMAC_Init(&cmacCtx, session.sMac, SCP_KEY_SIZE);
    if (nRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    nRet = HLCRYPT_SignUpdate(hContext, session.mcv, SCP_KEY_SIZE);
//    nRet = HOST_CMAC_Update(cmacCtx, session.mcv, SCP_KEY_SIZE);
    nRet &= HLCRYPT_SignUpdate(hContext, pApdu->pBuf, pApdu->buflen);
//    nRet &= HOST_CMAC_Update(cmacCtx, pApdu->pBuf, pApdu->buflen);
    nRet &= HLCRYPT_SignFinal(hContext, macToAdd, &signatureLen);
//    nRet &= HOST_CMAC_Finish(cmacCtx, macToAdd);
    if (nRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    // Store updated mcv!
    SCP_HostLocal_SetMacChainingValue(channelId, macToAdd);

    memcpy(&(pApdu->pBuf[pApdu->buflen]), macToAdd, SCP_GP_IU_CARD_CRYPTOGRAM_LEN);
    pApdu->buflen += SCP_GP_IU_CARD_CRYPTOGRAM_LEN;
    pApdu->offset = pApdu->buflen;
    return SW_OK;
}

/*
 * scp_GetCommandICV
 *
 * The ICV shall be calculated as follows:
 * - A counter shall be incremented for each command (i.e. for each pair of command and response
 *   APDU) within a secure channel session.
 * - The starting value shall be 1 for the first command after a successful EXTERNAL AUTHENTICATE.
 * - The binary counter value shall be left padded with zeroes to form a full block.
 * - This block shall be encrypted with S-ENC; the result shall be used as ICV.
*/
static S32 scp_GetCommandICV(ChannelId_t channelId, U8* pIcv)
{
    S32 nRet;
    U8 ivZero[SCP_KEY_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    Scp03SessionState_t session;

    SCP_HostLocal_GetSessionState(channelId, &session);
    PRINT_BYTE_STRING("SCP: SndCmdCounter", session.cCounter, SCP_KEY_SIZE);

    {
        HLSE_MECHANISM_INFO mechInfo;
        U32 outLen = SCP_KEY_SIZE;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_AES_CBC_ENCRYPT;
        mechInfo.pParameter = ivZero;
        mechInfo.ulParameterLen = SCP_KEY_SIZE;

        nRet = HLCRYPT_Encrypt(&mechInfo,
            session.sEnc, SCP_KEY_SIZE,
            session.cCounter, SCP_KEY_SIZE,
            pIcv, &outLen);
    }
//    nRet = HOST_AES_CBC_Process(session.sEnc, SCP_KEY_SIZE, ivZero, HOST_ENCRYPT, session.cCounter, SCP_KEY_SIZE, pIcv);
    return nRet;
}

static S32 scp_GetResponseICV(ChannelId_t channelId, U8* pIcv)
{
    S32 nRet;
    U8 ivZero[SCP_KEY_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    U8 commandCounter[SCP_KEY_SIZE];
    Scp03SessionState_t session;

    SCP_HostLocal_GetSessionState(channelId, &session);
    memcpy(commandCounter, session.cCounter, SCP_KEY_SIZE);
    commandCounter[0] = 0x80; // Section 6.2.7 of SCP03 spec
    PRINT_BYTE_STRING("SCP: RcvCmdCounter", commandCounter, 16);

    {
        HLSE_MECHANISM_INFO mechInfo;
        U32 outLen = SCP_KEY_SIZE;

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_AES_CBC_ENCRYPT;
        mechInfo.pParameter = ivZero;
        mechInfo.ulParameterLen = SCP_KEY_SIZE;

        nRet = HLCRYPT_Encrypt(&mechInfo,
            session.sEnc, SCP_KEY_SIZE,
            commandCounter, sizeof(commandCounter),
            pIcv, &outLen);
    }
//    nRet = HOST_AES_CBC_Process(session.sEnc, SCP_KEY_SIZE, ivZero, HOST_ENCRYPT, commandCounter, sizeof(commandCounter), pIcv);
    return nRet;
}

static U32 scp_RemovePaddingRestoreSw(apdu_t *pApdu, U8 *plaintextResponse, U16 plaintextResponseLen, U8 *sw)
{
    int i;
    int removePaddingOk = 0;

    i = plaintextResponseLen;

    while ( (i > 1) && (i > (plaintextResponseLen-SCP_KEY_SIZE)) )
    {
        if (plaintextResponse[i-1] == 0x00)
        {
            i--;
        }
        else if (plaintextResponse[i-1] == 0x80)
        {
            // We have found padding delimitor
            memcpy(&plaintextResponse[i-1], sw, SCP_GP_SW_LEN);
            memcpy(pApdu->pBuf, plaintextResponse, i+1);
            pApdu->rxlen = (U16)(i+1);
            removePaddingOk = 1;
            PRINT_BYTE_STRING("SCP: plaintextResponseStripped+SW", pApdu->pBuf, pApdu->rxlen);
            break;
        }
        else
        {
            // We've found a non-padding character while removing padding
            // Most likely the cipher text was not properly decoded.
            DPRINTF("Decoding failed.\r\n");
            break;
        }
    }

    if (removePaddingOk == 0)
    {

        DPRINTF("Invoking callback. 'Found illegal (or no) padding'\r\n");
        DPRINTF("**************************************************\r\n");
        if (scp_SignalFunctionCb != NULL)
        {
            scp_SignalFunctionCb(SCP_WRONG_PADDING, NULL);
        }

        return SCP_DECODE_FAIL;
    }
    return SMCOM_OK;
}

/**
 * @brief scp_TransformResponse
 * @param pApdu
 * @retval ::SMCOM_OK
 * @retval ::SCP_RSP_MAC_FAIL
 * @retval ::SCP_DECODE_FAIL
 */
static U32 scp_TransformResponse(apdu_t *pApdu)
{
    U32 status = SMCOM_OK;
    U8 iv[16];
    U8* pIv = (U8*) iv;
    scp_CommandType_t dummy;

    ChannelId_t channelId = DEV_GetSelectedChannel(&dummy);
    Scp03SessionState_t session;

    U8 response[SCP_BUFFER_SIZE];
    U8 plaintextResponse[SCP_BUFFER_SIZE];
    U8 sw[SCP_GP_SW_LEN] = {0};
    U8 pMac[16];
    //axHcCmacCtx_t *cmacCtx;
    HLSE_CONTEXT_HANDLE hContext;

    HLSE_MECHANISM_INFO mechInfo;
    U32 signatureLen = sizeof(pMac);

    memset(&mechInfo, 0, sizeof(mechInfo));
    mechInfo.mechanism = HLSE_AES_CMAC;

    SCP_HostLocal_GetSessionState(channelId, &session);

    if (pApdu->rxlen >= 10)
    {
        S32 nRet = 1;
        memcpy(sw, &(pApdu->pBuf[pApdu->rxlen-SCP_GP_SW_LEN]), SCP_GP_SW_LEN);

        // get the MAC chaining value
        nRet = HLCRYPT_SignInit(&mechInfo, session.sRMac, SCP_KEY_SIZE, &hContext);
//        nRet = HOST_CMAC_Init(&cmacCtx, session.sRMac, SCP_KEY_SIZE);
        if (nRet != HOST_CRYPTO_OK)
        {
            return ERR_CRYPTO_ENGINE_FAILED;
        }

        nRet = HLCRYPT_SignUpdate(hContext, session.mcv, SCP_CMAC_SIZE);
//        nRet = HOST_CMAC_Update(cmacCtx, session.mcv, SCP_CMAC_SIZE);
        if (pApdu->rxlen > 10)
        {
            nRet &= HLCRYPT_SignUpdate(hContext, pApdu->pBuf, pApdu->rxlen - SCP_COMMAND_MAC_SIZE - SCP_GP_SW_LEN);
//            nRet &= HOST_CMAC_Update(cmacCtx, pApdu->pBuf, pApdu->rxlen - SCP_COMMAND_MAC_SIZE - SCP_GP_SW_LEN);
        }

        nRet &= HLCRYPT_SignUpdate(hContext, sw, SCP_GP_SW_LEN);
//        nRet &= HOST_CMAC_Update(cmacCtx, sw, SCP_GP_SW_LEN);
        nRet &= HLCRYPT_SignFinal(hContext, pMac, &signatureLen);
//        nRet &= HOST_CMAC_Finish(cmacCtx, pMac);
        PRINT_BYTE_STRING("SCP: Calculated Response Mac", pMac, SCP_CMAC_SIZE);
        if (nRet != HOST_CRYPTO_OK)
        {
            return ERR_CRYPTO_ENGINE_FAILED;
        }

        // Do a comparison of the received and the calculated mac
        if ( memcmp(pMac, &pApdu->pBuf[pApdu->rxlen-SCP_COMMAND_MAC_SIZE-SCP_GP_SW_LEN], SCP_COMMAND_MAC_SIZE) != 0 )
        {
            DPRINTF("Invoking callback. 'RESPONSE MAC DID NOT VERIFY!'\r\n");
            DPRINTF("*************************************************\r\n");
            if (scp_SignalFunctionCb != NULL)
            {
                scp_SignalFunctionCb(SCP_WRONG_RESPMAC, NULL);
            }
            return SCP_RSP_MAC_FAIL;
        }
    }

    // Decrypt Response Data Field in case Reponse Mac verified OK
    if (pApdu->rxlen > 10)
    {
        S32 nRet;
        memcpy(response, pApdu->pBuf, pApdu->rxlen - 10);
        PRINT_BYTE_STRING("SCP: EncResponse (MAC and SW stripped)", response, pApdu->rxlen - 10);

        memcpy(sw, &(pApdu->pBuf[pApdu->rxlen-SCP_GP_SW_LEN]), SCP_GP_SW_LEN);
        // PRINT_BYTE_STRING("sw", sw, 2);

        nRet = scp_GetResponseICV(channelId, pIv);
        if (nRet != HOST_CRYPTO_OK)
        {
            return ERR_CRYPTO_ENGINE_FAILED;
        }

        {
            U32 outLen = SCP_BUFFER_SIZE;

            memset(&mechInfo, 0, sizeof(mechInfo));
            mechInfo.mechanism = HLSE_AES_CBC_DECRYPT;
            mechInfo.pParameter = pIv;
            mechInfo.ulParameterLen = SCP_KEY_SIZE;

            nRet = HLCRYPT_Decrypt(&mechInfo,
                session.sEnc, SCP_KEY_SIZE,
                response, pApdu->rxlen - 10,
                plaintextResponse, &outLen);
        }
//        nRet = HOST_AES_CBC_Process(session.sEnc, SCP_KEY_SIZE, pIv, HOST_DECRYPT, response, pApdu->rxlen - 10, plaintextResponse);
        if (nRet != HOST_CRYPTO_OK)
        {
            return ERR_CRYPTO_ENGINE_FAILED;
        }
        PRINT_BYTE_STRING("SCP: Plaintext Response", plaintextResponse, pApdu->rxlen - 10);

        // Remove the padding from the plaintextResponse
        status = scp_RemovePaddingRestoreSw(pApdu, plaintextResponse, pApdu->rxlen - 10, sw);
    }
    else if (pApdu->rxlen == 10)
    {
        // There's no data payload in response
        memcpy(pApdu->pBuf, sw, SCP_GP_SW_LEN);
        pApdu->rxlen = SCP_GP_SW_LEN;
    }
    else
    {
        // We're receiving a response with an unexpected response length
        DPRINTF("SCP: Unexpected Response Length: %d\r\n", pApdu->rxlen);
    }
    SCP_HostLocal_IncIcvCCounter(channelId);
    return status;
}

#endif /* SSS_HAVE_A71XX || SSS_HAVE_SE050_EAR */

#endif // SECURE_CHANNEL_SUPPORTED
