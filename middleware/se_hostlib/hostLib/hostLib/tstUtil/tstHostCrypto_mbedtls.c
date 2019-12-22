/**
 * @file tstHostCrypto.c
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
 *  Module implementing host based crypto functionality used in example programs.
 * This module relies on the availability of mbed TLS on the Host plaform.
 * @par HISTORY
 *
 */

/*******************************************************************
* project specific include files
*******************************************************************/

#include "tstHostCrypto.h"
#include "tst_sm_util.h"
#include "sm_printf.h"
#include "string.h"
#include "mbedtls/aes.h"
#include "hkdf_mbedtls.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/rsa.h"
#include "mbedtls/error.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/md.h"
#include "mbedtls/platform.h"
#include "assert.h"
#include "HostCryptoAPI.h"
#include "string.h" /* For size_t */

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#include "fsl_trng.h"
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
#include "fsl_rnga.h"
#elif defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)
#include "fsl_rng.h"
#endif

/*******************************************************************
* global variables and struct definitions
*******************************************************************/

static int fast_aes_wrap(const uint8_t *kek, int n, const uint8_t *plain, uint8_t *cipher);

#if AX_EMBEDDED
static int HOSTCRYPTO_GetRandom(void *pCtx, unsigned char *output, size_t len);
#endif

static mbedtls_ecp_group_id curveType2GroupID (ECCCurve_t curveType) {
    switch (curveType) {
    case ECCCurve_NIST_P192:
        return MBEDTLS_ECP_DP_SECP192R1;
    case ECCCurve_NIST_P224:
        return MBEDTLS_ECP_DP_SECP224R1;
    case ECCCurve_NIST_P256:
        return MBEDTLS_ECP_DP_SECP256R1;
    case ECCCurve_BrainPoolP256r1:
        return MBEDTLS_ECP_DP_BP256R1;
    default:
        assert(0);
    }
    return MBEDTLS_ECP_DP_NONE;
}
// TODO: Check on buffer size passed as argument
U16 HOSTCRYPTO_Sign(EC_KEY* pKey, U8* pInputData, U16 inputLength, U8* pSignature, U16* pSignatureLength, U8 signatureFormat)
{
    #ifdef __FUNCTION__
        sm_printf(CONSOLE, "Not Implemented => '"  __FUNCTION__  "'.\n");
    #endif
    return 0;
}

U16 HOSTCRYPTO_ECC_ComputeSharedSecret(EC_KEY *pKey, U8 *pubKey, U16 pubKeyLen, U8 *pSharedSecretData, U16 *pSharedSecretDataLen)
{
    int retval;
    int keyLen = 0;
    int sharedSecretLen;
    int sharedSecretLen_Derived;
    U16 nStatus = SW_OK;
    mbedtls_mpi rawSharedData;
    mbedtls_ecp_point ecp_point;
    const mbedtls_ecp_curve_info *p_curve_info = NULL;
    mbedtls_ecp_keypair * pEcCtx = mbedtls_pk_ec( *pKey );

    mbedtls_ecp_point_init(&ecp_point);
    mbedtls_mpi_init(&rawSharedData);
    /* Compute the size of the shared secret */
    sharedSecretLen = mbedtls_mpi_size( &(pEcCtx->d) );
    if (sharedSecretLen > *pSharedSecretDataLen)
    {
        return ERR_API_ERROR;
    }

     // convert external public key data to POINT
    // external public key curve == local curve
    // data has leading 0x04 (uncompressed point representation)
    retval = mbedtls_ecp_point_read_binary( &(pEcCtx->grp), &ecp_point,
                           pubKey, pubKeyLen );
    if (retval != 0)
    {
        return ERR_GENERAL_ERROR;
    }
    /* Compute the shared secret, no KDF is applied */
    retval = mbedtls_ecdh_compute_shared( &(pEcCtx->grp), &rawSharedData,
                         &ecp_point, &(pEcCtx->d),
                         NULL,
                         NULL );
    if (retval != 0)
    {
        return ERR_GENERAL_ERROR;
    }

    mbedtls_ecp_point_free(&ecp_point);

    sharedSecretLen_Derived = mbedtls_mpi_size( &(rawSharedData) );
    if (sharedSecretLen_Derived != sharedSecretLen)
    {
        mbedtls_mpi_free(&(rawSharedData));
        return ERR_GENERAL_ERROR;
    }

    p_curve_info = mbedtls_ecp_curve_info_from_grp_id( pEcCtx->grp.id );
    keyLen = ((p_curve_info->bit_size + 7))/8;
    //get sharedsecret len from rawSharedData and check with sharedsecretlen. must be same
    if (keyLen > *pSharedSecretDataLen)
    {
        mbedtls_mpi_free(&(rawSharedData));
        return ERR_BUF_TOO_SMALL;
    }
    *pSharedSecretDataLen = keyLen;
    retval = mbedtls_mpi_write_binary(&rawSharedData,pSharedSecretData,keyLen);
    if (retval != 0)
    {
        return ERR_GENERAL_ERROR;
    }


    mbedtls_mpi_free(&(rawSharedData));
    return nStatus;
}

