/**
* @file a71ch_crypto_ecc.c
* @author NXP Semiconductors
* @version 1.2
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
* This file wraps the ECC cryptographic functionality of the A71CH module.
* @par History
* 1.0   2016-Aug-26: Initial version
* 1.1   2017-Nov-30: Adding function A71_EccRestrictedSign
* 1.2   2018-Dec-12: Modified implementation of A71_EccSign to adapt reported size of ECDSA signature
*
*****************************************************************************/
#include <stdio.h>

#include "a71ch_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "hcAsn.h"
#include "axHostCrypto.h"

/// @cond
#define ASN_SKIP_INTEGER_NORMALIZE 0x00
#define ASN_DO_INTEGER_NORMALIZE   0x01
static U16 A71_EccSign_Local(SST_Index_t index, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen, U8 normalize);
/// @endcond

/**
* Generates an ECC keypair at storage location \p index.
* @pre INJECTION_LOCKED has not been set
* @param[in] index  Storage index of the keypair to be created.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GenerateEccKeyPair(SST_Index_t index)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

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
* Generates an ECC keypair at storage location \p index.
* This function must be called instead of ::A71_GenerateEccKeyPair in case INJECTION_LOCKED was set.
*
* To use this function the value of the Key Pair configuration key must be known on the host. If
* this is not the case use ::A71_GenerateEccKeyPairWithCode instead.
*
* @param[in] index  Storage index of the keypair to be created.
* @param[in] configKey Value of Key Pair configuration key. This value has a high level of
*   confidentiality and may not be available to the Host.
* @param[in] configKeyLen Length of Key Pair configuration key
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GenerateEccKeyPairWithChallenge(SST_Index_t index, const U8 *configKey, U16 configKeyLen)
{
    U16 rv;
    S32 hcRet = 0;
    U8 challenge[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};
    U16 challengeLen = sizeof(challenge);
    U8 code[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};

#ifndef A71_IGNORE_PARAM_CHECK
    if (configKey == NULL) {
        return ERR_API_ERROR;
    }
#endif

    if (configKeyLen != 16)
    {
        return ERR_API_ERROR;
    }

    rv = A71_GetKeyPairChallenge(challenge, &challengeLen);
    if (rv != SW_OK) { return rv; }

    // Decrypt challenge
    hcRet = HOST_AES_ECB_DECRYPT(code, challenge, configKey, configKeyLen);
    if (hcRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    rv = A71_GenerateEccKeyPairWithCode(index, code, A71CH_MODULE_UNLOCK_CHALLENGE_LEN);
    return rv;
}

/**
* Generates an ECC keypair at storage location \p index.
* This function must be called instead of ::A71_GenerateEccKeyPair in case INJECTION_LOCKED was set.
*
* The assumption is the value of the Key Pair configuration key is not known on the host.
* If this does not apply use ::A71_GenerateEccKeyPairWithChallenge instead.
*
* The code is calculated as follows:
    - Request a challenge from A71CH using ::A71_GetUnlockChallenge.
    - Decrypt the challenge in ECB mode using the appropriate configuration key value (the same as stored at index ::A71XX_CFG_KEY_IDX_PRIVATE_KEYS).
    - The decrypted value is the value of \p code
*
* @param[in] index  Storage index of the keypair to be created.
* @param[in] code Value of unlock code.
* @param[in] codeLen Length of unlock code (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GenerateEccKeyPairWithCode(SST_Index_t index, const U8 *code, U16 codeLen)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = (U8)codeLen;

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
* Signs the hash \p pHash using the keypair at the indicated index.
* @param[in] index  Storage index of the keypair (private key) to be used.
* @param[in] pHash  Pointer to the provided hash (or any other bytestring).
* @param[in] hashLen Length of the provided hash.
* @param[in,out] pSignature Pointer to the computed signature.
* @param[in,out] pSignatureLen Pointer to the length of the computed signature.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EccSign(SST_Index_t index, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen)
{
    return A71_EccSign_Local(index, pHash, hashLen, pSignature, pSignatureLen, ASN_SKIP_INTEGER_NORMALIZE);
}

/**
* Signs the hash \p pHash using the keypair at the indicated index.
*
* The integer representation of the ECDSA signatures'
*  r and s component is modified to be in line with ASN.1 (Ensuring an integer value is always encoded in the
*  smallest possible number of octets)
* @param[in] index  Storage index of the keypair (private key) to be used.
* @param[in] pHash  Pointer to the provided hash (or any other bytestring).
* @param[in] hashLen Length of the provided hash.
* @param[in,out] pSignature Pointer to the computed signature.
* @param[in,out] pSignatureLen Pointer to the length of the computed signature.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EccNormalizedAsnSign(SST_Index_t index, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen)
{
    return A71_EccSign_Local(index, pHash, hashLen, pSignature, pSignatureLen, ASN_DO_INTEGER_NORMALIZE);
}

/// @cond
static U16 A71_EccSign_Local(SST_Index_t index, const U8 *pHash, U16 hashLen, U8 *pSignature, U16 *pSignatureLen, U8 normalize)
{
    U8 isOk = 0;
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((pHash == NULL) || (pSignature == NULL) || (pSignatureLen == NULL) || (*pSignatureLen < 72)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SIGN_HASH_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, pHash, hashLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, pSignature, pSignatureLen);
            if (rv != SW_OK)
            {
                return ERR_WRONG_RESPONSE;
            }

            if (normalize == ASN_DO_INTEGER_NORMALIZE)
            {
                if ( hcNormalizeAsnSignatureEcc(pSignature, pSignatureLen) != SW_OK)
                {
                    return ERR_WRONG_RESPONSE;
                }
            }
            else
            {
                U16 actualSignatureLen;
                // Detect whether padded bytes must be removed from pSignature

                // As we're not inspecting whether we received a valid ASN structure, at least ensure there
                // is an 'L' byte in the signature
                if (*pSignatureLen >= 2)
                {
                    actualSignatureLen = pSignature[1] + 2;
                    if (actualSignatureLen < *pSignatureLen)
                    {
                        // Patch the value of *pSignatureLen
                        *pSignatureLen = actualSignatureLen;
                    }
                }
                else
                {
                    return ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
/// @endcond

/**
* Patches a predetermined fixed size memory region in GP storage with the byte array \p updateBytes
* Creates a signed certificate - in place in GP storage - using a predetermined block of GP storage data
*
* @param[in] index  Storage index of the keypair (private key) to be used.
* @param[in] updateBytes Byte array to be written into GP storage
* @param[in] updateBytesLen Length of the provided byte array (\p updateBytes).
* @param[out] invocationCount Amount of times the underlying APDU has been called succesfully.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EccRestrictedSign(SST_Index_t index, const U8 *updateBytes, U16 updateBytesLen, U8 *invocationCount)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv;
    U8 isOk = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((updateBytes == NULL) || (invocationCount == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    if (updateBytesLen > (U16)0x00FF)
    {
        return ERR_API_ERROR;
    }

    *invocationCount = 0;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SIGN_HASH_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, updateBytes, updateBytesLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        U8 result = 0x00;
        U16 resultLen = 1;
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, &result, &resultLen);
            if ((rv != SW_OK) || (resultLen != 1))
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                *invocationCount = result;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}


/**
* Verifies whether \p pSignature is the signature of \p pHash
* using the public key stored under \p index as the verifying public key.
*
* The index refers to an instance of the PUBLIC_KEY secure storage class on the A71CH.
* \note The public key of an ECC key pair cannot be used for a verify operation.
* \note ::A71_EccVerifyWithKey allows to pass the value of the public key rather than use a stored public key.
*
* @param[in] index Storage index of the key used for the verification.
* @param[in] pHash Pointer to the provided hash (or any other bytestring).
* @param[in] hashLen Length of the provided hash (\p pHash).
* @param[in] pSignature Pointer to the provided signature.
* @param[in] signatureLen Length of the provided signature (\p pSignature)
* @param[out] pResult Pointer to the computed result of the verification.
  Points to a value of 0x01 in case of successful verification
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EccVerify(SST_Index_t index, const U8 *pHash, U16 hashLen, const U8 *pSignature, U16 signatureLen, U8 *pResult)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv;
    U8 isOk = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((pHash == NULL) || (pSignature == NULL) || (pResult == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    *pResult = 0;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_VERIFY_SIG_ECC_PUBLIC_KEY;
    pApdu->p1    = index;
    pApdu->p2    = P2_PUBKEY_ABSENT;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, pHash, hashLen);
    smApduAppendCmdData(pApdu, pSignature, signatureLen);

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

/**
* Generates and retrieves a shared secret ECC point \p pSharedSecret using the private key stored
* at \p index and a public key \p pOtherPublicKey passed as argument.
* @param[in] index to the key pair (private key to be used)
* @param[in] pOtherPublicKey Pointer to the given public key.
* @param[in] otherPublicKeyLen Length of the given public key.
* @param[in,out] pSharedSecret Pointer to the computed shared secret.
* @param[in,out] pSharedSecretLen Pointer to the length of the computed shared secret.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EcdhGetSharedSecret(U8 index, const U8 *pOtherPublicKey, U16 otherPublicKeyLen, U8 *pSharedSecret, U16 *pSharedSecretLen)
{
    U16 rv;
    U8 isOk = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((pOtherPublicKey == NULL) || (pSharedSecret == NULL) || (pSharedSecretLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SHARED_SECRET_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, pOtherPublicKey, otherPublicKeyLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, pSharedSecret, pSharedSecretLen);
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
