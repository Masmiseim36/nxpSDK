/**
* @file ax_crypto_ecc.c
* @author NXP Semiconductors
* @version 1.0
* @section LICENSE
* ----------------------------------------------------------------------------
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
* ----------------------------------------------------------------------------
* @section DESCRIPTION
*
* ----------------------------------------------------------------------------
* @section HISTORY
* 1.0   20-feb-2012 : Initial version
*
*****************************************************************************/
#include <stdio.h>
#include "nxLog.h"
#include <nxEnsure.h>
#include <string.h>

#include "scp.h"
#include "ax_api.h"
#include "ax_util.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "axHostCrypto.h"
#ifdef TGT_A70CU
#include "hcAsn.h"
#endif

#define MAX_SIGNATURE 128

#ifndef AX_SF
#ifdef INS_AX_ECC_GENERATE_KEY
U16 ECC_GenerateKey(SST_Item_t keyId)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 identifier = 0x00;
    U8 index = 0;
    U16 rv = SW_OK;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_GENERATE_KEY;
    pApdu->p1    = 0x01;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

#ifdef TGT_EDEV
    rv = translatePairKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    if (rv == SW_OK)
    {
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

        scp_Transceive(pApdu, SCP_MODE);

        // no response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;

}

#endif /* INS_AX_ECC_GENERATE_KEY */

#ifdef INS_AX_ECC_SIGN

