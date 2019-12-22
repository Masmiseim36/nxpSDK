/**
* @file a71ch_module.c
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
* This file wraps the module centric APDU functionality of the A71CH module.
* @par History
* 1.0   2016-aug-29 : Initial version
*
*/
#include <stdio.h>
#include <string.h>

#include "scp.h"
#include "a71ch_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "ax_common_private.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#define NX_LOG_ENABLE_HOSTLIB_DEBUG 1
#include <nxLog_hostLib.h>

/// @cond
static U16 A71_GetChallengeGeneric(U8 challengeType, U8 *challenge, U16 *challengeLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((challenge == NULL) || (challengeLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_MODULE;
    pApdu->p1    = challengeType;
    pApdu->p2    = P2_CHALLENGE;

    if (*challengeLen < A71CH_MODULE_UNLOCK_CHALLENGE_LEN)
    {
        return ERR_BUF_TOO_SMALL;
    }

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, challenge, challengeLen);
            if (*challengeLen != A71CH_MODULE_UNLOCK_CHALLENGE_LEN)
            {
                rv = ERR_WRONG_RESPONSE;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

static U16 A71_GetRandomGeneric(U8 *random, U8 randomLen, U8 mode)
{
    U8 isOk = 0;
    U16 rv;
    U16 randomBufLen = randomLen;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (random == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_MODULE_GET_RANDOM;
    pApdu->p1    = mode;
    pApdu->p2    = (mode == P1_RANDOM_PLAIN) ? randomLen : 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, random, &randomBufLen);
            if (rv == SW_OK)
            {
                if (randomBufLen != randomLen)
                {
                    rv = ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/// @endcond

/**
* Get credential info from Module (in raw format)
* @param[in,out] map
* @param[in,out] mapLen
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GetCredentialInfo(U8 *map, U16 *mapLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((map == NULL) || (mapLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_CREDENTIAL_MAP;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, 0);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, map, mapLen);
        }
        else
        {
            *mapLen = 0;
        }
    }
    else
    {
        *mapLen = 0;
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Get info on Module
* @param[out] selectResponse     Encodes applet revision and whether Debug Mode is available
* @param[out] debugOn            Equals 0x01 when the Debug Mode is available
* @param[out] restrictedKpIdx    Either the index of the restricted keypair or ::A71XX_NO_RESTRICTED_KP
* @param[out] transportLockState The value retieved is one of ::A71XX_TRANSPORT_LOCK_STATE_LOCKED,
*   A71XX_TRANSPORT_LOCK_STATE_UNLOCKED or A71XX_TRANSPORT_LOCK_STATE_ALLOW_LOCK
* @param[out] scpState           The value retrieved is on of ::A71XX_SCP_MANDATORY, ::A71XX_SCP_NOT_SET_UP
*   or ::A71XX_SCP_KEYS_SET
* @param[out] injectLockState The value retrieved is one of ::A71XX_INJECT_LOCK_STATE_LOCKED or ::A71XX_INJECT_LOCK_STATE_UNLOCKED
* @param[out] gpStorageSize   Total storage size (in byte) of the General Purpose data store
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GetModuleInfo(U16 *selectResponse, U8 *debugOn, U8 *restrictedKpIdx, U8 *transportLockState, U8 *scpState, U8 *injectLockState, U16 *gpStorageSize)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk;
    U8 data[64];
    U16 dataLen = sizeof(data);

#ifndef A71_IGNORE_PARAM_CHECK
    if ((selectResponse == NULL) || (debugOn == NULL) || (restrictedKpIdx == NULL) || (transportLockState == NULL) ||
        (scpState == NULL) || (injectLockState == NULL) || (gpStorageSize == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_APPLET_INFO;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    *selectResponse = 0;
    *debugOn = 0;
    *restrictedKpIdx = 0;
    *transportLockState = 0;
    *injectLockState = 0;
    *gpStorageSize = 0;

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, data, &dataLen);
            if (dataLen == AX_MODULE_INFO_RESPONSE_LEN)
            {
                *selectResponse = (data[0] << 8) + data[1];
                *debugOn = *selectResponse & 0x01;
                *restrictedKpIdx = (data[2] & 0xF0) >> 4;
                *transportLockState = data[2] & 0x0F;
                *scpState = (data[3] & 0xF0) >> 4;
                *injectLockState = data[3] & 0x0F;
                *gpStorageSize = (data[4] << 8) + data[5];
            }
            else
            {
                rv = ERR_WRONG_RESPONSE;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}


/**
* Get Unique Identifier from the Secure Module
* @param[in,out] uid IN: buffer to contain uid; OUT: uid retrieved from Secure Module
* @param[in,out] uidLen IN: Size of buffer provided (at least ::A71CH_MODULE_UNIQUE_ID_LEN byte);
    OUT: length of retrieved unique identifier (expected to be ::A71CH_MODULE_UNIQUE_ID_LEN byte)
* @retval ::SW_OK Upon successful execution
* @retval ::ERR_WRONG_RESPONSE In case an identifier with a length different from ::A71CH_MODULE_UNIQUE_ID_LEN was retrieved
*/
U16 A71_GetUniqueID(U8 *uid, U16 *uidLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((uid == NULL) || (uidLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_UNIQUE_ID;

    if (*uidLen < A71CH_MODULE_UNIQUE_ID_LEN)
    {
        return ERR_BUF_TOO_SMALL;
    }

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, uid, uidLen);
            if (*uidLen != A71CH_MODULE_UNIQUE_ID_LEN)
            {
                rv = ERR_WRONG_RESPONSE;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
 * Get cert uid from the Secure Module. The cert uid is a subset of the Secure Module Unique Identifier
 * @param[in,out] certUid IN: buffer to contain cert uid; OUT: cert uid retrieved from Secure Module
 * @param[in,out] certUidLen IN: Size of buffer provided (at least ::A71XX_MODULE_CERT_UID_LEN byte);
 * OUT: length of retrieved unique identifier (expected to be ::A71XX_MODULE_CERT_UID_LEN byte)
 *
 * @retval ::SW_OK Upon successful execution
 * @retval ::ERR_WRONG_RESPONSE In case the Secure Module Unique Identifier (i.e. the base uid) did not have the expected length
 */
U16 A71_GetCertUid(U8 *certUid, U16 *certUidLen)
{
    U16 rv = 0;
    U8 uid[A71CH_MODULE_UNIQUE_ID_LEN] = {0};
    U16 uidLen = A71CH_MODULE_UNIQUE_ID_LEN;
    int idx = 0;

    if (*certUidLen < A71CH_MODULE_CERT_UID_LEN)
    {
        return ERR_BUF_TOO_SMALL;
    }

    rv = A71_GetUniqueID(uid, &uidLen);
    if (rv == SMCOM_OK)
    {
        idx = 0;
        certUid[idx++] = uid[A71CH_UID_IC_TYPE_OFFSET];
        certUid[idx++] = uid[A71CH_UID_IC_TYPE_OFFSET + 1];
        certUid[idx++] = uid[A71CH_UID_IC_FABRICATION_DATA_OFFSET];
        certUid[idx++] = uid[A71CH_UID_IC_FABRICATION_DATA_OFFSET + 1];
        certUid[idx++] = uid[A71CH_UID_IC_SERIAL_NR_OFFSET];
        certUid[idx++] = uid[A71CH_UID_IC_SERIAL_NR_OFFSET + 1];
        certUid[idx++] = uid[A71CH_UID_IC_SERIAL_NR_OFFSET + 2];
        certUid[idx++] = uid[A71CH_UID_IC_BATCH_ID_OFFSET];
        certUid[idx++] = uid[A71CH_UID_IC_BATCH_ID_OFFSET + 1];
        certUid[idx++] = uid[A71CH_UID_IC_BATCH_ID_OFFSET + 2];
        *certUidLen = A71CH_MODULE_CERT_UID_LEN;
    }
    else
    {
        *certUidLen = 0;
    }

    return rv;
}

/**
* Get Unlock challenge from the Secure Module
* @param[in,out] challenge IN: buffer to contain challenge; OUT: challenge retrieved from Secure Module
* @param[in,out] challengeLen IN: Size of buffer provided (at least ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN byte);
    OUT: length of retrieved unique identifier (must be ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN byte)
* @retval ::SW_OK Upon successful execution
* @retval ::ERR_WRONG_RESPONSE In case an identifier with a length different from ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN was retrieved
*/
U16 A71_GetUnlockChallenge(U8 *challenge, U16 *challengeLen)
{
    return A71_GetChallengeGeneric(P1_MODULE_UNLOCK_CHALLENGE, challenge, challengeLen);
}

/**
* Get Unlock challenge for a Keypair
* @param[in,out] challenge IN: buffer to contain challenge; OUT: challenge retrieved from Secure Module
* @param[in,out] challengeLen IN: Size of buffer provided (at least ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN byte);
    OUT: length of retrieved unique identifier (must be ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN byte)
* @retval ::SW_OK Upon successful execution
* @retval ::ERR_WRONG_RESPONSE In case an identifier with a length different from ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN was retrieved
*/
U16 A71_GetKeyPairChallenge(U8 *challenge, U16 *challengeLen)
{
    return A71_GetChallengeGeneric(P1_KEYPAIR_CHALLENGE, challenge, challengeLen);
}
/**
* Get Unlock challenge for a Public Key
* @param[in,out] challenge IN: buffer to contain challenge; OUT: challenge retrieved from Secure Module
* @param[in,out] challengeLen IN: Size of buffer provided (at least ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN byte);
    OUT: length of retrieved unique identifier (must be ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN byte)
* @retval ::SW_OK Upon successful execution
* @retval ::ERR_WRONG_RESPONSE In case an identifier with a length different from ::A71CH_MODULE_UNLOCK_CHALLENGE_LEN was retrieved
*/
U16 A71_GetPublicKeyChallenge(U8 *challenge, U16 *challengeLen)
{
    return A71_GetChallengeGeneric(P1_PUBLIC_KEY_CHALLENGE, challenge, challengeLen);
}

/**
* Retrieves a random byte array of size randomLen from the Secure Module.
* The maximum amount of data that can be retrieved depends on
* whether an authenticated channel (SCP03) has been set up.
* In case SCP03 has been set up, this (worst-case) maximum is ::A71XX_SCP03_MAX_PAYLOAD_SIZE
* @param[in,out] random  IN: buffer to contain random value (at least of size randomLen);
                         OUT: retrieved random data
* @param[in] randomLen Amount of byte to retrieve
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GetRandom(U8 *random, U8 randomLen)
{
    return A71_GetRandomGeneric(random, randomLen, P1_RANDOM_PLAIN);
}

/**
* Updates a 32 byte random value inside the A71CH and returns this value to the caller.
* \post A71CH is in a state it will accept ::A71_PskDeriveMasterSecret or ::A71_EcdhPskDeriveMasterSecret as an API call.
*
* @param[in,out] clientHello  IN: buffer to contain random value (at least of size randomLen);
                              OUT: retrieved random data
* @param[in] clientHelloLen Amount of byte to retrieve (must be equal to ::AX_TLS_PSK_HELLO_RANDOM_LEN)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_CreateClientHelloRandom(U8 *clientHello, U8 clientHelloLen)
{
    if (clientHelloLen != AX_TLS_PSK_HELLO_RANDOM_LEN)
    {
        return ERR_API_ERROR;
    }
    return A71_GetRandomGeneric(clientHello, clientHelloLen, P1_RANDOM_CLIENT_HELLO);
}

/**
 * Get the index of the restricted key pair (\p idx) together with the number of modifiable blocks
 * (\p nBlocks) in the locked GP storage area that is associated with the restricted key pair.
 * Detailed info on block offset and block length is contained in the \p blockInfo byte array.
 * Per block 2 bytes indicate the offset into GP storage and two bytes indicate the length of
 * the modifiable block.
 *
 * @param[out] idx Index of restricted key pair. ::A71XX_NO_RESTRICTED_KP in case there is no restricted key pair
 * @param[out] nBlocks Number of modifiable blocks
 * @param[in,out] blockInfo IN: Storage to contain blockInfo; OUT: Raw info on block offset and block lenght per block.
 * @param[in,out] blockInfoLen IN: Size of blockInfo (in byte); OUT: effective size of blockInfo
 * @retval ::SW_OK Upon successful execution
 */
U16 A71_GetRestrictedKeyPairInfo(U8 *idx, U16 *nBlocks, U8 *blockInfo, U16 *blockInfoLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk;
    U8 localBuf[255];
    U16 localBufLen = sizeof(localBuf);

#ifndef A71_IGNORE_PARAM_CHECK
    if ((idx == NULL) || (nBlocks == NULL) || (blockInfo == NULL) || (blockInfoLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    *idx = A71CH_NO_RESTRICTED_KP;
    *nBlocks = 0;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_RESTRICTED_INFO;

    // Ensure there's at least space to contain Offset|Lenght description of one block.
    if (*blockInfoLen < 4)
    {
        return ERR_BUF_TOO_SMALL;
    }

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, localBuf, &localBufLen);
            if (rv == SW_OK)
            {
                if (localBufLen == 0)
                {
                    // There is no restricted keypair and associated modifiable block(s)
                    *blockInfoLen = 0;
                }
                else if (localBufLen == 2)
                {
                    // Workaround for Applet 1.3 Issue
                    if ( (localBuf[0] == 0xFF) && (localBuf[1] == 0x00) )
                    {
                        // There is no restricted keypair and associated modifiable block(s)
                        *blockInfoLen = 0;
                    }
                    else
                    {
                        *blockInfoLen = 0;
                        rv = ERR_WRONG_RESPONSE;
                    }
                }
                else if (localBufLen > 2)
                {
                    if (*blockInfoLen < (localBufLen-2) )
                    {
                        *blockInfoLen = 0;
                        rv = ERR_BUF_TOO_SMALL;
                    }
                    else
                    {
                        *idx = localBuf[0];
                        *nBlocks = localBuf[1];
                        *blockInfoLen = localBufLen - 2;
                        memcpy(blockInfo, &localBuf[2], *blockInfoLen);
                    }
                }
                else
                {
                    *blockInfoLen = 0;
                    rv = ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Calculates the SHA256 value of the data provided as input.
* @param[in] data    Data buffer for which the SHA256 must be calculated
* @param[in] dataLen The length of data passed as argument
* @param[in,out] sha    IN: caller passes a buffer of at least 32 byte;
                        OUT: contains the calculated SHA256
* @param[in,out] shaLen IN: length of the sha buffer passed;
                        OUT: because SHA256 is used this is 32 byte exact
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GetSha256(U8 *data, U16 dataLen, U8 *sha, U16 *shaLen)
{
    U8 isOk = 0;
    U16 rv;
    U16 maxChunk = 0;
    U16 remainingData = dataLen;
    U16 toSend = 0;
    U16 dataOffset = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((data == NULL) || (sha == NULL) || (shaLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_MODULE_GET_SHA256;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_SHA256_ONE_SHOT;

    AllocateAPDUBuffer(pApdu);

    maxChunk = A71CH_SHA256_MAX_DATA_CHUNK;

    // Depending on dataLen we can do a one-shot SHA256 or need to split up across multiple APDU's
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
                rv = smApduGetResponseBody(pApdu, sha, shaLen);
                if (rv == SW_OK)
                {
                    if (32 != *shaLen)
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
        // Send out first chunk (SHA256_Init)
        pApdu->p2    = P2_SHA256_INIT;
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
                if (rv != SW_OK) { goto LBL_LEAVE_A71_GetSha256; }
            }
        }

        while (remainingData > maxChunk)
        {
            // Send out 'middle' chunks (SHA256_Update)
            pApdu->p2    = P2_SHA256_UPDATE;
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
                    if (rv != SW_OK) { goto LBL_LEAVE_A71_GetSha256; }
                }
            }
        }

        // Always close with SHA256_Final
        pApdu->p2    = P2_SHA256_FINAL;
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
                rv = smApduGetResponseBody(pApdu, sha, shaLen);
                if (rv == SW_OK)
                {
                    if (32 != *shaLen)
                    {
                        rv = ERR_WRONG_RESPONSE;
                    }
                }
            }
        }
    }

LBL_LEAVE_A71_GetSha256:
    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* This function disables - at device level - the ability to
  - Set symmetric keys without prior wrapping
  - Erase symmetric keys
  - Set ECC key pairs (private key part) without prior wrapping
  - Set ECC public key without prior wrapping

* @retval ::SW_OK Upon successful execution
*/
U16 A71_InjectLock()
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_SMF_LOCK_PLAIN_INJECTION;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* This function locks the module (typically to protect the module during transport to production
 facilities). When the A71CH is locked the functionality is reduced to the following subset:
    - ::A71_GetUniqueID
    - ::A71_GetUnlockChallenge
    - ::A71_UnlockModule
    - ::A71_GetModuleInfo

* @retval ::SW_OK Upon successful execution
*/
U16 A71_LockModule()
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_SMF_LOCK_MODULE;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* This function unlocks the module provided the correct code is provided as input argument.
The A71CH can only be unlocked once: if the device is already unlocked, the device cannot be locked
or unlocked again (it will remain unlocked).

The unlock code is calculated as follows:
    - Request a challenge from A71CH using ::A71_GetUnlockChallenge.
    - Decrypt the challenge in ECB mode using the appropriate configuration key value (the same as stored at index ::A71XX_CFG_KEY_IDX_MODULE_LOCK).
    - The decrypted value is the unlock \p code
* @param[in] code Value of unlock code
* @param[in] codeLen Length of unlock code (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_UnlockModule(U8 *code, U16 codeLen)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (code == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_SMF_UNLOCK_MODULE;

    if (codeLen != 16)
    {
        return ERR_API_ERROR;
    }

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, code, codeLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
 * Sets the label that is used when calling ::A71_EcdhPskDeriveMasterSecret or
 * ::A71_PskDeriveMasterSecret. Calling this function is optional. By default the label
 * used by the A71CH is \p 'master secret' (no quotes) as applicable for TLS 1.2.
 * The maximum size of the label that can be set is 24 byte.
 * @param[in] label Value to be stored and used as 'label' in TLS 1.2 protocol
 * @param[in] labelLen Length of label (less than or equal to ::A71XX_TLS_MAX_LABEL)
 * @retval ::SW_OK Upon successful execution
 */
U16 A71_SetTlsLabel(const U8* label, U16 labelLen)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (label == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_MODULE;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_SMF_LABEL;

    if (labelLen > A71CH_TLS_MAX_LABEL)
    {
        return ERR_API_ERROR;
    }

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, label, labelLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Verifies whether \p pSignature is the signature of \p pHash
* using \p pKeyData as the verifying public key.
*
* As opposed to function ::A71_EccVerify the public key value is passed as
* an argument to the A71CH.
*
* @param[in] pKeyData Public key passed as byte array in ANSI X9.62 uncompressed format
* @param[in] keyDataLen Length of public key passed as argument
* @param[in] pHash Pointer to the provided hash (or any other bytestring).
* @param[in] hashLen Length of the provided hash.
* @param[in] pSignature Pointer to the provided signature.
* @param[in] signatureLen Length of the provided signature.
* @param[out] pResult Pointer to the computed result of the verification.
  Points to a value of 0x01 in case of successful verification
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EccVerifyWithKey(const U8 *pKeyData, U16 keyDataLen, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv;
    U8 isOk = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((pKeyData == NULL) || (pHash == NULL) || (pSignature == NULL) || (pResult == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    if (keyDataLen != A71CH_PUB_KEY_LEN) { return ERR_API_ERROR; }
    if (hashLen != AX_SHA256_LEN) { return ERR_API_ERROR; }

    *pResult = 0;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_VERIFY_SIG_ECC_PUBLIC_KEY;
    pApdu->p1    = 0x00;
    pApdu->p2    = P2_PUBKEY_PRESENT;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, pHash, hashLen);
    smApduAppendCmdData(pApdu, pSignature, signatureLen);
    smApduAppendCmdData(pApdu, pKeyData, keyDataLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        U8 result = AX_VERIFY_FAILURE;
        U16 resultLen = 1;
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, &result, &resultLen);
            if ((rv != SW_OK) || (resultLen != 1) || ((result != AX_VERIFY_SUCCESS) && (result != AX_VERIFY_FAILURE)))
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                if (result == AX_VERIFY_SUCCESS)
                {
                    *pResult = 1;
                }
                else
                {
                    *pResult = 0;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
