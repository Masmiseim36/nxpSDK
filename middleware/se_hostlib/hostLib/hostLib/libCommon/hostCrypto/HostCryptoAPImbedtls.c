/**
* @file HostCryptoAPImbedtls.c
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
* Host Crypto mbed TLS wrapper implementation
*
* @par HISTORY
*
*/

#include "HostCryptoAPI.h"
//#include "ax_util.h"
//#include "sm_types.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <mbedtls/aes.h>
#include <mbedtls/des.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#ifdef MBEDTLS_CMAC_C
#   include <mbedtls/cmac.h>
#endif
#include <mbedtls/cipher.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/pk.h>
#include <mbedtls/entropy_poll.h>

/*TODO check return paths and framework specified terminate paths are met */
#define AES_BLOCK_SIZE 16 /*TODO check if this can come from mbedtls */
static mbedtls_cipher_context_t cipher_ctx;

HLSE_RET_CODE   HLCRYPT_GetSupportedMechanisms(HLSE_MECHANISM_TYPE* mechanism, U32* mechanismLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (mechanismLen == NULL)
        return HLSE_ERR_API_ERROR;
#endif

    if (mechanism == NULL) {
        *mechanismLen = 11;
        return HLSE_SW_OK;
    }
    if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 11) {
        *mechanismLen = 11;
        return HLSE_ERR_BUF_TOO_SMALL;
    }

    *mechanismLen = 11;

    *mechanism++ = HLSE_SHA1;
    *mechanism++ = HLSE_SHA256;
    *mechanism++ = HLSE_AES_CMAC;
    *mechanism++ = HLSE_AES_ECB_ENCRYPT;
    *mechanism++ = HLSE_AES_ECB_DECRYPT;
    *mechanism++ = HLSE_AES_CBC_ENCRYPT;
    *mechanism++ = HLSE_AES_CBC_DECRYPT;
    *mechanism++ = HLSE_DES_ECB_ENCRYPT;
    *mechanism++ = HLSE_DES_ECB_DECRYPT;
    *mechanism++ = HLSE_DES_CBC_ENCRYPT;
    *mechanism++ = HLSE_DES_CBC_DECRYPT;

    return HLSE_SW_OK;

}