/**
 * Extract the public key - as a byte array in uncompress format - from an ECC key
 * @param[in] pKey Reference to ECC key.
 * @param[in,out] pPublicKeyData IN: Buffer to contain public key; OUT: Public key
 * @param[out] pPublicKeyLen Length of public key \p pPublicKeyData retrieved
 * @param[in] maxPublicKeyLen Size of buffer (\p pPublicKeyData) provided to contain public key
*/
U16 HOSTCRYPTO_GetPublicKey(EC_KEY *pKey, U8 *pPublicKeyData, U16 *pPublicKeyLen, U16 maxPublicKeyLen)
{
    int res;
    size_t keylen = 0;
    mbedtls_ecp_keypair * pEcCtx;
    if ( pKey == NULL) {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    pEcCtx = mbedtls_pk_ec( *pKey );

    res = mbedtls_ecp_check_pubkey(  &(pEcCtx->grp), &(pEcCtx->Q) );
    if ((res != 0) || (pPublicKeyData == NULL) || (pPublicKeyLen == NULL))
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    res = mbedtls_ecp_point_write_binary(&(pEcCtx->grp),&(pEcCtx->Q),MBEDTLS_ECP_PF_UNCOMPRESSED,&keylen,pPublicKeyData,maxPublicKeyLen);
    *pPublicKeyLen = (U16)keylen;

    if ((*pPublicKeyLen == 0) || (res != 0))
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    return SW_OK;
}

/**
 * Extract the private key - as a byte array restored to nominal key size by sign extension - from an ECC key (in an mbed TLS specific format)
 * @param[in] pKey Reference to ECC key.
 * @param[in,out] pPrivateKeyData IN: Buffer to contain private key; OUT: Private key
 * @param[out] pPrivateKeyLen Length of private key \p pPrivateKeyData retrieved
 * @param[in] maxPrivateKeyLen Size of buffer (\p pPrivateKeyData) provided to contain private key
*/
U16 HOSTCRYPTO_GetPrivateKey(EC_KEY *pKey, U8 *pPrivateKeyData, U16 *pPrivateKeyLen, U16 maxPrivateKeyLen)
{

    int significantBytes = 0;
    int keyLen = 0;
    U16 res = SW_OK;
    U8 keyArray[256];
    const mbedtls_ecp_curve_info *p_curve_info = NULL;
    mbedtls_ecp_keypair * pEcCtx;
    if ( pKey == NULL) {
        return ERR_CRYPTO_ENGINE_FAILED;
    }
    pEcCtx = mbedtls_pk_ec( *pKey );

    /*TODO check sign extension part */
    res = mbedtls_ecp_check_privkey( &(pEcCtx->grp), &(pEcCtx->d));
    if ((res != 0) || (pPrivateKeyData == NULL) || (pPrivateKeyLen == NULL))
    {
        return ERR_CRYPTO_ENGINE_FAILED;
    }

    significantBytes = mbedtls_mpi_size( &(pEcCtx->d) );
    p_curve_info = mbedtls_ecp_curve_info_from_grp_id( pEcCtx->grp.id );
    keyLen = ((p_curve_info->bit_size + 7))/8;
    if (keyLen > maxPrivateKeyLen)
    {
        return ERR_BUF_TOO_SMALL;
    }
    res = mbedtls_mpi_write_binary(&(pEcCtx->d),keyArray,keyLen);
    if (res == 0)
    {
        // Extend byte array with leading 0x00 byte in case private key had
        // been truncated because the MSB were not significant
        if (significantBytes > 0)
        {
            res = axZeroSignExtend(keyArray, (U16)significantBytes, (U16)keyLen);
            if (res == SW_OK)
            {
                memcpy(pPrivateKeyData, keyArray, keyLen);
                *pPrivateKeyLen = (U16)keyLen;
            }
            else
            {
                *pPrivateKeyLen = 0;

            }
        }
        else
        {
            *pPrivateKeyLen = 0;
            res = ERR_GENERAL_ERROR;
        }
    }
    else
    {
        res = ERR_CRYPTO_ENGINE_FAILED;
    }


    return res;
}

void HOSTCRYPTO_FreeEccKey(EC_KEY** ppKey) {
    if ( ppKey == NULL )
    {
        ; /* Nothing to do */
    }
    else
    {
        if ( *ppKey != NULL)
        {
            mbedtls_pk_free(*ppKey);
            free(*ppKey);
        }
        *ppKey = NULL;
    }
}

/**
 * Create an ECC key (in an mbedTLS specific format) on the requested curve
 * @param[in] curveType E.g. ECCCurve_NIST_P256. Not all curves defined in ::ECCCurve_t are always supported
     by the underlying mbed TLS crypto library.
 * @param[out] ppKey    Double indirection to EC_KEY data structure. In case *ppKey already points
     to a key object, that object is freed first.
*/
U16 HOSTCRYPTO_GenerateEccKey(ECCCurve_t curveType, EC_KEY** ppKey)
{
    int32_t ret = 0;
#if !AX_EMBEDDED
  mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
#endif

    /*TODO check how to consume and destroy */
    mbedtls_pk_context * pKey;

    if ( ppKey != NULL && *ppKey != NULL )
    {
        HOSTCRYPTO_FreeEccKey(ppKey);
        *ppKey = NULL;
    }

    pKey = (mbedtls_pk_context *)malloc(sizeof(*pKey));
    mbedtls_pk_init( pKey );
#if !AX_EMBEDDED
    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );

    ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               NULL,
                               0 );