/**
* @function ECDSA_Sign
* @description Signs a given hash using the keypair keyId.
* @param keyId         The identifier of the keyPair
* @param hash         Pointer to the provided hash (or any other bytestring).
* @param hashLen       Length of the provided hash.
* @param signature    Pointer to the computed signature (in plain format).
* @param signatureLen Pointer to the length of the computed signature.
* @return status
*/
U16 ECDSA_Sign(SST_Item_t keyId, U8 *hash, U16 hashLen, U8 *signature, U16 *signatureLen)
{
    U8 isOk = 0;
    U16 rv = SW_OK;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 identifier = 0x00;
    U8 index = 0;
#ifdef TGT_A70CU
    U8 asnSignature[MAX_SIGNATURE];
    U16 asnSignatureLen = sizeof(asnSignature);
#endif

    ENSURE_OR_GO_EXIT(hash != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);
    ENSURE_OR_GO_EXIT(signatureLen != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_SIGN;
    pApdu->p1    = P1_ECDSA_SIGN;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

#ifdef TGT_EDEV
    rv = translatePairKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    if (rv == SW_OK)
    {
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_HASH, hashLen, hash);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
#ifdef TGT_A70CU
            U16 retval = ParseResponse(pApdu, TAG_SIGNATURE, &asnSignatureLen, asnSignature);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                rv = hcAsnToPlainEcc256(signature, signatureLen, asnSignature, asnSignatureLen);
            }
#else
            U16 retval = ParseResponse(pApdu, TAG_SIGNATURE, signatureLen, signature);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
#endif
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif /* INS_AX_ECC_SIGN */

#ifdef TGT_A70CU
/**
* @function ECC_SignData
* @description Signs data using the keypair keyId (according to GBCS)
* @param keyId         The identifier of the keyPair
* @param data          Pointer to data
* @param dataLen       Length of the provided data.
* @param signature     Pointer to the computed signature (in plain format)
* @param signatureLen  Pointer to the length of the computed signature.
* @return status
*/
U16 ECC_SignData(SST_Item_t keyId, U8 *data, U16 dataLen, U8 *signature, U16 *signatureLen)
{
    U8 isOk = 0;
    U16 rv = SW_OK;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 identifier = 0x00;
    U8 index = 0;
    U8 sha256StateBuffer[SHA256_STATE_SIZE];
    U32 numProcessedBytes = 0;
    S32 nRet = 0;
    U8 asnSignature[MAX_SIGNATURE];
    U16 asnSignatureLen = sizeof(asnSignature);

    ENSURE_OR_GO_EXIT(data != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);
    ENSURE_OR_GO_EXIT(signatureLen != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_SIGN;
    pApdu->p1    = P1_SIGN_PARTIAL_HASH;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;

    memset(sha256StateBuffer, 0x00, sizeof(sha256StateBuffer));

    nRet = HOST_SHA256_GetPartialHash(data, dataLen, sha256StateBuffer, &numProcessedBytes);
    if (nRet != HOST_CRYPTO_OK)
    {
        rv = ERR_CRYPTO_ENGINE_FAILED;
    }

    // printf("[DEBUG]dataLen: %d, numProcessedBytes: %d\r\n", dataLen, numProcessedBytes);

    if (rv == SW_OK)
    {
        U8 buf[2];

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_STATE, SHA256_STATE_SIZE, sha256StateBuffer);
        buf[0] = (numProcessedBytes >> 8) & 0x00FF;
        buf[1] = numProcessedBytes & 0x00FF;
        AddTlvItem(pApdu, TAG_SIZE, 2, (U8*)buf);
        AddTlvItem(pApdu, TAG_INPUT_DATA, dataLen - (U16)numProcessedBytes, data+numProcessedBytes);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U16 retval = ParseResponse(pApdu, TAG_SIGNATURE, &asnSignatureLen, asnSignature);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                rv = hcAsnToPlainEcc256(signature, signatureLen, asnSignature, asnSignatureLen);
            }
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}

/**
* @function ECC_SignPartialHash
* @description Signs data using the keypair keyId (according to GBCS). The signature is based upon an internal hash state and the remaining data.
* @param keyId         The identifier of the keyPair
* @param sha256StateBuffer     Internal state of SHA256 corresponding to data hashed so far
* @param sha256StateBufferLen  Length of sha256StateBuffer, must be SHA256_STATE_SIZE
* @param numProcessedBytes     Data hashed so far, must be multiple of 64
* @param data          Pointer to data still to be hashed
* @param dataLen       Length of data still to be hashed
* @param signature     Pointer to the computed signature (in plain format)
* @param signatureLen  Pointer to the length of the computed signature.
* @return status
*/
U16 ECC_SignPartialHash(SST_Item_t keyId, U8 *sha256StateBuffer, U16 sha256StateBufferLen, U16 numProcessedBytes, U8 *data, U16 dataLen, U8 *signature, U16 *signatureLen)
{
    U8 isOk = 0;
    U16 rv = SW_OK;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 identifier = 0x00;
    U8 index = 0;

    U8 asnSignature[MAX_SIGNATURE];
    U16 asnSignatureLen = sizeof(asnSignature);

    ENSURE_OR_GO_EXIT(data != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);
    ENSURE_OR_GO_EXIT(signatureLen != NULL);

    if (sha256StateBufferLen != SHA256_STATE_SIZE) {return ERR_API_ERROR;}
    // SHA256_STATE_SIZE<<1 == SHA256 block size == 64 byte
    if ( (numProcessedBytes % (SHA256_STATE_SIZE<<1)) != 0 ) {return ERR_API_ERROR;}

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_SIGN;
    pApdu->p1    = P1_SIGN_PARTIAL_HASH;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;

    if (rv == SW_OK)
    {
        U8 buf[2];

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_STATE, sha256StateBufferLen, sha256StateBuffer);
        buf[0] = (numProcessedBytes >> 8) & 0x00FF;
        buf[1] = numProcessedBytes & 0x00FF;
        AddTlvItem(pApdu, TAG_SIZE, 2, (U8*)buf);
        AddTlvItem(pApdu, TAG_INPUT_DATA, dataLen, data);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U16 retval = ParseResponse(pApdu, TAG_SIGNATURE, &asnSignatureLen, asnSignature);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                rv = hcAsnToPlainEcc256(signature, signatureLen, asnSignature, asnSignatureLen);
            }
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif // TGT_A70CU
#endif // AX_SF

#ifdef INS_AX_ECC_SIGN
/**
* @function ECDSA_Verify
* @description Verifies the signature of a given hash against the given signature.
* @param keyId         The identifier of the keyPair
* @param hash          Pointer to the provided hash (or any other bytestring).
* @param hashLen       Length of the provided hash.
* @param signature     Pointer to the provided signature in [GBCS] format.
* @param signatureLen  Length of the provided signature.
* @param ok            Pointer to the computed result of the verification. (0 for failure; 1 for success)
* @return status.
*/
U16 ECDSA_Verify(SST_Item_t keyId, U8 *hash, U16 hashLen, U8 *signature, U16 signatureLen, U8 *ok)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 isOk = 0;
    U8 identifier = 0x00;
    U8 index = 0;
    U8 asnSignature[MAX_SIGNATURE];
    U16 asnSignatureLen = sizeof(asnSignature);

    ENSURE_OR_GO_EXIT(hash != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);
    ENSURE_OR_GO_EXIT(ok != NULL);

    *ok = 0;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_SIGN;
    pApdu->p1    = P1_VERIFY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

#ifdef TGT_EDEV
    if (signatureLen > MAX_SIGNATURE)
    {
        rv = ERR_API_ERROR;
    }
    else
    {
        memcpy(asnSignature, signature, signatureLen);
        asnSignatureLen = signatureLen;

        rv = translatePublicKeyItemToIdentityIndexPair(keyId, &identifier, &index);
    }
#else
    rv = hcPlainToAsnEcc256(asnSignature, &asnSignatureLen, signature, signatureLen);

    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    if (rv == SW_OK)
    {
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8 *)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_HASH, hashLen, hash);
        AddTlvItem(pApdu, TAG_SIGNATURE, asnSignatureLen, asnSignature);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U8 result[20];
            U16 length = sizeof(result);

            U16 retval = ParseResponse(pApdu, TAG_VERIFICATION, &length, result);

            if ((retval != SW_OK) || (length != 1) || ((result[0] != AX_VERIFY_SUCCESS) && (result[0] != AX_VERIFY_FAILURE)))
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                if (result[0] == AX_VERIFY_SUCCESS)
                {
                    *ok = 1;
                }
                else
                {
                    *ok = 0;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif /* INS_AX_ECC_SIGN */

#ifdef TGT_A70CU
/**
* @function ECDSA_VerifyWithKey
* @description Verifies the signature of a given hash against the given key and signature.
* @param publicKey     The key (data) to use for the verifcation
* @param hash          Pointer to the provided hash (or any other bytestring).
* @param hashLen       Length of the provided hash.
* @param signature     Pointer to the provided signature in [GBCS] format.
* @param signatureLen  Length of the provided signature.
* @param ok            Pointer to the computed result of the verification. (0 for failure; 1 for success)
* @return status.
*/
U16 ECDSA_VerifyWithKey(U8* pubKey, U16 pubKeyLen, U8 *hash, U16 hashLen, U8 *signature, U16 signatureLen, U8 *ok)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 isOk = 0;
    U8 asnSignature[MAX_SIGNATURE];
    U16 asnSignatureLen = sizeof(asnSignature);

    ENSURE_OR_GO_EXIT(pubKey != NULL);
    ENSURE_OR_GO_EXIT(hash != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);
    ENSURE_OR_GO_EXIT(ok != NULL);

    *ok = 0;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_SIGN;
    pApdu->p1    = P1_VERIFY_WITH_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    rv = hcPlainToAsnEcc256(asnSignature, &asnSignatureLen, signature, signatureLen);

    if (rv == SW_OK)
    {
        AddTlvItem(pApdu, TAG_ECC_PUBLIC_KEY, pubKeyLen, pubKey);
        AddTlvItem(pApdu, TAG_HASH, hashLen, hash);
        AddTlvItem(pApdu, TAG_SIGNATURE, asnSignatureLen, asnSignature);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U8 result[20];
            U16 length = sizeof(result);

            U16 retval = ParseResponse(pApdu, TAG_VERIFICATION, &length, result);

            if ((retval != SW_OK) || (length != 1) || ((result[0] != AX_VERIFY_SUCCESS) && (result[0] != AX_VERIFY_FAILURE)))
            {
                rv = ERR_WRONG_RESPONSE;
            }
            else
            {
                if (result[0] == AX_VERIFY_SUCCESS)
                {
                    *ok = 1;
                }
                else
                {
                    *ok = 0;
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif

#ifdef TGT_EDEV
#ifndef AX_SF
/**
* @function ECDH_GenerateSharedSecret
* @description Generates a shared secret ECC point using private key from secure storage and a given public key.
* @param keyId              The identifier of the keyPair
* @param publicKey          Pointer to the given public key.
* @param publicKeyLen       Length of the given public key.
* @param sharedSecret       Pointer to the computed shared secret.
* @param sharedSecretLen    Pointer to the length of the computed shared secret.
* @return status
*/
U16 ECDH_GenerateSharedSecret(SST_Item_t keyId, U8 *publicKey, U16 publicKeyLen, U8 *sharedSecret, U16 *sharedSecretLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 isOk = 0;
    U8 identifier = 0x00;
    U8 index = 0;

    ENSURE_OR_GO_EXIT(publicKey != NULL);
    ENSURE_OR_GO_EXIT(sharedSecret != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_ECDH;
    pApdu->p1    = P1_GENERATE_SHARED_SECRET;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translatePairKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    if (rv == SW_OK)
    {
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8 *)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_ECC_PUBLIC_KEY, publicKeyLen, publicKey);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U16 retval = ParseResponse(pApdu, TAG_SHARED_SECRET, sharedSecretLen, sharedSecret);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif // AX_SF
#endif // TGT_EDEV

#ifdef TGT_A70CU
/**
* @function ECDH_GenerateSharedSecretToStore
* @description Generates a shared secret ECC point, to be securely stored, using private key from secure storage and a given public key.
* @param keyId              The identifier of the keyPair
* @param publicKey          Pointer to the given public key.
* @param publicKeyLen       Length of the given public key.
* @param secretId           The identifier of the shared secret ECC point's secure storage location
* @return status
*/
U16 ECDH_GenerateSharedSecretToStore(SST_Item_t keyId, U8 *publicKey, U16 publicKeyLen, SST_Item_t secretId)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 keyIdentifier = 0x00;
    U8 keyIndex = 0;
    U8 secretIdentifier = 0x00;
    U8 secretIndex = 0;

    ENSURE_OR_GO_EXIT(publicKey != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_ECDH;
    pApdu->p1    = P1_GENERATE_SHARED_SECRET_TO_STORE;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    keyIdentifier = (U8) (keyId >> 8);
    keyIndex = (U8) keyId;
    secretIdentifier = (U8) (secretId >> 8);
    secretIndex = (U8) secretId;

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8 *)&keyIdentifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&keyIndex);

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER2, 1, (U8 *)&secretIdentifier);
    AddTlvItem(pApdu, TAG_SST_INDEX2, 1, (U8*)&secretIndex);

    AddTlvItem(pApdu, TAG_ECC_PUBLIC_KEY, publicKeyLen, publicKey);

    scp_Transceive(pApdu, SCP_MODE);

    // no response data expected
    rv = CheckNoResponseData(pApdu);

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}

/**
* @function ECDH_DeriveKeyData
* @description Generates key data from a stored shared secret ECC point and other info according to the NIST SP-800-56a Rev2 Single Step KDF
* @param secretId           The identifier of the shared secret ECC point
* @param otherInfo          Pointer to the otherInfo.
* @param otherInfoLen       Length of the given otherInfo
* @param numBytes           Amount (in bytes) of requested key data
* @param keyData            Pointer to retrieved key data
* @param keyDataLen         Length of the retrieved key data
* @return status
*/
U16 ECDH_DeriveKeyData(SST_Item_t secretId, U8 *otherInfo, U16 otherInfoLen, U16 numBytes, U8 *keyData, U16 *keyDataLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 secretIdentifier = 0x00;
    U8 secretIndex = 0;
    U8 isOk = 0;

    ENSURE_OR_GO_EXIT(otherInfo != NULL);
    ENSURE_OR_GO_EXIT(keyData != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_ECC_ECDH;
    pApdu->p1    = P1_DERIVE_KEY_DATA;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    secretIdentifier = (U8) (secretId >> 8);
    secretIndex = (U8) secretId;

    if (rv == SW_OK)
    {
        U8 tmp[2];
        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8 *)&secretIdentifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8 *)&secretIndex);
        AddTlvItem(pApdu, TAG_INPUT_DATA, otherInfoLen, otherInfo);
        tmp[0] = numBytes >> 8;
        tmp[1] = numBytes & 0xff;
        AddTlvItem(pApdu, TAG_SIZE, 2, tmp);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);

        if (isOk == 1)
        {
            rv = ParseResponse(pApdu, TAG_OUTPUT_DATA, keyDataLen, keyData);
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif
