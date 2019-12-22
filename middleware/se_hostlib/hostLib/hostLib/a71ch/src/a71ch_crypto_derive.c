/**
* @file a71ch_crypto_derive.c
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
* This file wraps the key derivation functionality of the A71CH module.
* @par History
* 1.0   2016-Sep-22 : Initial version
*
*****************************************************************************/
#include <stddef.h>
#include <string.h>
#include <stdio.h> // DEBUG

#include "scp.h"
#include "a71ch_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include <nxLog_App.h>
#include "nxEnsure.h"

/// @cond
static U16 DERIVE_KdfGeneric(SST_Index_t index, U8 nBlock, const U8 *salt, U16 saltLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen)
{
    U8 isOk;
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 minBufLen = derivedDataLen;
    U8 data = 0x00;

#ifndef A71_IGNORE_PARAM_CHECK
    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(((info != NULL) && (derivedData != NULL)), rv, ERR_API_ERROR)
#endif


    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(infoLen <= DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_INFO, rv, ERR_API_ERROR);

    ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(derivedDataLen <= DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_DERIVED_DATA, rv, ERR_API_ERROR);

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_KDF_SYM_KEY;
    pApdu->p1    = (nBlock << 4) + index;
    pApdu->p2    = (U8)derivedDataLen;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    if (saltLen != 0)
    {
        if (salt == NULL) {
            FreeAPDUBuffer(pApdu);
            return ERR_API_ERROR;
        }
        data = (U8)saltLen;
        smApduAppendCmdData(pApdu, &data, 1);
        smApduAppendCmdData(pApdu, salt, saltLen);
    }
    data = (U8)infoLen;
    smApduAppendCmdData(pApdu, &data, 1);
    smApduAppendCmdData(pApdu, info, infoLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, derivedData, &minBufLen);
            if (rv == SW_OK)
            {
                if (minBufLen != derivedDataLen)
                {
                    rv = ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
/// @endcond

/**
* The HMAC Key Derivation function derives a key from a stored secret using SHA256 as hash function according to [RFC5869].
  Only the expand step will be executed.

  The secret is stored in the SYM key store. It can be either 16, 32, 48 or 64 byte long.
  The Most Significant part of the secret resides in the storage location with the lowest index.
  The subsequent parts reside in the next storage locations. The nBlock parameter is equal to the length
  of the secret divided by 16.
  A secret with length 64 can only start at Index 0 of the SYM key store: a secret can not be stored wrapped around in the
  SYM key store.

  \note infoLen must be smaller than 254 byte.

* \param[in] index Index of the SYM key store containing the MSB part of the pre-shared secret
* \param[in] nBlock Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
* \param[in] info Context and application specific information used in expand step
* \param[in] infoLen The length of the info data passed as argument
* \param[in,out] derivedData IN: caller passes a buffer of at least derivedDataLen;
     OUT: contains the calculated derived data
* \param[in] derivedDataLen IN: length of the requested derivedData. Must be smaller than 256 byte.
*
* \retval ::SW_OK Successfull execution
*/
U16 A71_HkdfExpandSymKey(SST_Index_t index, U8 nBlock, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen)
{
    U8 *salt = NULL;
    U16 saltLen = 0;
    return DERIVE_KdfGeneric(index, nBlock, salt, saltLen, info, infoLen, derivedData, derivedDataLen);
}

/**
* The HMAC Key Derivation function derives a key from a stored secret using SHA256 as hash function according to [RFC5869].
  Both the extract and expand steps will be executed.

  In case a zero length salt value is passed as argument, this function is equivalent to A71_HkdfExpandSymKey:
  i.e. the extract step is skipped. To enforce the usage of the default salt value (a Bytestring of 32 zeroes)
  the caller must explicitly pass this default salt value as argument to this function.

  The secret is stored in the SYM key store. It can be either 16, 32, 48 or 64 byte long.
  The Most Significant part of the secret resides in the storage location with the lowest index.
  The subsequent parts reside in the next storage locations. The nBlock parameter is equal to the length
  of the secret divided by 16.
  A secret with length 64 can only start at Index 0 of the SYM key store: a secret can not be stored wrapped around in the
  SYM key store.

  \note The sum of saltLen and infoLen must be smaller than 254 byte.

* \param[in] index Index of the SYM key store containing the MSB part of the pre-shared secret
* \param[in] nBlock Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
* \param[in] salt Salt data used in extract step
* \param[in] saltLen The length of the salt data passed as argument
* \param[in] info Context and application specific information used in expand step
* \param[in] infoLen The length of the info data passed as argument
* \param[in,out] derivedData IN: caller passes a buffer of at least derivedDataLen;
     OUT: contains the calculated derived data
* \param[in] derivedDataLen IN: length of the requested derivedData. Must be smaller than 256 byte.
*
* \retval ::SW_OK Successfull execution
*/
U16 A71_HkdfSymKey(SST_Index_t index, U8 nBlock, const U8 *salt, U16 saltLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen)
{
    return DERIVE_KdfGeneric(index, nBlock, salt, saltLen, info, infoLen, derivedData, derivedDataLen);
}

/**
* This function calculates the PRF according to TLS1.2 [RFC5246]. The pre-master secret is formed - based upon
  a pre-shared secret (PSK) stored in the secure module - according to [RFC4279].

  The pre-shared secret is stored in the SYM key store. It can be either 16, 32, 48 or 64 byte long.
  The Most Significant part of the pre-shared secret resides in the storage location with the lowest index.
  The subsequent parts reside in the next storage locations. The nBlock parameter is equal to the length
  of the PSK divided by 16.

  A PSK cannot be stored wrapped around in the SYM key store.

  The PRF creating the masterSecret also takes as parameter the concatentation of label ("master_secret"), ClientHello.random and ServerHello.random.
  This function only takes ServerHello.random as parameter: ClientHello.random has already been set by a call to ::A71_CreateClientHelloRandom,
  the value of the label (default is "master_secret") can be overruled by a call to ::A71_SetTlsLabel.

  \pre This call must be preceded by a call to ::A71_CreateClientHelloRandom, no other A71CH API call (implying
  an APDU exchange between Host and A71CH) may be executed in between the invocation of ::A71_CreateClientHelloRandom
  and ::A71_PskDeriveMasterSecret

* \param[in] index Index of the SYM key store containing the MSB part of the pre-shared secret
* \param[in] nBlock Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
* \param[in] serverHelloRnd ServerHello.random (concatenated with values already contained in A71CH)
* \param[in] serverHelloRndLen The length of serverHelloRnd passed as an argument
* \param[in,out] masterSecret IN: caller passes a buffer of at least 48 byte; OUT: contains the calculated master Secret, TLS 1.2 mandates this to be 48 byte exact
*
* \retval ::SW_OK Successfull execution
*/
U16 A71_PskDeriveMasterSecret(SST_Index_t index, U8 nBlock, const U8 *serverHelloRnd, U16 serverHelloRndLen, U8 *masterSecret)
{
    U8 isOk;
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 minBufLen = AX_TLS_PSK_MASTER_SECRET_LEN;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((serverHelloRnd == NULL) || (masterSecret == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    if ( nBlock > A71CH_SYM_KEY_COMBINED_MAX)
    {
        return ERR_API_ERROR;
    }

    if (serverHelloRndLen > A71CH_MAX_CMD_PAYLOAD_SIZE)
    {
        return ERR_API_ERROR;
    }

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_MS_PSK_SYM_KEY;
    pApdu->p1    = (nBlock << 4) + index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, serverHelloRnd, serverHelloRndLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, masterSecret, &minBufLen);
            if (rv == SW_OK)
            {
                if (minBufLen != AX_TLS_PSK_MASTER_SECRET_LEN)
                {
                    rv = ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}


/**
* This function calculates the PRF according to TLS1.2 [RFC5246]. The pre-master secret is formed - based upon
  a pre-shared secret (PSK) stored in the secure module and on an ECDH calculation - according to [RFC5489].

  The pre-shared secret is stored in the SYM key store. It can be either 16, 32, 48 or 64 byte long.
  The Most Significant part of the pre-shared secret resides in the storage location with the lowest index.
  The subsequent parts reside in the next storage locations. The nBlock parameter is equal to the length
  of the PSK divided by 16.

  A PSK cannot be stored wrapped around in the SYM key store.

  The PRF creating the masterSecret also takes as parameter the concatentation of label ("master_secret"), ClientHello.random and ServerHello.random.
  This function only takes ServerHello.random as parameter: ClientHello.random has already been set by a call to ::A71_CreateClientHelloRandom,
  the value of the label (default is "master_secret") can be overruled by a call to ::A71_SetTlsLabel.

  \pre This call must be preceded by a call to ::A71_CreateClientHelloRandom, no other A71CH API call (implying
  an APDU exchange between Host and A71CH) may be executed in between the invocation of ::A71_CreateClientHelloRandom
  and ::A71_EcdhPskDeriveMasterSecret


* \param[in] indexKp Index of the ECC keypair whose private key is used in the ECDH operation
* \param[in] publicKey Value of the public key to be used in ECDH operation
* \param[in] publicKeyLen Length of publicKey in byte
* \param[in] index Index of the SYM key store containing the MSB part of the pre-shared secret
* \param[in] nBlock Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
* \param[in] serverHelloRnd ServerHello.random (concatenated with values already contained in A71CH)
* \param[in] serverHelloRndLen The length of serverHelloRnd passed as an argument
* \param[in,out] masterSecret IN: caller passes a buffer of at least 48 byte; OUT: contains the calculated master Secret, TLS 1.2 mandates this to be 48 byte exact
*
* \retval ::SW_OK Successfull execution
*/
U16 A71_EcdhPskDeriveMasterSecret(SST_Index_t indexKp, const U8 *publicKey, U16 publicKeyLen,
    SST_Index_t index, U8 nBlock, const U8 *serverHelloRnd, U16 serverHelloRndLen, U8 *masterSecret)
{
    U8 isOk;
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 minBufLen = AX_TLS_PSK_MASTER_SECRET_LEN;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((publicKey == NULL) || (serverHelloRnd == NULL) || (masterSecret == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    if ( nBlock > A71CH_SYM_KEY_COMBINED_MAX)
    {
        return ERR_API_ERROR;
    }

    if ( (serverHelloRndLen + publicKeyLen) > A71CH_MAX_CMD_PAYLOAD_SIZE)
    {
        return ERR_API_ERROR;
    }

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_MS_ECDH_PSK_ECC_KEYPAIR;
    pApdu->p1    = indexKp;
    pApdu->p2    = (nBlock << 4) + index;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, publicKey, publicKeyLen);
    smApduAppendCmdData(pApdu, serverHelloRnd, serverHelloRndLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, masterSecret, &minBufLen);
            if (rv == SW_OK)
            {
                if (minBufLen != AX_TLS_PSK_MASTER_SECRET_LEN)
                {
                    rv = ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Calculates the HMAC on \p data using SHA256 as Hash Function according to [RFC2104].
  The secret is stored in the SYM key store. It can be either 16, 32, 48 or 64 byte long.
  The Most Significant part of the secret resides in the storage location with the lowest index.
  The subsequent parts reside in the next storage locations. The nBlock parameter is equal to the length
  of the secret divided by 16.
  A secret with length 64 can only start at Index 0 of the SYM key store: a secret can not be stored wrapped around in the
  SYM key store.
* \param[in] index Index of the SYM key store containing the MSB part of the pre-shared secret
* \param[in] nBlock Amount of blocks, equivalent to the pre-shared secret length when multiplied by 16
* \param[in] data Data buffer for which the HMAC-SHA256 must be calculated
* \param[in] dataLen The length of data passed as argument
* \param[in,out] hmac IN: caller passes a buffer of at least 32 byte;
     OUT: contains the calculated hmac
* \param[in,out] hmacLen IN: length of the hmac buffer passed;
     OUT: because SHA256 is used this is 32 byte exact
*
* \retval ::SW_OK Successfull execution
*/
U16 A71_GetHmacSha256(SST_Index_t index, U8 nBlock, const U8 *data, U16 dataLen, U8 *hmac, U16 *hmacLen)
{
    U8 isOk = 0;
    U16 rv;
    // U8 payload = 0x00;
    U16 maxChunk = 0;
    U16 remainingData = dataLen;
    U16 toSend = 0;
    U16 dataOffset = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((data == NULL) || (hmac == NULL) || (hmacLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_HMAC_SHA256_SYM_KEY;
    pApdu->p1    = (nBlock << 4) + index;
    pApdu->p2    = P2_HMAC_ONE_SHOT;

    AllocateAPDUBuffer(pApdu);

    // Assumes SCP03 is used
    maxChunk = A71CH_HMAC_SHA256_MAX_DATA_CHUNK;

    // Depending on dataLen we can do a one-shot HMAC or need to split up across multiple APDU's
    if (dataLen <= maxChunk)
    {
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
        smApduAppendCmdData(pApdu, data, dataLen);

        rv = (U16)scp_Transceive(pApdu, SCP_MODE);
        if (rv == SMCOM_OK)
        {
            rv = smGetSw(pApdu, &isOk);
            if (isOk)
            {
                rv = smApduGetResponseBody(pApdu, hmac, hmacLen);
                if (rv == SW_OK)
                {
                    if (32 != *hmacLen)
                    {
                        rv = ERR_WRONG_RESPONSE;
                    }
                }
            }
        }
    }
    else
    {
        // Split up in multiple transactions ...
        // rv = ERR_NOT_IMPLEMENTED;
        // goto LBL_LEAVE_A71_GetHmacSha256;

        // Send out first chunk (HMAC_SHA256_Init)
        pApdu->p2    = P2_HMAC_INIT;
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
        toSend = maxChunk;
        remainingData -= toSend;
        smApduAppendCmdData(pApdu, data, toSend);
        dataOffset += toSend;

        rv = (U16)scp_Transceive(pApdu, SCP_MODE);
        if (rv == SMCOM_OK)
        {
            if (rv == SMCOM_OK)
            {
                // No response data expected
                rv = CheckNoResponseData(pApdu);
                if (rv != SW_OK) { goto LBL_LEAVE_A71_GetHmacSha256; }
            }
        }

        while (remainingData > maxChunk)
        {
            // Send out 'middle' chunks (HMAC_SHA256_Update)
            pApdu->p2    = P2_HMAC_UPDATE;
            SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
            toSend = maxChunk;
            remainingData -= toSend;
            smApduAppendCmdData(pApdu, &data[dataOffset], toSend);
            dataOffset += toSend;

            rv = (U16)scp_Transceive(pApdu, SCP_MODE);
            if (rv == SMCOM_OK)
            {
                if (rv == SMCOM_OK)
                {
                    // No response data expected
                    rv = CheckNoResponseData(pApdu);
                    if (rv != SW_OK) { goto LBL_LEAVE_A71_GetHmacSha256; }
                }
            }
        }

        // Always close with HMAC_SHA256_Final
        pApdu->p2    = P2_HMAC_FINAL;
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
        toSend = remainingData;
        // remainingData -= toSend;
        // dataOffset += toSend;
        smApduAppendCmdData(pApdu, &data[dataOffset], toSend);

        rv = (U16)scp_Transceive(pApdu, SCP_MODE);
        {
            rv = smGetSw(pApdu, &isOk);
            if (isOk)
            {
                rv = smApduGetResponseBody(pApdu, hmac, hmacLen);
                if (rv == SW_OK)
                {
                    if (32 != *hmacLen)
                    {
                        rv = ERR_WRONG_RESPONSE;
                    }
                }
            }
        }
    }

LBL_LEAVE_A71_GetHmacSha256:
    FreeAPDUBuffer(pApdu);
    return rv;
}