#endif
    if (ret == 0)
    {
        ret = mbedtls_pk_setup( pKey, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY) );
        if (ret == 0)
        {

#if !AX_EMBEDDED
            ret = mbedtls_ecp_gen_key( curveType2GroupID(curveType), mbedtls_pk_ec( *pKey ),
                          mbedtls_ctr_drbg_random, &ctr_drbg );
#else
            ret = mbedtls_ecp_gen_key( curveType2GroupID(curveType), mbedtls_pk_ec( *pKey ),
                          &HOSTCRYPTO_GetRandom, NULL );
#endif
            if (ret == 0)
            {
                *ppKey = pKey;
#if !AX_EMBEDDED
                mbedtls_ctr_drbg_free( &ctr_drbg );
                mbedtls_entropy_free( &entropy );

#endif
                return SW_OK;
            }
        }

    }

    mbedtls_pk_free( pKey );
    free(pKey);
    *ppKey = NULL;

#if !AX_EMBEDDED
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
#endif
    return ERR_CRYPTO_ENGINE_FAILED;
}


/**
 * Create an mbed TLS specific key structure and fill it up with the ECC key material contained
 * in the crypto library agnostic \p eccKc data structure. In case \p eccKc does not contain a
 * private key, only the public part will be copied into the mbed TLS specific key structure.
 *
 * @param[out] eccRef Double indirection to EC_KEY data structure.
 *   In case *ppKey already points to a key object, that object is freed first.
 * @param[in]  eccKc  Data structure containing ECC key material.
 */
U16 HOSTCRYPTO_EccCreateOpenSslEccFromComponents(EC_KEY **ppKey, eccKeyComponents_t *eccKc)
{
    int ret;
    mbedtls_pk_context * pKey;

    if ( ppKey != NULL && *ppKey != NULL )
    {
        HOSTCRYPTO_FreeEccKey(ppKey);
        *ppKey = NULL;
    }

    pKey = (mbedtls_pk_context *) malloc(sizeof(*pKey));
    mbedtls_pk_init( pKey );

    ret = mbedtls_pk_setup( pKey, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY) );
    if (ret == 0)
    {
        mbedtls_ecp_keypair * pEcCtx = mbedtls_pk_ec( *pKey );
        *ppKey = pKey; //mbedtls_pk_ec( *pKey );
        ret = mbedtls_ecp_group_load( &(pEcCtx->grp), curveType2GroupID(eccKc->curve) );
        if (ret == 0)
        {
            ret = mbedtls_ecp_point_read_binary( &(pEcCtx->grp), &(pEcCtx->Q),
                eccKc->pub, eccKc->pubLen );
            if (ret == 0)
            {
                ret = mbedtls_mpi_read_binary( &(pEcCtx->d), eccKc->priv, eccKc->privLen );
                if (ret == 0)
                {
                    return SW_OK;
                }
            }
        }
    }
    /* Error */
    free(pKey);

    return ERR_CRYPTO_ENGINE_FAILED;

}

/**
 * Key unwrapping according to RFC3394 (https://tools.ietf.org/html/rfc3394)
 * \note Only tested with an 128 bits wrapKey.
 * @param[in] wrapKey Secret key used to unwrap \p in with. Also called KEK (key encryption key)
 * @param[in] wrapKeyLen Length in byte of wrapKey
 * @param[in,out] out IN: buffer of at least \p outLen byte; OUT: unwrapped key
 * @param[in,out] outLen IN: size of buffer \p out provided; OUT: actual length of unwrapped key
 * @param[in] in Wrapped key to be unwrapped
 * @param[in] inLen Length in byte of key to be unwrapped
 */
U16 HOSTCRYPTO_AesUnwrapKeyRFC3394(const U8 *wrapKey, U16 wrapKeyLen, U8 *out, U16 *outLen, const U8 *in, U16 inLen)
{
    #ifdef __FUNCTION__
    sm_printf(CONSOLE, "Not Implemented => '" __FUNCTION__ "'.\n");
    #endif
    return 0;
}

/**
 * Key wrapping according to RFC3394 (https://tools.ietf.org/html/rfc3394)
 * \note Only tested with an 128 bits wrapKey. When wrapping a 128 bit key (16 byte), the resulting
 *  wrapped key is 24 byte long.
 * @param[in] wrapKey Secret key used to wrap \p in. Also called KEK (key encryption key)
 * @param[in] wrapKeyLen Length in byte of wrapKey
 * @param[in,out] out IN: buffer of at least \p outLen byte; OUT: wrapped key
 * @param[in,out] outLen IN: size of buffer \p out provided; OUT: actual length of wrapped key
 * @param[in] in Key to be wrapped
 * @param[in] inLen Length in byte of key to be wrapped
 */
U16 HOSTCRYPTO_AesWrapKeyRFC3394(const U8 *wrapKey, U16 wrapKeyLen, U8 *out, U16 *outLen, const U8 *in, U16 inLen)
{
    int retval = 0;
    //mbedtls_aes_context *ctx = NULL;

    //int keybits = wrapKeyLen * 8;

    /*TODO Borrowed check license */
    retval = fast_aes_wrap(wrapKey,((inLen * 8) >> 6),in,out);

    return retval;
}

/**
 * fast_aes_wrap - Wrap keys with AES Key Wrap Algorithm (128-bit KEK) (RFC3394)
 * @kek: 16-octet Key encryption key (KEK)
 * @n: Length of the plaintext key in 64-bit units; e.g., 2 = 128-bit = 16
 * bytes
 * @plain: Plaintext key to be wrapped, n * 64 bits
 * @cipher: Wrapped key, (n + 1) * 64 bits
 * Returns: 0 on success, -1 on failure
 */
static int fast_aes_wrap(const uint8_t *kek, int n, const uint8_t *plain, uint8_t *cipher)
{
    uint8_t *a, *r, b[16];
    int32_t i, j;
    int32_t ret = 0;
    mbedtls_aes_context ctx;

    a = cipher;
    r = cipher + 8;

    /* 1) Initialize variables. */
    memset(a, 0xa6, 8);
    memcpy(r, plain, 8 * n);

    mbedtls_aes_init(&ctx);
    ret = mbedtls_aes_setkey_enc(&ctx, kek, 128);
    /*TODO check return code */
    if (ret != 0) {
        mbedtls_aes_free(&ctx);
        return ret;
    }

    /* 2) Calculate intermediate values.
     * For j = 0 to 5
     *     For i=1 to n
     *         B = AES(K, A | R[i])
     *         A = MSB(64, B) ^ t where t = (n*j)+i
     *         R[i] = LSB(64, B)
     */
    for (j = 0; j <= 5; j++) {
    r = cipher + 8;
    for (i = 1; i <= n; i++) {
            memcpy(b, a, 8);
            memcpy(b + 8, r, 8);
            mbedtls_aes_encrypt(&ctx, b, b);
            memcpy(a, b, 8);
            a[7] ^= n * j + i;
            memcpy(r, b + 8, 8);
            r += 8;
    }
    }
    mbedtls_aes_free(&ctx);

    /* 3) Output the results.
     *
     * These are already in @cipher due to the location of temporary
     * variables.
     */

    return SW_OK;
}
#ifdef TGT_A71CH
U16 HOSTCRYPTO_HkdfExpandSha256(const U8 *secret, U16 secretLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen)
{
    U16 sw = SW_OK;
    int ret;
    const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    ret = mbedtls_hkdf_expand(md,
        secret, secretLen,
        info, infoLen,
        derivedData, derivedDataLen);

    if (ret != 0)
    {
        sw = ERR_CRYPTO_ENGINE_FAILED;
    }

    return sw;
}

U16 HOSTCRYPTO_HkdfFullSha256(const U8 *salt, U16 saltLen, const U8 *secret, U16 secretLen, const U8 *info, U16 infoLen, U8 *derivedData, U16 derivedDataLen)
{
    U16 sw = SW_OK;
    int ret;
    const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    ret = mbedtls_hkdf(md,
        salt, saltLen,
        secret, secretLen,
        info, infoLen,
        derivedData, derivedDataLen);


    if (ret != 0)
    {
        sw = ERR_CRYPTO_ENGINE_FAILED;
    }

    return sw;

}

// Implements the CreatePremasterSecret functionality of RFC4279
U16 HOSTCRYPTO_TlsPskCreatePremasterSecret(const U8 *secret, U16 secretLen, U8 *premasterSecret, U16 *premasterSecretLen)
{
    U16 targetLen = 0;

    // Ensure buffer premasterSecret is big enough
    targetLen = 2 * 2 + 2 * secretLen;
    if (*premasterSecretLen < targetLen)
    {
        return ERR_BUF_TOO_SMALL;
    }

    premasterSecret[0] = (U8)(secretLen >> 8);
    premasterSecret[1] = (U8)secretLen;
    memset(&premasterSecret[2], 0x00, secretLen);
    premasterSecret[2+secretLen] = (U8)(secretLen >> 8);
    premasterSecret[3+secretLen] = (U8)secretLen;
    memcpy(&premasterSecret[4+secretLen], secret, secretLen);

    *premasterSecretLen = targetLen;
    return SW_OK;
}

// Implements the CreatePremasterSecret functionality of RFC5489
U16 HOSTCRYPTO_TlsEcdhPskCreatePremasterSecret(const U8 *ecdhSS, U16 ecdhSSLen, const U8 *secret, U16 secretLen, U8 *premasterSecret, U16 *premasterSecretLen)
{
    U16 targetLen = 0;

    // Ensure buffer premasterSecret is big enough
    targetLen = 2 * 2 + ecdhSSLen + secretLen;
    if (*premasterSecretLen < targetLen)
    {
        return ERR_BUF_TOO_SMALL;
    }

    premasterSecret[0] = (U8)(ecdhSSLen >> 8);
    premasterSecret[1] = (U8)ecdhSSLen;
    memcpy(&premasterSecret[2], ecdhSS, ecdhSSLen);
    premasterSecret[2+ecdhSSLen] = (U8)(secretLen >> 8);
    premasterSecret[3+ecdhSSLen] = (U8)secretLen;
    memcpy(&premasterSecret[4+ecdhSSLen], secret, secretLen);

    *premasterSecretLen = targetLen;
    return SW_OK;
}

U16 HOSTCRYPTO_HmacSha256(const U8 *secret, U16 secretLen, const U8 *data, U16 dataLen, U8 *hmacData)
{
    int ret;
    U16 sw = SW_OK;
    const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    ret = mbedtls_md_hmac(md, secret, secretLen, data, dataLen, hmacData);
    if (ret != 0)
    {
        sw = ERR_CRYPTO_ENGINE_FAILED;
    }
    return sw;
}