HLSE_RET_CODE HLCRYPT_Single_DES_CBC_Encrypt(U8 *key, U32 keylen,
    U8 *iv,
    U16 ivlen,
    U8 *inData,
    U32 inDatalen,
    U8 * outData,
    U32 *outDatalen)
{
    int nRet = HOST_CRYPTO_OK;
    int mbedtls_ret;
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (key == NULL || inData == NULL || outData == NULL || outDatalen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif
    mbedtls_des_context ctx;
    mbedtls_des_init(&ctx);
    mbedtls_ret = mbedtls_des_setkey_enc(&ctx, key);
    if (mbedtls_ret != 0)
    {
        nRet = HOST_CRYPTO_ERROR;
    }
    mbedtls_ret = mbedtls_des_crypt_cbc(&ctx,
        MBEDTLS_DES_ENCRYPT,
        inDatalen,
        iv,
        inData,
        outData);
    if (mbedtls_ret != 0)
    {
        nRet = HOST_CRYPTO_ERROR;
    }
    mbedtls_des_free(&ctx);
    return nRet;
}

HLSE_RET_CODE   HLCRYPT_Encrypt(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* outEncryptedData, U32* outEncryptedDataLen)
{

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (pMechanismType == NULL || inKey == NULL || inData == NULL || outEncryptedData == NULL || outEncryptedDataLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_AES_ECB_ENCRYPT) {
        for (;;) {
            mbedtls_aes_context ctx;
            int keyLenBits = inKeyLen * 8;
            int nRet = HOST_CRYPTO_OK;
            int mbedtls_ret;

            mbedtls_aes_init(&ctx);
            // This works assuming the plaintext has the same size as the key
            // NOTE: AES_set_encrypt_key returns 0 upon success
            mbedtls_ret = mbedtls_aes_setkey_enc(&ctx, inKey, keyLenBits);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }

            mbedtls_ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, inData, outEncryptedData);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_aes_free(&ctx);
            return nRet;
        }
    }
    else if (pMechanismType->mechanism == HLSE_AES_CBC_ENCRYPT) {
        for (;;) {
            int nRet = HOST_CRYPTO_OK;
            int mbedtls_ret;
            int keyLenBits = inKeyLen * 8;
            mbedtls_aes_context ctx;
            unsigned char * pIv = (unsigned char *)pMechanismType->pParameter;

            mbedtls_aes_init(&ctx);
            mbedtls_ret = mbedtls_aes_setkey_enc(&ctx, inKey, keyLenBits);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }

            mbedtls_ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, inDataLen, pIv, inData, outEncryptedData);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_aes_free(&ctx);
            return nRet;
        }
    }

    else if (pMechanismType->mechanism == HLSE_DES_CBC_ENCRYPT) {
        for (;;) {
            int nRet = HOST_CRYPTO_OK;
            int mbedtls_ret;
            //int keyLenBits = inKeyLen * 8;
            mbedtls_des3_context ctx;

            unsigned char * pIv = (unsigned char *)pMechanismType->pParameter;

            mbedtls_des3_init(&ctx);
            mbedtls_ret = mbedtls_des3_set2key_enc(&ctx, inKey);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }

            mbedtls_ret = mbedtls_des3_crypt_cbc(&ctx, MBEDTLS_DES_ENCRYPT, inDataLen, pIv, inData, outEncryptedData);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_des3_free(&ctx);
            return nRet;
        }
    }
    else if (pMechanismType->mechanism == HLSE_DES_ECB_ENCRYPT) {
        for (;;) {
            int nRet = HOST_CRYPTO_OK;
            int mbedtls_ret;
            mbedtls_des_context ctx;
            mbedtls_des_init(&ctx);
            mbedtls_ret = mbedtls_des_setkey_enc(&ctx, inKey);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_ret = mbedtls_des_crypt_ecb(&ctx, inData, outEncryptedData);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_des_free(&ctx);
            return nRet;
        }

    }
    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLCRYPT_Decrypt(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* outDecryptedData, U32* outDecryptedDataLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (pMechanismType == NULL || inKey == NULL || inData == NULL || outDecryptedData == NULL || outDecryptedDataLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_AES_ECB_DECRYPT) {
        for (;;) {
            mbedtls_aes_context ctx;
            int keyLenBits = inKeyLen * 8;
            int mbedtls_ret;
            int nRet = HOST_CRYPTO_OK;

            mbedtls_aes_init(&ctx);
            // This works assuming the plaintext has the same size as the key
            // NOTE: AES_set_encrypt_key returns 0 upon success
            mbedtls_ret = mbedtls_aes_setkey_dec(&ctx, inKey, keyLenBits);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }

            // AES_ecb_encrypt has return type void
            mbedtls_ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, inData, outDecryptedData);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_aes_free(&ctx);
            return nRet;
        }
    }
    else if (pMechanismType->mechanism == HLSE_AES_CBC_DECRYPT) {
        for (;;) {
            int nRet = HOST_CRYPTO_OK;
            int mbedtls_ret;
            int keyLenBits = inKeyLen * 8;
            unsigned char * pIv = (unsigned char *)pMechanismType->pParameter;
            mbedtls_aes_context ctx;

            mbedtls_aes_init(&ctx);
            if (inKeyLen != AES_BLOCK_SIZE) {
                // printf("Unsupported key length for HOST_AES_CBC_Process\r\n");
                nRet = HLSE_ERR_API_ERROR;
                break;
            }

            // iv is passed in the pParameter
            if (pMechanismType->pParameter == NULL) {
                nRet = HLSE_ERR_API_ERROR;
                break;
            }

            if ((inDataLen % AES_BLOCK_SIZE) != 0) {
                // printf("Input data are not block aligned for HOST_AES_CBC_Process\r\n");
                nRet = HLSE_ERR_API_ERROR;
                break;
            }

            // EVP_EncryptInit_ex returns 0 on failure and 1 upon success
            mbedtls_ret = mbedtls_aes_setkey_dec(&ctx, inKey, keyLenBits);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, inDataLen, pIv, inData, outDecryptedData);
            if (mbedtls_ret != 0)
            {
                nRet = HOST_CRYPTO_ERROR;
                break;
            }
            mbedtls_aes_free(&ctx);
            return nRet;
        }
    }
    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLCRYPT_Digest(HLSE_MECHANISM_INFO* pMechanismType,
    U8* inData, U32 inDataLen,
    U8* outDigest, U32* outDigestLen)
{

    return HLSE_ERR_API_ERROR;
}


HLSE_RET_CODE   HLCRYPT_Sign(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* outSignature, U32* outSignatureLen)
{

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (pMechanismType == NULL || inKey == NULL || inData == NULL || outSignature == NULL || outSignatureLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_AES_CMAC) {
        int ret;
        //mbedtls_cipher_context_t cipher_ctx;
        const mbedtls_cipher_info_t *cipher_info;
        /*TODO CHECK mbedtls cmac does not suppot cbc but only ecb. mbedtls_cipher_cmac_starts returns error */
        cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
        if (cipher_info != NULL)
        {
            mbedtls_cipher_init(&cipher_ctx);
            ret = mbedtls_cipher_setup(&cipher_ctx, cipher_info);
            if (ret == 0)
            {
#ifdef MBEDTLS_CMAC_C
                ret = mbedtls_cipher_cmac_starts(&cipher_ctx, inKey, (inKeyLen * 8));
                if (ret == 0)
                {
                    ret = mbedtls_cipher_cmac_update(&cipher_ctx, inData, inDataLen);
                    if (ret == 0)
                    {
                        ret = mbedtls_cipher_cmac_finish(&cipher_ctx, outSignature);
                        if (ret == 0)
                        {
                            ret = HOST_CRYPTO_OK;
                        }
                    }
                }
#else
                ret = 1;
#endif
            }
            mbedtls_cipher_free(&cipher_ctx);
        }
        else
        {
            return 0;
        }
        return ret;
    }
    else if (pMechanismType->mechanism == HLSE_ECDSA_SIGN)
    {
        int status;
        size_t sigLen = *outSignatureLen;
        status = (mbedtls_ecdsa_write_signature(mbedtls_pk_ec(*(mbedtls_pk_context *)inKey), MBEDTLS_MD_SHA256,
            inData, inDataLen,
            outSignature, &sigLen,
            NULL,
            NULL));
        *outSignatureLen = sigLen;
        if (status == 0)
        {
            return HLSE_SW_OK;
        }
        else
        {
            return HLSE_ERR_CRYPTO_ENGINE_FAILED;
        }

    }
    else
    {
        return HLSE_ERR_API_ERROR;
    }

}

