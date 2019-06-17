/**
* @file a71ch_sst.c
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
* This file wraps the secure storage functionality of the A71CH module.
* @par History
* 1.0   1-oct-2016 : Initial version
*
*****************************************************************************/
#include <stddef.h>
#include <string.h>
#include <stdio.h> // DEBUG

#include "scp.h"
#include "a71ch_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "axHostCrypto.h"
#include "sm_printf.h"

/// @cond
static U16 SST_FreezeGeneric(U8 ins, SST_Index_t index)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = ins;
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

static U16 SST_FreezeGenericWithCode(U8 ins, SST_Index_t index, U8 *code, U16 codeLen)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = ins;
    pApdu->p1    = index;
    pApdu->p2    = (U8)codeLen;

    if ((codeLen != 16) || (code == NULL))
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

static U16 SST_FreezeGenericWithChallenge(U8 ins, SST_Index_t index, U8 *configKey, U16 configKeyLen)
{
    U16 rv;
    S32 hcRet = 0;
    U8 challenge[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};
    U16 challengeLen = sizeof(challenge);
    U8 code[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};

    if ( (configKey == NULL) || (configKeyLen != 16) )
    {
        return ERR_API_ERROR;
    }

    switch (ins)
    {
        case A71CH_INS_FREEZE_ECC_KEYPAIR:
            rv = A71_GetKeyPairChallenge(challenge, &challengeLen);
            if (rv != SW_OK) { return rv; }
        break;

        case A71CH_INS_FREEZE_ECC_PUBLIC_KEY:
            rv = A71_GetPublicKeyChallenge(challenge, &challengeLen);
            if (rv != SW_OK) { return rv; }
        break;

        default:
            return ERR_API_ERROR;
    }

    // Decrypt challenge
    hcRet = HOST_AES_ECB_DECRYPT(code, challenge, configKey, configKeyLen);
    if (hcRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    rv = SST_FreezeGenericWithCode(ins, index, code, A71CH_MODULE_UNLOCK_CHALLENGE_LEN);
    return rv;
}

static U16 SST_EraseGeneric(U8 ins, SST_Index_t index)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = ins;
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

static U16 SST_EraseGenericWithCode(U8 ins, SST_Index_t index, U8 *code, U16 codeLen)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = ins;
    pApdu->p1    = index;
    pApdu->p2    = (U8)codeLen;

    if ((code == NULL) || (codeLen != 16)) {
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

static U16 SST_EraseGenericWithChallenge(U8 ins, SST_Index_t index, U8 *configKey, U16 configKeyLen)
{
    U16 rv;
    S32 hcRet = 0;
    U8 challenge[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};
    U16 challengeLen = sizeof(challenge);
    U8 code[A71CH_MODULE_UNLOCK_CHALLENGE_LEN] = {0};

    if ((configKey == NULL) || (configKeyLen != 16))
    {
        return ERR_API_ERROR;
    }

    switch (ins)
    {
        case A71CH_INS_ERASE_ECC_KEYPAIR:
            rv = A71_GetKeyPairChallenge(challenge, &challengeLen);
            if (rv != SW_OK) { return rv; }
        break;

        case A71CH_INS_ERASE_ECC_PUBLIC_KEY:
            rv = A71_GetPublicKeyChallenge(challenge, &challengeLen);
            if (rv != SW_OK) { return rv; }
        break;

        default:
            return ERR_API_ERROR;
    }

    // Decrypt challenge
    hcRet = HOST_AES_ECB_DECRYPT(code, challenge, configKey, configKeyLen);
    if (hcRet != HOST_CRYPTO_OK)
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    rv = SST_EraseGenericWithCode(ins, index, code, A71CH_MODULE_UNLOCK_CHALLENGE_LEN);
    return rv;
}
/// @endcond

/**
* Sets an ECC Key pair at storage location \p index with the provided values for public and private key.
* The private key can optionally be RFC3944 wrapped. Whether wrapping is applied or not is implicit in
* the length of the private key.
* @param[in] index  Storage index of the keypair to be created.
* @param[in] publicKey Pointer to the byte array containing the public key.
*    The public key must be in ANSI X9.62 uncompressed format (including the leading 0x04 byte).
* @param[in] publicKeyLen Length of the public key (65 byte)
* @param[in] privateKey Pointer to the byte array containing the private key. The private key may be RFC3394 wrapped
  using the config key stored at index ::A71CH_CFG_KEY_IDX_PRIVATE_KEYS
* @param[in] privateKeyLen Length of the private key (either 32 byte for keys in plain format or 40 byte for keys in RFC3944 wrapped format)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetEccKeyPair(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen, const U8 *privateKey, U16 privateKeyLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((publicKey == NULL) || (privateKey == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, privateKey, privateKeyLen);
    smApduAppendCmdData(pApdu, publicKey, publicKeyLen);

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
* Retrieves the ECC Public Key - from a key pair - from the storage location \p index into the provided buffer.
* The public key retrieved is in ANSI X9.62 uncompressed format (including the leading 0x04 byte).
*
* @param[in] index  Storage index of the key pair
* @param[in,out] publicKey IN: buffer to contain public key byte array; OUT: public key
* @param[in,out] publicKeyLen IN: size of provided buffer; OUT: Length of the retrieved public key
* @retval ::SW_OK Upon successful execution
* @retval ::ERR_BUF_TOO_SMALL \p publicKey buffer is too small
*/
U16 A71_GetPublicKeyEccKeyPair(SST_Index_t index, U8 *publicKey, U16 *publicKeyLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0x00;

    if ( (publicKey == NULL) || (publicKeyLen == NULL) || (*publicKeyLen < 65) ) {return ERR_BUF_TOO_SMALL;}

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, publicKey, publicKeyLen);
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
 * Retrieve the usage counter (i.e. how much times the key pair has been used so far to sign)
 * and the maximum usage counter.
 * If the key pair is NOT restricted, usage counter and maximum usage counter will be set to 0 and the
 * restricted parameter will be 0 (otherwise it is 1)
 *
 * @param[in] index  Storage index of the key pair
 * @param[out] restricted 0 when the key pair on \p index is not restricted; 1 if it is restricted.
 *   A restricted key pair is a key pair that can only be used to sign a dedicated area in GP storage.
 * @param[out] usedCnt  Number of times the key pair on \p index has been used to sign
 *   (assuming it is a restricted key pair)
 * @param[out] maxUseCnt Indicates the maximum amount of signing operations associated with the key pair at \p index.
 *   In case the value is zero, there is no limit on the amount of signing operations.
 */
U16 A71_GetEccKeyPairUsage(SST_Index_t index, U8 *restricted, U16 *usedCnt, U16 *maxUseCnt)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0x00;
    U8 localBuf[2];
    U16 localBufLen = sizeof(localBuf);

#ifndef A71_IGNORE_PARAM_CHECK
    if ((restricted == NULL) || (usedCnt == NULL) || (maxUseCnt == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_ECC_KEYPAIR;
    pApdu->p1    = index;
    pApdu->p2    = P2_KEY_PAIR_USAGE_COUNTER;

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
                    *restricted = 0;
                    *usedCnt = 0;
                    *maxUseCnt = 0;
                }
                else if (localBufLen == 2)
                {
                    // Applet 1.3: Under some circumstances
                    // 0x00:0x00 is returned even though there is NO restricted signing pair
                    *restricted = 1;
                    *usedCnt = localBuf[0];
                    *maxUseCnt = localBuf[1];
                }
                else
                {
                    *restricted = 0;
                    *usedCnt = 0;
                    *maxUseCnt = 0;
                    rv = ERR_WRONG_RESPONSE;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Freezes an ECC key pair at storage location \p index. This means that the key pair
  can no longer be erased or its value changed.
* @pre INJECTION_LOCKED has not been set
* @param[in] index  Storage index of the key pair to be frozen.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeEccKeyPair(SST_Index_t index)
{
    return SST_FreezeGeneric(A71CH_INS_FREEZE_ECC_KEYPAIR, index);
}

/**
* Freezes an ECC key pair at storage location \p index. This means that the key pair
  can no longer be erased or its value changed.
  This function must be called instead of ::A71_FreezeEccKeyPair in case INJECTION_LOCKED
  was set

  The assumption is the value of the Key Pair configuration key is known on the host. If
  this does not apply use ::A71_FreezeEccKeyPairWithCode instead.
* @param[in] index  Storage index of the key pair to be frozen.
* @param[in] configKey Value of Key Pair configuration key. This value has a high level of
    confidentiality and may not be available to the Host.
* @param[in] configKeyLen Length of Key Pair configuration key
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeEccKeyPairWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen)
{
    return SST_FreezeGenericWithChallenge(A71CH_INS_FREEZE_ECC_KEYPAIR, index, configKey, configKeyLen);
}

/**
* Freezes an ECC key pair at storage location \p index provided the correct code value
  is passed as argument. Freezing the key pair means that it can no longer be erased or its value changed.
  This function must be called instead of ::A71_FreezeEccKeyPair in case INJECTION_LOCKED was set.

  The assumption is the value of the Key Pair configuration key is not known on the host.
  If this does not apply use ::A71_FreezeEccKeyPairWithChallenge instead.

  The code is calculated as follows:
    - Request a challenge from A71CH using ::A71_GetUnlockChallenge.
    - Decrypt the challenge in ECB mode using the appropriate configuration key value (the same as stored at index ::A71CH_CFG_KEY_IDX_PRIVATE_KEYS).
    - The decrypted value is the value of \p code
* @param[in] index  Storage index of the key pair to be frozen.
* @param[in] code Value of unlock code
* @param[in] codeLen Length of unlock code (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeEccKeyPairWithCode(SST_Index_t index, U8 *code, U16 codeLen)
{
    return SST_FreezeGenericWithCode(A71CH_INS_FREEZE_ECC_KEYPAIR, index, code, codeLen);
}

/**
* Erases an ECC key pair at storage location \p index. This means that the key pair
  can no longer be used before a new value is set.
* @pre INJECTION_LOCKED has not been set
* @param[in] index  Storage index of the key pair to be frozen.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseEccKeyPair(SST_Index_t index)
{
    return SST_EraseGeneric(A71CH_INS_ERASE_ECC_KEYPAIR, index);
}

/**
* Erases an ECC key pair at storage location \p index. This means that the key pair
  can no longer be used before a new value is set.
  This function must be called instead of ::A71_EraseEccKeyPair in case INJECTION_LOCKED was set.

  The assumption is the value of the Key Pair configuration key is known on the host.
  If this does not apply use ::A71_EraseEccKeyPairWithCode instead.
* @param[in] index  Storage index of the key pair to be frozen.
* @param[in] configKey Value of Key Pair configuration key. This value has a high level of
    confidentiality and may not be available to the Host.
* @param[in] configKeyLen Length of Key Pair configuration key
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseEccKeyPairWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen)
{
    return SST_EraseGenericWithChallenge(A71CH_INS_ERASE_ECC_KEYPAIR, index, configKey, configKeyLen);
}

/**
* Erases an ECC key pair at storage location \p index. This means that the key pair
  can no longer be used before a new value is set.
  This function must be called instead of ::A71_EraseEccKeyPair in case INJECTION_LOCKED was set.

  The assumption is the value of the Key Pair configuration key is not known on the host.
  If this does not apply use ::A71_EraseEccKeyPairWithChallenge instead.

  The code is calculated as follows:
    - Request a challenge from A71CH using ::A71_GetUnlockChallenge.
    - Decrypt the challenge in ECB mode using the appropriate configuration key value (the same as stored at index ::A71CH_CFG_KEY_IDX_PRIVATE_KEYS).
    - The decrypted value is the value of \p code
* @param[in] index  Storage index of the key pair to be frozen.
* @param[in] code Value of unlock code
* @param[in] codeLen Length of unlock code (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseEccKeyPairWithCode(SST_Index_t index, U8 *code, U16 codeLen)
{
    return SST_EraseGenericWithCode(A71CH_INS_ERASE_ECC_KEYPAIR, index, code, codeLen);
}

/**
* Sets an ECC Public Key at storage location \p index with the provided value for public key
* either in plain ANSI X9.62 uncompressed format or wrapped.
* Whether RFC3944 wrapping is applied or not is implicit in the length of the public key.
* In case RFC3944 wrapping is applied the first byte of the public key
* (the one indicating the public key format) is removed before applying wrapping.
* @param[in] index  Storage index of the public key to be set.
* @param[in] publicKey Pointer to the byte array containing the public key. The public key may be RFC3394 wrapped
    using the config key stored at index ::A71CH_CFG_KEY_IDX_PUBLIC_KEYS
* @param[in] publicKeyLen Length of the public key (either 65 byte for keys in plain format or 72 byte for keys in RFC3944 wrapped format)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetEccPublicKey(SST_Index_t index, const U8 *publicKey, U16 publicKeyLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (publicKey == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_ECC_PUBLIC_KEY;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, publicKey, publicKeyLen);

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
* Retrieves the ECC Public Key from the storage location \p index into the provided buffer.
* The public key is in ANSI X9.62 uncompressed format (including the leading 0x04 byte).
* @param[in] index  Storage index of the public key to be retrieved.
* @param[in,out] publicKey IN: buffer to contain public key byte array; OUT: public key
* @param[in,out] publicKeyLen IN: size of provided buffer; OUT: Length of the retrieved public key
* @retval ::SW_OK Upon successful execution
* @retval ::ERR_BUF_TOO_SMALL \p publicKey buffer is too small
*/
U16 A71_GetEccPublicKey(SST_Index_t index, U8 *publicKey, U16 *publicKeyLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0x00;

    if ( (publicKey == NULL) || (publicKeyLen == NULL) || (*publicKeyLen < 65) ) {return ERR_BUF_TOO_SMALL;}

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_ECC_PUBLIC_KEY;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, publicKey, publicKeyLen);
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Freezes an ECC public key at storage location \p index. This means that the public key
  can no longer be erased or its value changed.
  This function must be called instead of ::A71_FreezeEccPublicKey in case INJECTION_LOCKED
  was set.

  The assumption is the value of the Public Key configuration key is known on the host. If
  this does not apply use ::A71_FreezeEccPublicKeyWithCode instead.
* @param[in] index  Storage index of the public key to be frozen.
* @param[in] configKey Value of Public Key Pair key. This value has a high level of
    confidentiality and may not be available to the Host.
* @param[in] configKeyLen Length of Public Key configuration key
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeEccPublicKeyWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen)
{
    return SST_FreezeGenericWithChallenge(A71CH_INS_FREEZE_ECC_PUBLIC_KEY, index, configKey, configKeyLen);
}

/**
* Freezes an ECC public key at storage location \p index provided the correct code value
  is passed as argument. Freezing the public key means that it can no longer be erased or its value changed.
  This function must be called instead of ::A71_FreezeEccPublicKey in case INJECTION_LOCKED was set.

  The assumption is the value of the Public Key configuration key is not known on the host.
  If this does not apply use ::A71_FreezeEccPublicKeyWithChallenge instead.

  The code is calculated as follows:
    - Request a challenge from A71CH using ::A71_GetUnlockChallenge.
    - Decrypt the challenge in ECB mode using the appropriate configuration key value (the same as stored at index ::A71CH_CFG_KEY_IDX_PUBLIC_KEYS).
    - The decrypted value is the value of \p code
* @param[in] index  Storage index of the key pair to be frozen.
* @param[in] code Value of unlock code
* @param[in] codeLen Length of unlock code (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeEccPublicKeyWithCode(SST_Index_t index, U8 *code, U16 codeLen)
{
    return SST_FreezeGenericWithCode(A71CH_INS_FREEZE_ECC_PUBLIC_KEY, index, code, codeLen);
}

/**
* Freezes an ECC public key at storage location \p index. This means that the public key
  can no longer be erased or its value changed.

* @param[in] index  Storage index of the public key to be frozen.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeEccPublicKey(SST_Index_t index)
{
    return SST_FreezeGeneric(A71CH_INS_FREEZE_ECC_PUBLIC_KEY, index);
}

/**
* Erases an ECC public key at storage location \p index. This means that the public key
  can no longer be used before a new value is set.
* @pre INJECTION_LOCKED has not been set
* @param[in] index  Storage index of the public key to be frozen.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseEccPublicKey(SST_Index_t index)
{
    return SST_EraseGeneric(A71CH_INS_ERASE_ECC_PUBLIC_KEY, index);
}

/**
* Erases an ECC public key at storage location \p index. This means that the public key
  can no longer be used before a new value is set.
  This function must be called instead of ::A71_EraseEccPublicKey in case INJECTION_LOCKED
  was set.

  The assumption is the value of the Public Key configuration key is known on the host. If
  this does not apply use ::A71_EraseEccPublicKeyWithCode instead.
* @param[in] index  Storage index of the public key to be frozen.
* @param[in] configKey Value of Public Key Pair key. This value has a high level of
    confidentiality and may not be available to the Host.
* @param[in] configKeyLen Length of Public Key configuration key
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseEccPublicKeyWithChallenge(SST_Index_t index, U8 *configKey, U16 configKeyLen)
{
    return SST_EraseGenericWithChallenge(A71CH_INS_ERASE_ECC_PUBLIC_KEY, index, configKey, configKeyLen);
}

/**
* Erases an ECC public key at storage location \p index. This means that the public key
  can no longer be used before a new value is set.
  This function must be called instead of ::A71_EraseEccPublicKey in case INJECTION_LOCKED was set.

  The assumption is the value of the Public Key configuration key is not known on the host.
  If this does not apply use ::A71_EraseEccPublicKeyWithChallenge instead.

  The code is calculated as follows:
    - Request a challenge from A71CH using ::A71_GetUnlockChallenge.
    - Decrypt the challenge in ECB mode using the appropriate configuration key value (the same as stored at index ::A71CH_CFG_KEY_IDX_PUBLIC_KEYS).
    - The decrypted value is the value of \p code
* @param[in] index  Storage index of the key pair to be frozen.
* @param[in] code Value of unlock code
* @param[in] codeLen Length of unlock code (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseEccPublicKeyWithCode(SST_Index_t index, U8 *code, U16 codeLen)
{
    return SST_EraseGenericWithCode(A71CH_INS_ERASE_ECC_PUBLIC_KEY, index, code, codeLen);
}

/**
* Sets a symmetric key at storage location \p index with the key value.
* The key locations indexed are the same as the one referenced by ::A71_SetRfc3394WrappedAesKey
* @param[in] index  Storage index of the symmetric key to be set.
* @param[in] key Pointer to the byte array containing the symmetric key
* @param[in] keyLen Length of the symmetric key (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetSymKey(SST_Index_t index, const U8 *key, U16 keyLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (key == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_SYM_KEY;
    pApdu->p1    = index;
    pApdu->p2    = P2_NOT_WRAPPED;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, key, keyLen);

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
* Sets an RFC3394 wrapped AES key in secure storage.
* The key value being set, must be wrapped with the value already stored at \p index.
* The key locations indexed are the same as the one referenced by ::A71_SetSymKey
* @param[in] index index of the key to be set. At the same time the index of the wrapping key.
* @param[in] key Pointer to the supplied key data.
* @param[in] keyLen Length of the supplied key data.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetRfc3394WrappedAesKey(SST_Index_t index, const U8 *key, U16 keyLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if (key == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_SYM_KEY;
    pApdu->p1    = index;
    pApdu->p2    = P2_RFC3394_WRAPPED;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, key, keyLen);

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
* Freezes a symmetric key at storage location \p index. This means the value of the key
  at the specified index can no longer be changed.

* @param[in] index  Storage index of the symmetric key to be frozen.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeSymKey(SST_Index_t index)
{
    return SST_FreezeGeneric(A71CH_INS_FREEZE_SYM_KEY, index);
}

/**
* Erases the symmetric key at storage location \p index. This means the value of the key
  at the specified index is cleared. The value must be set anew before the key can be
  used.

* @param[in] index  Storage index of the symmetric key to be set.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_EraseSymKey(SST_Index_t index)
{
    return SST_EraseGeneric(A71CH_INS_ERASE_SYM_KEY, index);
}


/**
* Increments the monotonic counter at storage location index by one.

* @param[in] index  Storage index of the counter.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_IncrementCounter(SST_Index_t index)
{
    U16 rv;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_COUNTERS;
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
* Sets the value of the monotonic counter at storage location 'index' with the value passed as parameter.

* @param[in] index  Storage index of the counter.
* @param[in] value Counter value to be set
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetCounter(SST_Index_t index, U32 value)
{
    U16 rv;
    U8 byteArray[MONOTONIC_COUNTER_BYTE_COUNT];

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_SET_COUNTERS;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    byteArray[0] = (U8)(value >> 24);
    byteArray[1] = (U8)(value >> 16);
    byteArray[2] = (U8)(value >> 8);
    byteArray[3] = (U8)value;

    smApduAppendCmdData(pApdu, byteArray, MONOTONIC_COUNTER_BYTE_COUNT);

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
* Gets the value of the monotonic counter at storage location 'index'.

* @param[in] index  Storage index of the counter.
* @param[out] pValue Counter value retrieved
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GetCounter(SST_Index_t index, U32 *pValue)
{
    U16 rv;
    U8 cntBuf[MONOTONIC_COUNTER_BYTE_COUNT];
    U16 cntBufLen = sizeof(cntBuf);
    U8 isOk = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (pValue == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_GET_COUNTERS;
    pApdu->p1    = index;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, cntBuf, &cntBufLen);

            if (rv != SW_OK)
            {
                *pValue = 0;
            }
            else if (cntBufLen != MONOTONIC_COUNTER_BYTE_COUNT)
            {
                *pValue = 0;
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                *pValue = (cntBuf[0] << 24) + (cntBuf[1] << 16) + (cntBuf[2] << 8) + cntBuf[3];
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Sets the value of the monotonic counter at storage location 'index' to zero.

  \note Only available when the applet is in Debug Mode.

* @param[in] index  Storage index of the counter.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_DbgEraseCounter(SST_Index_t index)
{
    return SST_EraseGeneric(A71CH_INS_ERASE_COUNTERS, index);
}

/**
 * Sets a data chunk of General Purpose storage in the security module.
 * Depending on the size of the chunk, this requires one or more APDU exchanges with the security module.
 * \pre The addressed General Purpose storage is not locked.
 * \note In case part of the addressed General Purpose storage is locked,
 *    only part of the provided data will have been written most likely
 *   leading to an inconsistent data set stored in General Purpose storage.
 *
 * @param[in] dataOffset Offset for the data in the GP Storage.
 * @param[in] data IN: buffer containing data to write
 * @param[in] dataLen Amount of data to write
 * @retval ::SW_OK Upon successful execution
*/
U16 A71_SetGpData(U16 dataOffset, const U8 *data, U16 dataLen)
{
    U16 gpdataOffset = dataOffset;
    U16 sentLength = 0;
    U16 chunkSize = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = 0;
    int maxChunkSize = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if (data == NULL) {
        return ERR_API_ERROR;
    }
#endif

    maxChunkSize = A71CH_GP_STORAGE_MAX_DATA_CHUNK;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_GP_DATA;

    AllocateAPDUBuffer(pApdu);

    while (sentLength < dataLen)
    {
        if ((dataLen-sentLength) > maxChunkSize)
        {
            chunkSize = (U16)maxChunkSize;
        }
        else
        {
            chunkSize = (dataLen-sentLength);
        }

        pApdu->p1 = (U8)(gpdataOffset >> 8);
        pApdu->p2 = (U8)gpdataOffset;
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        smApduAppendCmdData(pApdu, &data[sentLength], chunkSize);

        rv = (U16)scp_Transceive(pApdu, SCP_MODE);
        if (rv == SMCOM_OK)
        {
            // No response data expected
            rv = CheckNoResponseData(pApdu);
            if (rv != SW_OK)
            {
                break;
            }
        }

        sentLength += chunkSize;
        gpdataOffset += chunkSize;
    }
    FreeAPDUBuffer(pApdu);

    return rv;
}

/**
* Sets a data chunk of General Purpose storage in the security module.
* Depending on the size of the chunk, this requires one or more APDU exchanges with the security module.
* \pre The addressed General Purpose storage is not locked.
* \note In case more than one apdu is required, this function first validates that each of the chunks of the
*   addressed General Purpose storage is not locked, and only in that case try to write the
*   provided data using A71_SetGpData()
*
* @param[in] dataOffset Offset for the data in the GP Storage.
* @param[in] data IN: buffer containing data to write
* @param[in] dataLen Amount of data to write
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetGpDataWithLockCheck(U16 dataOffset, const U8 *data, U16 dataLen)
{
    U16 gpdataOffset = dataOffset;
    U16 rv = 0;

    U8 map[A71CH_MAP_SIZE_MAX] = {0};
    U16 mapLen = sizeof(map);

    int gpStatusOffset;     // GP Storage offsets in CredentialinfoTable returned from GetCredentialInfo
    int gpStatusN;          // size of GP storage map in CredentialinfoTable returned from GetCredentialInfo

    U8 credtialstateLocked = 0x0F;   // nibble of lock state in map

    U8 fLocked = 0;         // flag to indicate that a locked segment was found

    if (dataLen <= A71CH_GP_STORAGE_MAX_DATA_CHUNK)
    {
        // We can safely use A71_SetGpData, since will be sent in one apdu
        rv = A71_SetGpData(dataOffset, data, dataLen);
    }
    else
    {
        // Need to Verify all segments of the data are not locked, by obtaining the credentials info map, and checking each segment within
        rv = A71_GetCredentialInfo(map, &mapLen);

        if (rv == SW_OK)
        {
            gpStatusN = (A71CH_GP_STORAGE_SIZE_B / A71CH_GP_STORAGE_GRANULARITY);
            gpStatusOffset = mapLen - gpStatusN;

            // loop over each segment in required write area to find out weather it is locked

            while (gpdataOffset < (dataOffset + dataLen) )
            {
                fLocked = ((map[(gpdataOffset / A71CH_GP_STORAGE_GRANULARITY) + gpStatusOffset] & 0x0F) == credtialstateLocked) ? 0x01 : 0x00;
                if (fLocked) {
                    rv = SW_COMMAND_NOT_ALLOWED;  // Locked memory - this value is what the SetGpData cmd would have returned in that case
                    break;
                }
                gpdataOffset += A71CH_GP_STORAGE_GRANULARITY;
            }

            if (! fLocked )
            {
                // no segment is locked - we can safly try to write requested data
                rv = A71_SetGpData(dataOffset, data, dataLen);
            }
        }
    }

    return rv;
}

/**
* Retrieve a chunk of data from general purpose (GP) storage.
* @param[in] dataOffset Offset for the data in the GP Storage.
* @param[in,out] data IN: buffer to contain data; OUT: retrieved data
* @param[in] dataLen Amount of data to retrieve
* @retval ::SW_OK Upon successful execution
*/
U16 A71_GetGpData(U16 dataOffset, U8 *data, U16 dataLen)
{
    U16 gpdataOffset = dataOffset;
    U16 outOffset = 0;
    U16 retval = SW_OK;
    U16 rv = ERR_API_ERROR;
    U8 tmp[2];
    U8 isOk = 0;
    U16 bytesToRead = dataLen;
    U16 toRead = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    int maxChunkSize = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if (data == NULL) {
        return ERR_API_ERROR;
    }
#endif

    maxChunkSize = A71CH_GP_STORAGE_MAX_DATA_CHUNK;

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_GET_GP_DATA;

    AllocateAPDUBuffer(pApdu);

    while (bytesToRead > 0)
    {
        if (bytesToRead > maxChunkSize)
        {
            toRead = (U16)maxChunkSize;
        }
        else
        {
            toRead = bytesToRead;
        }

        pApdu->p1 = gpdataOffset >> 8;
        pApdu->p2 = gpdataOffset & 0xff;

        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        tmp[0] = toRead >> 8;
        tmp[1] = toRead & 0xff;
        smApduAppendCmdData(pApdu, tmp, 2);

        rv = (U16)scp_Transceive(pApdu, SCP_MODE);
        if (rv == SMCOM_OK)
        {
            rv = smGetSw(pApdu, &isOk);
            if (isOk)
            {
                // A buffer overflow can only occur when the secure module would
                // return more data then requested.
                // We calculate remaining buffer size to be resistent against such an attack scenario.
                U16 length = (dataLen - outOffset);

                rv = smApduGetResponseBody(pApdu,  &data[outOffset], &length);
                if ((rv != SW_OK) || (toRead != length))
                {
                    PRINTF("DEBUG (File=%s,Line=%d): toRead=%d ?= length=%d; retval=0x%04X\r\n", __FILE__, __LINE__, toRead, length, retval);
                    rv = ERR_WRONG_RESPONSE;
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }

        gpdataOffset += toRead;
        bytesToRead -= toRead;
        outOffset += toRead;
    }

    FreeAPDUBuffer(pApdu);

    return rv;
}

/**
* Mark a chunk in GP storage as frozen (meaning further modification of the GP storage area is disallowed).
  Both the \p dataOffset and \p dataLen must be aligned on ::A71CH_GP_STORAGE_GRANULARITY

* @param[in] dataOffset Offset for the data in the GP Storage.
* @param[in] dataLen Amount of data to freeze
* @retval ::SW_OK Upon successful execution
*/
U16 A71_FreezeGpData(U16 dataOffset, U16 dataLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = 0;
    U8 byteArray[2];

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_FREEZE_GP_DATA;
    pApdu->p1 = (U8)(dataOffset >> 8);
    pApdu->p2 = (U8)dataOffset;

    AllocateAPDUBuffer(pApdu);

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    byteArray[0] = (U8)(dataLen >> 8);
    byteArray[1] = (U8)dataLen;

    smApduAppendCmdData(pApdu, byteArray, 2);

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
* Sets a config key at storage location \p index with the key value.
* The key locations indexed are the same as the one referenced by ::A71_SetRfc3394WrappedConfigKey
* @param[in] index  Storage index of the config key to be set.
* @param[in] key Pointer to the byte array containing the config key
* @param[in] keyLen Length of the config key (must be 16)
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetConfigKey(SST_Index_t index, const U8 *key, U16 keyLen)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

#ifndef A71_IGNORE_PARAM_CHECK
    if (key == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_CONFIG_KEY;
    pApdu->p1    = index;
    pApdu->p2    = P2_NOT_WRAPPED;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, key, keyLen);

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
* Sets an RFC3394 wrapped config key in secure storage.
* The key value being set, must be wrapped with the value already stored at the \p index.
* The key locations indexed are the same as the one referenced by ::A71_SetConfigKey
* @param[in] index index of the key to be set. At the same time the index of the wrapping key.
* @param[in] key Pointer to the supplied key data.
* @param[in] keyLen Length of the supplied key data.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_SetRfc3394WrappedConfigKey(SST_Index_t index, const U8 *key, U16 keyLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = 0;

#ifndef A71_IGNORE_PARAM_CHECK
    if (key == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_SET_CONFIG_KEY;
    pApdu->p1    = index;
    pApdu->p2    = P2_RFC3394_WRAPPED;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, key, keyLen);

    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