U16 HOSTCRYPTO_Tls1_2_P_Sha256(const U8 *secret, U16 secretLen, const U8 *seed, U16 seedLen, U8 *derivedData, U16 derivedDataLen)
{

    unsigned int nPos = 0;
    unsigned char hashed0[MBEDTLS_MD_MAX_SIZE];
    unsigned char hashed1[MBEDTLS_MD_MAX_SIZE];
    const mbedtls_md_info_t *p_mbedtls_md_info = NULL;
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    U16 bytesToCopy;
    unsigned char digLen = 0;


    p_mbedtls_md_info = mbedtls_md_info_from_type(md_type);
    digLen = mbedtls_md_get_size( p_mbedtls_md_info );
    mbedtls_md_init(&ctx);
    if ((mbedtls_md_setup(&ctx, p_mbedtls_md_info , 1))) //use hmac
        goto err;



    // First compute A(1) = HMAC(secret, SEED)
    if ((mbedtls_md_hmac_starts(&ctx, secret, secretLen)))
        goto err;
    if ((mbedtls_md_hmac_update(&ctx, (const unsigned char *) seed, seedLen)))
        goto err;
    if ((mbedtls_md_hmac_finish(&ctx, hashed0)))
        goto err;

    while (derivedDataLen)
    {
        // Now compute P(N) = HMAC(secret, A(N) | seed) # N > 0
        if (mbedtls_md_hmac_reset(&ctx))
            goto err;
        if ((mbedtls_md_hmac_update(&ctx, (const unsigned char *) hashed0, digLen)))
            goto err;
        if ((mbedtls_md_hmac_update(&ctx, (const unsigned char *) seed, seedLen)))
            goto err;
        if ((mbedtls_md_hmac_finish(&ctx, hashed1 )))
            goto err;

        bytesToCopy = (derivedDataLen < digLen) ? derivedDataLen : (U16)digLen;

        memcpy(&derivedData[nPos], hashed1, bytesToCopy);
        derivedDataLen -= bytesToCopy;
        nPos += bytesToCopy;

        // Compute A(N+1) and store for next round
        if (mbedtls_md_hmac_reset(&ctx))
            goto err;
        if ((mbedtls_md_hmac_update(&ctx, (const unsigned char *) hashed0, digLen)))
            goto err;
        if ((mbedtls_md_hmac_finish(&ctx, hashed0 )))
            goto err;
    }

    mbedtls_md_free(&ctx);
    return SW_OK;

 err:
    mbedtls_md_free(&ctx);
    return ERR_CRYPTO_ENGINE_FAILED;
}

#endif // TGT_A71CH



#if AX_EMBEDDED
#include "mbedtls/entropy_poll.h"



int HOSTCRYPTO_GetRandom(void *pCtx, unsigned char *output, size_t len)
{
    status_t result = kStatus_Success;

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && (FSL_FEATURE_SOC_TRNG_COUNT > 0)
#ifndef TRNG0
#define TRNG0 TRNG
#endif
    result = TRNG_GetRandomData(TRNG0, output, len);
#elif defined(FSL_FEATURE_SOC_RNG_COUNT) && (FSL_FEATURE_SOC_RNG_COUNT > 0)
    result = RNGA_GetRandomData(RNG, (void *)output, len);
#elif defined(FSL_FEATURE_SOC_LPC_RNG_COUNT) && (FSL_FEATURE_SOC_LPC_RNG_COUNT > 0)
    uint32_t rn;
    size_t length;
    int i;

    length = len;

    while (length > 0)
    {
        rn = RNG_GetRandomData();

        if (length >= sizeof(uint32_t))
        {
            memcpy(output, &rn, sizeof(uint32_t));
            length -= sizeof(uint32_t);
            output += sizeof(uint32_t);
        }
        else
        {
            memcpy(output, &rn, length);
            output += length;
            len = 0U;
        }

        /* Discard next 32 random words for better entropy */
        for (i = 0; i < 32; i++)
        {
            RNG_GetRandomData();
        }
    }

    result = kStatus_Success;
#endif
    if (result == kStatus_Success)
    {
        return 0;
    }
    else
    {
        return result;
    }
}
#endif