HLSE_RET_CODE   HLCRYPT_Verify(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen,
    U8* inData, U32 inDataLen,
    U8* inSignature, U32 inSignatureLen)
{

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (pMechanismType == NULL || inKey == NULL || inData == NULL || inSignature == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_ECDSA_VERIFY)
    {
        int status;
        status = (mbedtls_ecdsa_read_signature(mbedtls_pk_ec(*(mbedtls_pk_context *)inKey),
            inData, inDataLen,
            inSignature, inSignatureLen));

        if (status == 0)
        {
            return HLSE_SW_OK;
        }
        else
        {
            return HLSE_ERR_CRYPTO_ENGINE_FAILED;
        }

    }
    else
    {
        return HLSE_ERR_API_ERROR;
    }
}
HLSE_RET_CODE   HLCRYPT_SignInit(HLSE_MECHANISM_INFO* pMechanismType, U8* inKey, U32 inKeyLen, HLSE_CONTEXT_HANDLE* hContext)
{

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (pMechanismType == NULL || inKey == NULL || hContext == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_AES_CMAC) {
        int ret;
        const mbedtls_cipher_info_t *cipher_info;
        *hContext = &cipher_ctx;
        /*TODO CHECK mbedtls cmac does not suppot cbc but only ecb. mbedtls_cipher_cmac_starts returns error */
        cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
        if (cipher_info != NULL)
        {
            mbedtls_cipher_init(*hContext);
            ret = mbedtls_cipher_setup(*hContext, cipher_info);
            if (ret == 0)
            {
#ifdef MBEDTLS_CMAC_C
                ret = mbedtls_cipher_cmac_starts(*hContext, inKey, (inKeyLen * 8));
#else
                ret = 1;
#endif
                if (ret == 0)
                {
                    ret = HOST_CRYPTO_OK;
                }
            }
        }
        else
        {
            return 0;
        }
        return ret;
    }
    else
    {
        return HLSE_ERR_API_ERROR;
    }

}

HLSE_RET_CODE   HLCRYPT_SignUpdate(HLSE_CONTEXT_HANDLE hContext, U8* inDataPart, U32 inDataPartLen)
{
    int ret;
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (inDataPart == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

#ifdef MBEDTLS_CMAC_C
    ret = mbedtls_cipher_cmac_update(hContext, inDataPart, inDataPartLen);
#else
    ret = 1;
#endif
    if (ret == 0)
    {
        ret = HOST_CRYPTO_OK;
    }

    return ret;
}

HLSE_RET_CODE   HLCRYPT_SignFinal(HLSE_CONTEXT_HANDLE hContext, U8* outSignature, U32* outSignatureLen)
{
    int ret;

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (outSignature == NULL || outSignatureLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

#ifdef MBEDTLS_CMAC_C
    ret = mbedtls_cipher_cmac_finish(hContext, outSignature);
#else
    ret = 1;
#endif
    if (ret == 0)
    {
        ret = HOST_CRYPTO_OK;
    }
    mbedtls_cipher_free(hContext);

    return ret;
}


HLSE_RET_CODE    HLCRYPT_GetRandom(U32 inLen, U8 * pRandom)
{
    int nRet;
#if AX_EMBEDDED
    size_t olen = 0;

#elif defined( MBEDTLS_CTR_DRBG_C)
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
#endif

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (pRandom == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif
#if AX_EMBEDDED
    mbedtls_hardware_poll(NULL, pRandom, inLen, &olen);
    nRet = HOST_CRYPTO_OK;
#elif defined( MBEDTLS_CTR_DRBG_C)
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    nRet = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
        NULL,
        0);
    if (nRet == 0)
    {
        nRet = mbedtls_ctr_drbg_random((void*)&ctr_drbg, pRandom, inLen);
        if (nRet == 0)
        {
            nRet = HOST_CRYPTO_OK;
        }
    }

#else
    nRet = HOST_CRYPTO_ERROR;
#endif

    return nRet;
}
