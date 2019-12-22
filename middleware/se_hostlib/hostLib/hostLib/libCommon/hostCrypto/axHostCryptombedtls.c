/**
 * @file axHostCryptoStubs.c
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
 * Host Crypto stub implementation for the A7-series
 *
 * @par HISTORY
 *
 */

#include "axHostCrypto.h"
#include "ax_util.h"
#include "sm_types.h"
#include "sm_printf.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <mbedtls/aes.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#ifdef MBEDTLS_CMAC_C
#   include <mbedtls/cmac.h>
#endif
#include <mbedtls/sha256.h>
#include <mbedtls/sha1.h>
#include "mbedtls/entropy_poll.h"
#include <mbedtls/version.h>

static mbedtls_cipher_context_t cipher_ctx;
// Provide here your own implementation (In case crypto is required and OpenSSL is not available)
S32 HOST_SHA1_Get(const U8 *msg, U32 msgLen, U8 *pHash)
{
    mbedtls_sha1_context sha1_ctx;

    mbedtls_sha1_init(&sha1_ctx);
#if (MBEDTLS_VERSION_NUMBER <= 0x02060000)
    mbedtls_sha1_starts(&sha1_ctx);
    mbedtls_sha1_update(&sha1_ctx,msg,msgLen);
    mbedtls_sha1_finish(&sha1_ctx,pHash);
#else
    mbedtls_sha1_starts_ret(&sha1_ctx);
    mbedtls_sha1_update_ret(&sha1_ctx,msg,msgLen);
    mbedtls_sha1_finish_ret(&sha1_ctx,pHash);
#endif
    mbedtls_sha1_free(&sha1_ctx);

    return HOST_CRYPTO_OK;
}

S32 HOST_SHA256_Get(const U8 *msg, U32 msgLen, U8 *pHash)
{
    mbedtls_sha256_context sha256_ctx;
#if (MBEDTLS_VERSION_NUMBER <= 0x02060000)
    mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts(&sha256_ctx,0);
    mbedtls_sha256_update(&sha256_ctx,msg,msgLen);
    mbedtls_sha256_finish(&sha256_ctx,pHash);
    mbedtls_sha256_free(&sha256_ctx);
#else
    mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts_ret(&sha256_ctx,0);
    mbedtls_sha256_update_ret(&sha256_ctx,msg,msgLen);
    mbedtls_sha256_finish_ret(&sha256_ctx,pHash);
    mbedtls_sha256_free(&sha256_ctx);
#endif

    return HOST_CRYPTO_OK;
}

S32 HOST_AES_ECB_DECRYPT(U8 *plainText, const U8 *cipherText, const U8 *decryptKey, U32 decryptKeyLen)
{
    mbedtls_aes_context ctx;
    int keyLenBits = decryptKeyLen * 8;
    int nRet = 0;

    // This works assuming the plaintext has the same size as the key
    // NOTE: AES_set_encrypt_key returns 0 upon success
    nRet = mbedtls_aes_setkey_dec(&ctx, decryptKey, keyLenBits);
    if (nRet != 0)
    {
        return HOST_CRYPTO_ERROR;
    }

    nRet = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, cipherText, plainText);
    if (nRet != 0)
    {
        return HOST_CRYPTO_ERROR;
    }

    return HOST_CRYPTO_OK;

}

S32 HOST_AES_ECB_ENCRYPT(const U8 *plainText, U8 *cipherText, const U8 *encryptKey, U32 encryptKeyLen)
{
    mbedtls_aes_context ctx;
    int keyLenBits = encryptKeyLen * 8;
    int nRet = 0;

    // This works assuming the plaintext has the same size as the key
    // NOTE: AES_set_encrypt_key returns 0 upon success
    nRet = mbedtls_aes_setkey_enc(&ctx, encryptKey, keyLenBits);
    if (nRet != 0)
    {
        return HOST_CRYPTO_ERROR;
    }

    nRet = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, plainText, cipherText);
    if (nRet != 0)
    {
        return HOST_CRYPTO_ERROR;
    }

    return HOST_CRYPTO_OK;
}

S32 HOST_CMAC_Get(const U8 *pKey, U8 keySizeInBytes, const U8* pMsg, U32 msgLen, U8 *pMac)
{
    int ret;
    axHcCmacCtx_t *ctx;

    ret = HOST_CMAC_Init(&ctx, pKey, keySizeInBytes);
    if (ret == HOST_CRYPTO_OK)
    {
        ret = HOST_CMAC_Update(ctx, pMsg, msgLen);
        if (ret == HOST_CRYPTO_OK)
        {
            ret = HOST_CMAC_Finish(ctx,pMac);
        }
    }

    return ret;
}

S32 HOST_CMAC_Init(axHcCmacCtx_t **ctx, const U8 *pKey,  U8 keySizeInBytes)
{
    int ret;
    const mbedtls_cipher_info_t *cipher_info;
    *ctx = &cipher_ctx;
    /*TODO CHECK mbedtls cmac does not suppot cbc but only ecb. mbedtls_cipher_cmac_starts returns error */
    cipher_info = mbedtls_cipher_info_from_type( MBEDTLS_CIPHER_AES_128_ECB);
    if (cipher_info != NULL)
    {
        mbedtls_cipher_init(*ctx);
        ret = mbedtls_cipher_setup( *ctx, cipher_info );
        if (ret == 0)
        {
#ifdef MBEDTLS_CMAC_C
            ret = mbedtls_cipher_cmac_starts(*ctx,pKey,(keySizeInBytes * 8));
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
        return ERR_MEMORY;
    }

    return ret;
}

S32 HOST_CMAC_Update(axHcCmacCtx_t *ctx, const U8 *pMsg, U32 msgLen)
{
    int ret = 1;

#ifdef MBEDTLS_CMAC_C
    ret = mbedtls_cipher_cmac_update(ctx, pMsg, msgLen);
#endif
    if (ret == 0)
    {
        ret = HOST_CRYPTO_OK;
    }

    return ret;
}

S32 HOST_CMAC_Finish(axHcCmacCtx_t *ctx, U8 *pMac)
{
    int ret = 1;

#ifdef MBEDTLS_CMAC_C
    ret = mbedtls_cipher_cmac_finish(ctx,pMac);
#endif
    if (ret == 0)
    {
        ret = HOST_CRYPTO_OK;
    }
    mbedtls_cipher_free(ctx);

    return ret;

}

S32 HOST_AES_CBC_Process(const U8 *pKey, U32 keyLen, const U8 *pIv, U8 dir, const U8 *pIn, U32 inLen, U8 *pOut)
{
    int ret = HOST_CRYPTO_ERROR;
    mbedtls_aes_context aes_ctx;

    if (dir == HOST_ENCRYPT)
    {
        ret = mbedtls_aes_setkey_enc( &aes_ctx, pKey, 128 );
        if (ret == 0)
        {
            ret = mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, inLen, (unsigned char *)pIv, pIn, pOut);
            if (ret == 0)
            {
                ret = HOST_CRYPTO_OK;
            }
        }
    }
    else if (dir == HOST_DECRYPT)
    {
        ret = mbedtls_aes_setkey_dec( &aes_ctx, pKey, 128 );
        if (ret == 0)
        {
            ret = mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, inLen, (unsigned char *)pIv, pIn, pOut);
            if (ret == 0)
            {
                ret = HOST_CRYPTO_OK;
            }
        }
    }

    return ret;
}



S32 HOST_AES_ECB_Process(const U8 *pKey, U32 keyLen, const U8 *pIv, U8 dir, const U8 *pIn, U32 inLen, U8 *pOut)
{
	int ret = HOST_CRYPTO_ERROR;
	mbedtls_aes_context aes_ctx;

	if (dir == HOST_ENCRYPT)
	{
		ret = mbedtls_aes_setkey_enc(&aes_ctx, pKey, 128);
		if (ret == 0)
		{
			ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, pIn, pOut);
			if (ret == 0)
			{
				ret = HOST_CRYPTO_OK;
			}
		}
	}
	else if (dir == HOST_DECRYPT)
	{
		ret = mbedtls_aes_setkey_dec(&aes_ctx, pKey, 128);
		if (ret == 0)
		{
			ret = mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, pIn, pOut);
			if (ret == 0)
			{
				ret = HOST_CRYPTO_OK;
			}
		}
	}

	return ret;
}

S32 HOST_CMAC_Get_Des(const U8 *pKey, U8 keySizeInBytes, const U8 *pMsg, U32 msgLen, U8* pMac)
{
	return HOST_CRYPTO_ERROR;
}

S32 HOST_3DES_CBC_Process(const U8 *pKey, U32 keyLen, const U8 *pIv, U8 dir, const U8 *pIn, U32 inLen, U8 *pOut)
{
	return HOST_CRYPTO_ERROR;
}

S32 HOST_3DES_ECB_Process(const U8 *pKey, U32 keyLen, const U8 *pIv,
	U8 dir, const U8 *pIn, U32 inLen, U8 *pOut)
{
	return HOST_CRYPTO_ERROR;
}

S32 HOST_GetRandom(U32 inLen, U8 *pRandom)
{
    int nRet;

#ifdef MBEDTLS_CTR_DRBG_C
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_entropy_init( &entropy );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    nRet = mbedtls_ctr_drbg_seed( &ctr_drbg , mbedtls_entropy_func, &entropy,
                                  NULL,
                                  0);
    if (nRet == 0)
    {
        nRet = mbedtls_ctr_drbg_random((void*)&ctr_drbg,pRandom,inLen);
        if (nRet == 0)
        {
            nRet = HOST_CRYPTO_OK;
        }
    }

#elif AX_EMBEDDED
    size_t olen = 0;
    mbedtls_hardware_poll(NULL, pRandom, inLen, &olen);
    nRet = HOST_CRYPTO_OK;
#else
    nRet = HOST_CRYPTO_ERROR;
#endif

    return nRet;
}
