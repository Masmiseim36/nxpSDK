/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"
#include "mbedtls/error.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform_util.h"
#endif

#if defined(FSL_FEATURE_EDGELOCK) && (FSL_FEATURE_EDGELOCK > 0)
#include "sss_crypto.h"

/******************************************************************************/
/*************************** SHA512 *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_SSS_SHA512) && defined(MBEDTLS_SHA512_ALT) && defined(MBEDTLS_SHA512_C)

#include "mbedtls/sha512.h"

#define SHA512_VALIDATE_RET(cond) MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_SHA512_BAD_INPUT_DATA)
#define SHA512_VALIDATE(cond)     MBEDTLS_INTERNAL_VALIDATE(cond)

void mbedtls_sha512_init(mbedtls_sha512_context *ctx)
{
    SHA512_VALIDATE(ctx != NULL);

    (void)memset(ctx, 0, sizeof(mbedtls_sha512_context));
}

void mbedtls_sha512_free(mbedtls_sha512_context *ctx)
{
    if (ctx == NULL)
        return;

    if( ctx->sss_ctx_init != 0 )
    {
         (void)sss_sscp_digest_context_free(&ctx->ctx);
          ctx->sss_ctx_init = 0;
    }

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha512_context ));
}

void mbedtls_sha512_clone(mbedtls_sha512_context *dst, const mbedtls_sha512_context *src)
{
    SHA512_VALIDATE( dst != NULL );
    SHA512_VALIDATE( src != NULL );

    *dst = *src;
}

/*
 * SHA-512 context setup
 */
int mbedtls_sha512_starts(mbedtls_sha512_context *ctx, int is384)
{
    SHA512_VALIDATE_RET( ctx != NULL );
#if defined(MBEDTLS_SHA384_C)
    SHA512_VALIDATE_RET( is384 == 0 || is384 == 1 );
#else
    SHA512_VALIDATE_RET( is384 == 0 );
#endif

    int ret;
    sss_algorithm_t alg;
    if (is384 == 1)
    {
        alg = kAlgorithm_SSS_SHA384;
    }
    else
    {
        alg = kAlgorithm_SSS_SHA512;
    }

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    ret = 0;
    if (!ctx->sss_ctx_init) {
       if (sss_sscp_digest_context_init(&ctx->ctx, &g_sssSession, alg, kMode_SSS_Digest) != kStatus_SSS_Success)
       {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }  
    }
    
    ctx->sss_ctx_init = 1;
    if (sss_sscp_digest_init(&ctx->ctx) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    
     return ret;
}

int mbedtls_internal_sha512_process(mbedtls_sha512_context *ctx, const unsigned char data[128])
{
    int ret;
    
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( (const unsigned char *)data != NULL );
    
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_digest_update(&ctx->ctx, (uint8_t *)(uintptr_t)data, 64) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*
 * SHA-512 process buffer
 */
int mbedtls_sha512_update(mbedtls_sha512_context *ctx, const unsigned char *input, size_t ilen)
{
    int ret;
    
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( ilen == 0 || input != NULL );
    
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_digest_update(&ctx->ctx, (uint8_t *)(uintptr_t)input, ilen) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*
 * SHA-512 final digest
 */
int mbedtls_sha512_finish(mbedtls_sha512_context *ctx, unsigned char *output)
{
    int ret;
    size_t len = ctx->ctx.digestFullLen;
    
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( (unsigned char *)output != NULL );

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_digest_finish(&ctx->ctx, output, &len) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    (void)sss_sscp_digest_context_free(&ctx->ctx);
    ctx->sss_ctx_init = 0;
    return ret;
}

#endif /* MBEDTLS_MCUX_SSS_SHA512 && MBEDTLS_SHA1_ALT && MBEDTLS_SHA512_C */


/******************************************************************************/
/*************************** SHA256********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_SSS_SHA256) && defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C)
#include "mbedtls/sha256.h"

#define SHA256_VALIDATE_RET(cond) MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_SHA256_BAD_INPUT_DATA)
#define SHA256_VALIDATE(cond)     MBEDTLS_INTERNAL_VALIDATE(cond)

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    SHA256_VALIDATE(ctx != NULL);
    
    memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if( ctx == NULL )
        return;

    if( ctx->sss_ctx_init != 0 )
    {
         (void)sss_sscp_digest_context_free(&ctx->ctx);
          ctx->sss_ctx_init = 0;
    }
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst, const mbedtls_sha256_context *src)
{
    SHA256_VALIDATE( dst != NULL );
    SHA256_VALIDATE( src != NULL );

    *dst = *src;
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{  
    int ret;
    sss_algorithm_t alg;
    
    SHA256_VALIDATE_RET( ctx != NULL );

#if defined(MBEDTLS_SHA224_C)
    SHA256_VALIDATE_RET( is224 == 0 || is224 == 1 );
#else
    SHA256_VALIDATE_RET( is224 == 0 );
#endif

    if (is224 == 1)
    {
        alg = kAlgorithm_SSS_SHA224;
    }
    else
    {
        alg = kAlgorithm_SSS_SHA256;
    }
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    
    ret = 0;
    if (!ctx->sss_ctx_init) {
       if (sss_sscp_digest_context_init(&ctx->ctx, &g_sssSession, alg, kMode_SSS_Digest) != kStatus_SSS_Success)
       {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            return ret;
        }  
    }
    
    ctx->sss_ctx_init = 1;
    if (sss_sscp_digest_init(&ctx->ctx) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }  
     return ret;
}

/* TBD - mbedtls is doing mathematics here..need to check why. Currently this is same as sha256_update */
int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    int ret;
    
    SHA256_VALIDATE_RET( ctx != NULL );
    SHA256_VALIDATE_RET( (const unsigned char *)data != NULL );

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_digest_update(&ctx->ctx, (uint8_t *)(uintptr_t)data, 64) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    int ret;
    
    SHA256_VALIDATE_RET( ctx != NULL );
    SHA256_VALIDATE_RET( ilen == 0 || input != NULL );
    
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_digest_update(&ctx->ctx, (uint8_t *)(uintptr_t)input, ilen) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char *output)
{
    int ret;
    size_t len = ctx->ctx.digestFullLen;
    
    SHA256_VALIDATE_RET( ctx != NULL );
    SHA256_VALIDATE_RET( (unsigned char *)output != NULL );
    
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_digest_finish(&ctx->ctx, output, &len) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    (void)sss_sscp_digest_context_free(&ctx->ctx);
    ctx->sss_ctx_init = 0;
    return ret;
}

#endif /* MBEDTLS_MCUX_SSS_SHA256 && MBEDTLS_SHA256_ALT && MBEDTLS_SHA256_C*/

/******************************************************************************/
/*************************** ECDSA ********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_SSS_ECDSA) && defined(MBEDTLS_ECDSA_C)
#include "mbedtls/platform.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"

/* Parameter validation macros based on platform_util.h */
#define ECDSA_VALIDATE_RET(cond) MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA)
#define ECDSA_VALIDATE(cond)     MBEDTLS_INTERNAL_VALIDATE(cond)

#define ECDSA_RS_ECP NULL

#define ECDSA_BUDGET(ops) /* no-op; for compatibility */

#define ECDSA_RS_ENTER(SUB) (void)rs_ctx
#define ECDSA_RS_LEAVE(SUB) (void)rs_ctx

/* Used as values s and n of mbedtls_mpi object to indicate that P contain pointer to key object. */
#define MBEDTLS_ECDSA_MPI_S_HAVE_OBJECT (155)
#define MBEDTLS_ECDSA_MPI_N_HAVE_OBJECT (1u)

#if defined(MBEDTLS_ECDSA_SIGN_ALT) || defined(MBEDTLS_ECDSA_VERIFY_ALT)
static int mbedtls_ecdsa_verify_digest_len(const size_t blen, sss_algorithm_t *alg)
{
    int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    switch (blen)
    {
        case 32:
            *alg = kAlgorithm_SSS_ECDSA_SHA256;
            ret  = 0;
            break;
        case 64:
            *alg = kAlgorithm_SSS_ECDSA_SHA512;
            ret  = 0;
            break;
        default:
            ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
            break;
    }
    return ret;
}

static int mbedtls_ecdsa_verify_digest_align(const size_t digestLen,
                                              const size_t ecCoordinateSize,
                                              const uint8_t *digest,
                                              uint8_t alignedDigest[])
{
    if (digestLen < ecCoordinateSize)
    {
        size_t diff = ecCoordinateSize - digestLen;
        (void)memcpy(&alignedDigest[diff], digest, digestLen);
    }
    else
    {
        (void)memcpy(alignedDigest, digest, digestLen);
    }
    return 0;
}
#endif /* defined(MBEDTLS_ECDSA_SIGN_ALT) || defined(MBEDTLS_ECDSA_VERIFY_ALT) */

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
/*
 * Compute ECDSA signature of a hashed message (SEC1 4.1.3)
 * Obviously, compared to SEC1 4.1.3, we skip step 4 (hash message)
 */
static int ecdsa_sign_restartable(mbedtls_ecp_group *grp,
                                  mbedtls_mpi *r,
                                  mbedtls_mpi *s,
                                  const mbedtls_mpi *d,
                                  const unsigned char *buf,
                                  size_t blen,
                                  int (*f_rng)(void *, unsigned char *, size_t),
                                  void *p_rng,
                                  mbedtls_ecdsa_restart_ctx *rs_ctx)
{
    int ret = 0;
    sss_sscp_asymmetric_t asyc;
    sss_algorithm_t alg;
    size_t bufLen          = (blen + 7u) / 8u;
    size_t coordinateLen   = (grp->pbits + 7u) / 8u;
    size_t signatureSize   = 2u * coordinateLen;
    uint8_t *signature     = mbedtls_calloc(signatureSize, sizeof(uint8_t));
    uint8_t *alignedDigest = mbedtls_calloc(coordinateLen, sizeof(uint8_t));

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (grp->N.MBEDTLS_PRIVATE(p) == NULL)
    {
        mbedtls_free(signature);
        mbedtls_free(alignedDigest);
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    /* Check if mbedtls_mpi was initialized with key object. */
    else if (d->MBEDTLS_PRIVATE(s) != MBEDTLS_ECDSA_MPI_S_HAVE_OBJECT)
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    /* Check if mbedtls_mpi was initialized with key object. */
    else if (d->MBEDTLS_PRIVATE(n) != MBEDTLS_ECDSA_MPI_N_HAVE_OBJECT)
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
    else if ((ret = mbedtls_ecdsa_verify_digest_len(blen, &alg)) != 0)
    {
    }
    else if (mbedtls_ecdsa_verify_digest_align(blen, coordinateLen, (const uint8_t *)buf, alignedDigest) != 0)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_asymmetric_context_init(&asyc, &g_sssSession, (sss_sscp_object_t *)(uintptr_t)d->MBEDTLS_PRIVATE(p), alg,
                                              kMode_SSS_Sign) != kStatus_SSS_Success)
    {
    }
    else if (sss_sscp_asymmetric_sign_digest(&asyc, alignedDigest, coordinateLen, signature, &signatureSize) !=
             kStatus_SSS_Success)
    {
    }
    else if ((ret = mbedtls_mpi_read_binary(r, signature, coordinateLen)) != 0)
    {
    }
    else if ((ret = mbedtls_mpi_read_binary(s, &signature[coordinateLen], coordinateLen)) != 0)
    {
    }
    else
    {
        ret = 0;
    }
    (void)sss_sscp_asymmetric_context_free(&asyc);
    mbedtls_free(alignedDigest);
    mbedtls_free(signature);
    return (ret);
}

#if defined(MBEDTLS_ECDSA_ALT)
/*
 * Initialize context
 */
void mbedtls_ecdsa_init(mbedtls_ecdsa_context *ctx)
{
    ECDSA_VALIDATE(ctx != NULL);
    ctx->isKeyInitialized = false;
    /*
     * Ensure the 3 elements in beginnign of ctx structure are
     * same as that in mbedtls_ecp_keypair.
     */
    mbedtls_ecp_keypair_init((mbedtls_ecp_keypair *)ctx);
}

/*
 * Free context
 */
void mbedtls_ecdsa_free(mbedtls_ecdsa_context *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->isKeyInitialized)
    {
        (void)SSS_KEY_OBJ_FREE(&ctx->key);
    }
    mbedtls_ecp_keypair_free((mbedtls_ecp_keypair *)ctx);
}
#endif /* MBEDTLS_ECDSA_ALT */

#if defined(MBEDTLS_ECDSA_GENKEY_ALT)
/*
 * Generate key pair
 */
int mbedtls_ecdsa_genkey(mbedtls_ecdsa_context *ctx,
                         mbedtls_ecp_group_id gid,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng)
{
    int ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    ECDSA_VALIDATE_RET(ctx != NULL);
    ret = mbedtls_ecp_group_load(&ctx->grp, gid);
    if (ret != 0)
    {
        return (ret);
    }
    size_t keyLen     = (ctx->grp.pbits + 7u) / 8u;
    size_t keyBitsLen = ctx->grp.pbits;

    size_t keySize    = SSS_ECP_KEY_SZ(keyLen); /* just 2 * key for A1 public key but 3 times for A0 */
    uint8_t *pubKey   = mbedtls_calloc(keySize, sizeof(uint8_t));
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        mbedtls_platform_zeroize(pubKey, keySize);
        mbedtls_free(pubKey);
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    if (ctx->isKeyInitialized == false)
    {
        if (sss_sscp_key_object_init(&ctx->key, &g_keyStore) != kStatus_SSS_Success)
        {
            mbedtls_platform_zeroize(pubKey, keySize);
            mbedtls_free(pubKey);
            (void)SSS_KEY_OBJ_FREE(&ctx->key);
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        /* Allocate key handle */
        else if (sss_sscp_key_object_allocate_handle(&ctx->key,
                                                     0x0u,
                                                     kSSS_KeyPart_Pair,
                                                     kSSS_CipherType_EC_NIST_P,
                                                     3 * keyLen,
                                                     SSS_KEYPROP_OPERATION_ASYM) != kStatus_SSS_Success)
        {
            mbedtls_platform_zeroize(pubKey, keySize);
            mbedtls_free(pubKey);
            (void)SSS_KEY_OBJ_FREE(&ctx->key);
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
        else
        {
            ctx->isKeyInitialized = true;
        }
    }
    if (SSS_ECP_GENERATE_KEY(&ctx->key, keyBitsLen) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (SSS_KEY_STORE_GET_PUBKEY(&ctx->key,
                                      pubKey,
                                      &keySize,
                                      &keyBitsLen) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((ret = mbedtls_mpi_read_binary(&ctx->Q.MBEDTLS_PRIVATE(X), pubKey, keyLen)) != 0)
    {
    }
    else if ((ret = mbedtls_mpi_read_binary(&ctx->Q.MBEDTLS_PRIVATE(Y), &pubKey[keyLen], keyLen)) != 0)
    {
    }
    else if ((ret = mbedtls_mpi_lset(&ctx->Q.MBEDTLS_PRIVATE(Z), 1)) != 0)
    {
    }
    else
    {
        ctx->d.MBEDTLS_PRIVATE(s) = MBEDTLS_ECDSA_MPI_S_HAVE_OBJECT;
        ctx->d.MBEDTLS_PRIVATE(n) = MBEDTLS_ECDSA_MPI_N_HAVE_OBJECT;
        ctx->d.MBEDTLS_PRIVATE(p) = (mbedtls_mpi_uint *)(uintptr_t)&ctx->key;
        ret      = 0;
    }
    mbedtls_platform_zeroize(pubKey, keySize);
    mbedtls_free(pubKey);
    return ret;
}
#endif /* MBEDTLS_ECDSA_GENKEY_ALT */

int mbedtls_ecdsa_can_do(mbedtls_ecp_group_id gid)
{
    switch (gid)
    {
#ifdef MBEDTLS_ECP_DP_CURVE25519_ENABLED
        case MBEDTLS_ECP_DP_CURVE25519:
            return 0;
#endif
#ifdef MBEDTLS_ECP_DP_CURVE448_ENABLED
        case MBEDTLS_ECP_DP_CURVE448:
            return 0;
#endif
        default:
            return 1;
    }
}

/*
 * Compute ECDSA signature of a hashed message
 */
int mbedtls_ecdsa_sign(mbedtls_ecp_group *grp,
                       mbedtls_mpi *r,
                       mbedtls_mpi *s,
                       const mbedtls_mpi *d,
                       const unsigned char *buf,
                       size_t blen,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng)
{
    ECDSA_VALIDATE_RET(grp != NULL);
    ECDSA_VALIDATE_RET(r != NULL);
    ECDSA_VALIDATE_RET(s != NULL);
    ECDSA_VALIDATE_RET(d != NULL);
    ECDSA_VALIDATE_RET(f_rng != NULL);
    ECDSA_VALIDATE_RET(buf != NULL || blen == 0);

    return (ecdsa_sign_restartable(grp, r, s, d, buf, blen, f_rng, p_rng, NULL));
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT */

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message (SEC1 4.1.4)
 * Obviously, compared to SEC1 4.1.3, we skip step 2 (hash message)
 */
static int ecdsa_verify_restartable(mbedtls_ecp_group *grp,
                                    const unsigned char *buf,
                                    size_t blen,
                                    const mbedtls_ecp_point *Q,
                                    const mbedtls_mpi *r,
                                    const mbedtls_mpi *s,
                                    mbedtls_ecdsa_restart_ctx *rs_ctx)
{
    int ret;
    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (grp->N.MBEDTLS_PRIVATE(p) == NULL)
    {
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }
    size_t coordinateLen     = (grp->pbits + 7u) / 8u;
    size_t coordinateBitsLen = grp->pbits;
    size_t keySize           = SSS_ECP_KEY_SZ(coordinateLen);
    size_t bufLen            = (blen + 7u) / 8u;
    uint8_t *pubKey          = mbedtls_calloc(keySize, sizeof(uint8_t));
    sss_sscp_object_t ecdsaPublic;
    sss_sscp_asymmetric_t asyc;
    sss_algorithm_t alg;
    uint8_t *alignedDigest = mbedtls_calloc(coordinateLen, sizeof(uint8_t));
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((ret = mbedtls_ecdsa_verify_digest_len(blen, &alg)) != 0)
    {
    }
    else if (mbedtls_ecdsa_verify_digest_align(blen, coordinateLen, (const uint8_t *)buf, alignedDigest) != 0)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((ret = mbedtls_mpi_write_binary(&Q->MBEDTLS_PRIVATE(X), pubKey, coordinateLen)) != 0)
    {
    }
    else if ((ret = mbedtls_mpi_write_binary(&Q->MBEDTLS_PRIVATE(Y), &pubKey[coordinateLen], coordinateLen)) != 0)
    {
    }
    else if (sss_sscp_key_object_init(&ecdsaPublic, &g_keyStore) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    /* Allocate key handle */
      else if (sss_sscp_key_object_allocate_handle(&ecdsaPublic,
                                                 0u,
                                                 kSSS_KeyPart_Public,
                                                 kSSS_CipherType_EC_NIST_P,
                                                 keySize,
                                                 SSS_KEYPROP_OPERATION_ASYM) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (SSS_KEY_STORE_SET_KEY(&ecdsaPublic, (const uint8_t *)pubKey, keySize, coordinateBitsLen,
                                        (uint32_t)kSSS_KeyPart_Public) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((ret = mbedtls_mpi_write_binary(r, pubKey, coordinateLen)) != 0)
    {
    }
    else if ((ret = mbedtls_mpi_write_binary(s, &pubKey[coordinateLen], coordinateLen)) != 0)
    {
    }
    else if (sss_sscp_asymmetric_context_init(&asyc, &g_sssSession, &ecdsaPublic, alg, kMode_SSS_Verify) !=
             kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_asymmetric_verify_digest(&asyc, alignedDigest, coordinateLen, pubKey, coordinateLen * 2u) !=
             kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    (void)SSS_KEY_OBJ_FREE(&ecdsaPublic);
    (void)sss_sscp_asymmetric_context_free(&asyc);
    mbedtls_platform_zeroize(pubKey, keySize);
    mbedtls_free(pubKey);
    mbedtls_platform_zeroize(alignedDigest, coordinateLen);
    mbedtls_free(alignedDigest);
    return ret;
}

/*
 * Verify ECDSA signature of hashed message
 */
int mbedtls_ecdsa_verify(mbedtls_ecp_group *grp,
                         const unsigned char *buf,
                         size_t blen,
                         const mbedtls_ecp_point *Q,
                         const mbedtls_mpi *r,
                         const mbedtls_mpi *s)
{
    ECDSA_VALIDATE_RET(grp != NULL);
    ECDSA_VALIDATE_RET(Q != NULL);
    ECDSA_VALIDATE_RET(r != NULL);
    ECDSA_VALIDATE_RET(s != NULL);
    ECDSA_VALIDATE_RET(buf != NULL || blen == 0);

    return (ecdsa_verify_restartable(grp, buf, blen, Q, r, s, NULL));
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#endif /* defined(MBEDTLS_MCUX_SSS_ECDSA) && defined(MBEDTLS_ECDSA_C) */

/******************************************************************************/
/*************************** ECDH *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_SSS_ECDH) && defined(MBEDTLS_ECDH_ALT) && defined(MBEDTLS_ECDH_C)
#include "mbedtls/platform.h"
#include "mbedtls/ecdh.h"

/* Parameter validation macros based on platform_util.h */
#define ECDH_VALIDATE_RET(cond) MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA)
#define ECDH_VALIDATE(cond)     MBEDTLS_INTERNAL_VALIDATE(cond)

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
typedef mbedtls_ecdh_context mbedtls_ecdh_context_mbed;
#endif

static mbedtls_ecp_group_id mbedtls_ecdh_grp_id(const mbedtls_ecdh_context *ctx)
{
#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ctx->MBEDTLS_PRIVATE(grp).id);
#else
    return (ctx->MBEDTLS_PRIVATE(grp_id));
#endif
}

int mbedtls_ecdh_can_do(mbedtls_ecp_group_id gid)
{
    /* At this time, all groups support ECDH. */
    (void)gid;
    return (1);
}

#if !defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
/*
 * Generate public key (restartable version)
 *
 * Note: this internal function relies on its caller preserving the value of
 * the output parameter 'd' across continuation calls. This would not be
 * acceptable for a public function but is OK here as we control call sites.
 */
static int ecdh_gen_public_restartable(mbedtls_ecp_group *grp,
                                       mbedtls_mpi *d,
                                       mbedtls_ecp_point *Q,
                                       int (*f_rng)(void *, unsigned char *, size_t),
                                       void *p_rng,
                                       mbedtls_ecp_restart_ctx *rs_ctx)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    /* If multiplication is in progress, we already generated a privkey */
#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx == NULL || rs_ctx->rsm == NULL )
#endif
        MBEDTLS_MPI_CHK( mbedtls_ecp_gen_privkey( grp, d, f_rng, p_rng ) );

    MBEDTLS_MPI_CHK( mbedtls_ecp_mul_restartable( grp, Q, d, &grp->G,
                                                  f_rng, p_rng, rs_ctx ) );

cleanup:
    return (ret);
}

/*
 * Generate public key
 */
int mbedtls_ecdh_gen_public(mbedtls_ecp_group *grp,
                            mbedtls_mpi *d,
                            mbedtls_ecp_point *Q,
                            int (*f_rng)(void *, unsigned char *, size_t),
                            void *p_rng)
{
    ECDH_VALIDATE_RET(grp != NULL);
    ECDH_VALIDATE_RET(d != NULL);
    ECDH_VALIDATE_RET(Q != NULL);
    ECDH_VALIDATE_RET(f_rng != NULL);
    return (ecdh_gen_public_restartable(grp, d, Q, f_rng, p_rng, NULL));
}
#endif /* !MBEDTLS_ECDH_GEN_PUBLIC_ALT */

#if !defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
/*
 * Compute shared secret (SEC1 3.3.1)
 */
static int ecdh_compute_shared_restartable(mbedtls_ecp_group *grp,
                                           mbedtls_mpi *z,
                                           const mbedtls_ecp_point *Q,
                                           const mbedtls_mpi *d,
                                           int (*f_rng)(void *, unsigned char *, size_t),
                                           void *p_rng,
                                           mbedtls_ecp_restart_ctx *rs_ctx)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_ecp_point P;

    mbedtls_ecp_point_init(&P);

    MBEDTLS_MPI_CHK(mbedtls_ecp_mul_restartable(grp, &P, d, Q, f_rng, p_rng, rs_ctx));

    if (mbedtls_ecp_is_zero(&P))
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    MBEDTLS_MPI_CHK(mbedtls_mpi_copy(z, &P.MBEDTLS_PRIVATE(X)));

cleanup:
    mbedtls_ecp_point_free(&P);

    return (ret);
}

/*
 * Compute shared secret (SEC1 3.3.1)
 */
int mbedtls_ecdh_compute_shared(mbedtls_ecp_group *grp,
                                mbedtls_mpi *z,
                                const mbedtls_ecp_point *Q,
                                const mbedtls_mpi *d,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng)
{
    ECDH_VALIDATE_RET(grp != NULL);
    ECDH_VALIDATE_RET(Q != NULL);
    ECDH_VALIDATE_RET(d != NULL);
    ECDH_VALIDATE_RET(z != NULL);
    return (ecdh_compute_shared_restartable(grp, z, Q, d, f_rng, p_rng, NULL));
}
#endif /* !MBEDTLS_ECDH_COMPUTE_SHARED_ALT */

static void ecdh_init_internal(mbedtls_ecdh_context_mbed *ctx)
{
    mbedtls_ecp_group_init( &ctx->MBEDTLS_PRIVATE(grp) );
    mbedtls_mpi_init( &ctx->MBEDTLS_PRIVATE(d)  );
    mbedtls_ecp_point_init( &ctx->MBEDTLS_PRIVATE(Q)  );
    mbedtls_ecp_point_init( &ctx->MBEDTLS_PRIVATE(Qp)  );
    mbedtls_mpi_init( &ctx->MBEDTLS_PRIVATE(z)  );
	
#if defined(MBEDTLS_ECP_RESTARTABLE)
    mbedtls_ecp_restart_init( &ctx->rs );
#endif
}

/*
 * Initialize context
 */
void mbedtls_ecdh_init(mbedtls_ecdh_context *ctx)
{
    ECDH_VALIDATE(ctx != NULL);

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    ecdh_init_internal(ctx);
    mbedtls_ecp_point_init(&ctx->MBEDTLS_PRIVATE(Vi));
    mbedtls_ecp_point_init(&ctx->MBEDTLS_PRIVATE(Vf));
    mbedtls_mpi_init(&ctx->MBEDTLS_PRIVATE(_d));
#else
    memset(ctx, 0, sizeof(mbedtls_ecdh_context));

    ctx->MBEDTLS_PRIVATE(var) = MBEDTLS_ECDH_VARIANT_NONE;
#endif
    ctx->MBEDTLS_PRIVATE(point_format)     = MBEDTLS_ECP_PF_UNCOMPRESSED;

#if defined(MBEDTLS_ECP_RESTARTABLE)
    ctx->MBEDTLS_PRIVATE(restart_enabled) = 0;
#endif

    ctx->isKeyInitialized = false;
}

static int ecdh_setup_internal(mbedtls_ecdh_context_mbed *ctx, mbedtls_ecp_group_id grp_id)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    ret = mbedtls_ecp_group_load(&ctx->MBEDTLS_PRIVATE(grp), grp_id);
    if (ret != 0)
    {
        return (MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE);
    }

    return (0);
}

/*
 * Setup context
 */
int mbedtls_ecdh_setup(mbedtls_ecdh_context *ctx, mbedtls_ecp_group_id grp_id)
{
    ECDH_VALIDATE_RET(ctx != NULL);

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_setup_internal(ctx, grp_id));
#else
    switch (grp_id)
    {
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        case MBEDTLS_ECP_DP_CURVE25519:
            ctx->MBEDTLS_PRIVATE(point_format) = MBEDTLS_ECP_PF_COMPRESSED;
            ctx->MBEDTLS_PRIVATE(var) = MBEDTLS_ECDH_VARIANT_EVEREST;
            ctx->MBEDTLS_PRIVATE(grp_id) = grp_id;
            return( mbedtls_everest_setup( &ctx->ctx.everest_ecdh, grp_id ) );
#endif
        default:
            ctx->MBEDTLS_PRIVATE(point_format) = MBEDTLS_ECP_PF_UNCOMPRESSED;
            ctx->MBEDTLS_PRIVATE(var)         = MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0;
            ctx->MBEDTLS_PRIVATE(grp_id)       = grp_id;
            ecdh_init_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh));
            return (ecdh_setup_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), grp_id));
    }
#endif
}

static void ecdh_free_internal(mbedtls_ecdh_context_mbed *ctx)
{
    mbedtls_ecp_group_free( &ctx->MBEDTLS_PRIVATE(grp) );
    mbedtls_mpi_free( &ctx->MBEDTLS_PRIVATE(d)  );
    mbedtls_ecp_point_free( &ctx->MBEDTLS_PRIVATE(Q)   );
    mbedtls_ecp_point_free( &ctx->MBEDTLS_PRIVATE(Qp)  );
    mbedtls_mpi_free( &ctx->MBEDTLS_PRIVATE(z)  );
	
#if defined(MBEDTLS_ECP_RESTARTABLE)
    mbedtls_ecp_restart_free( &ctx->MBEDTLS_PRIVATE(rs) );
#endif
}

#if defined(MBEDTLS_ECP_RESTARTABLE)
/*
 * Enable restartable operations for context
 */
void mbedtls_ecdh_enable_restart( mbedtls_ecdh_context *ctx )
{
    ECDH_VALIDATE( ctx != NULL );

    ctx->MBEDTLS_PRIVATE(restart_enabled) = 1;
}
#endif

/*
 * Free context
 */
void mbedtls_ecdh_free(mbedtls_ecdh_context *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->isKeyInitialized)
    {
        (void)SSS_KEY_OBJ_FREE(&ctx->key);
    }

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    mbedtls_ecp_point_free(&ctx->MBEDTLS_PRIVATE(Vi));
    mbedtls_ecp_point_free(&ctx->MBEDTLS_PRIVATE(Vf));
    mbedtls_mpi_free(&ctx->MBEDTLS_PRIVATE(_d));
    ecdh_free_internal(ctx);
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        case MBEDTLS_ECDH_VARIANT_EVEREST:
            mbedtls_everest_free( &ctx->ctx.everest_ecdh );
            break;
#endif
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            ecdh_free_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh));
            break;
        default:
            /* All the cases have been listed above, the default clause should not be reached. */
            break;
    }

    ctx->MBEDTLS_PRIVATE(point_format) = MBEDTLS_ECP_PF_UNCOMPRESSED;
    ctx->MBEDTLS_PRIVATE(var)          = MBEDTLS_ECDH_VARIANT_NONE;
    ctx->MBEDTLS_PRIVATE(grp_id)       = MBEDTLS_ECP_DP_NONE;
#endif
}

static int ecdh_make_params_internal(mbedtls_ecdh_context_mbed *ctx,
                                     size_t *olen,
                                     int point_format,
                                     unsigned char *buf,
                                     size_t blen,
                                     int (*f_rng)(void *, unsigned char *, size_t),
                                     void *p_rng,
                                     int restart_enabled)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t grp_len, pt_len;
#if defined(MBEDTLS_ECP_RESTARTABLE)
    mbedtls_ecp_restart_ctx *rs_ctx = NULL;
#endif

    if (ctx->MBEDTLS_PRIVATE(grp).pbits == 0u)
    {
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( restart_enabled )
        rs_ctx = &ctx->MBEDTLS_PRIVATE(rs);
#else
    (void) restart_enabled;
#endif

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( ( ret = ecdh_gen_public_restartable( &ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(d), &ctx->MBEDTLS_PRIVATE(Q),
                                             f_rng, p_rng, rs_ctx ) ) != 0 )
        return( ret );
#else
    if( ( ret = mbedtls_ecdh_gen_public( &ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(d), &ctx->MBEDTLS_PRIVATE(Q),
                                         f_rng, p_rng ) ) != 0 )
        return( ret );
#endif /* MBEDTLS_ECP_RESTARTABLE */

    if ((ret = mbedtls_ecp_tls_write_group(&ctx->MBEDTLS_PRIVATE(grp), &grp_len, buf, blen)) != 0)
    {
        return (ret);
    }

    buf += grp_len;
    blen -= grp_len;

    if ((ret = mbedtls_ecp_tls_write_point(&ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(Q), point_format, &pt_len, buf, blen)) != 0)
    {
        return (ret);
    }

    *olen = grp_len + pt_len;
    return( 0 );
}

/*
 * Setup and write the ServerKeyExchange parameters (RFC 4492)
 *      struct {
 *          ECParameters    curve_params;
 *          ECPoint         public;
 *      } ServerECDHParams;
 */
int mbedtls_ecdh_make_params( mbedtls_ecdh_context *ctx,
                              size_t *olen,
                              unsigned char *buf,
                              size_t blen,
                              int (*f_rng)(void *, unsigned char *, size_t),
                              void *p_rng )
{
    int restart_enabled = 0;
    ECDH_VALIDATE_RET( ctx != NULL );
    ECDH_VALIDATE_RET( olen != NULL );
    ECDH_VALIDATE_RET( buf != NULL );
    ECDH_VALIDATE_RET( f_rng != NULL );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    restart_enabled = ctx->restart_enabled;
#else
    (void) restart_enabled;
#endif

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_make_params_internal(ctx, olen, ctx->MBEDTLS_PRIVATE(point_format), buf, blen, f_rng, p_rng, restart_enabled));
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        case MBEDTLS_ECDH_VARIANT_EVEREST:
            return( mbedtls_everest_make_params( &ctx->ctx.everest_ecdh, olen,
                                                 buf, blen, f_rng, p_rng ) );
#endif
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            return (ecdh_make_params_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), olen, ctx->MBEDTLS_PRIVATE(point_format), buf, blen, f_rng, p_rng,
                                              restart_enabled));
        default:
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
#endif
}

static int ecdh_read_params_internal( mbedtls_ecdh_context_mbed *ctx,
                                      const unsigned char **buf,
                                      const unsigned char *end )
{
    return( mbedtls_ecp_tls_read_point( &ctx->MBEDTLS_PRIVATE(grp),
                                        &ctx->MBEDTLS_PRIVATE(Qp),
                                        buf,
                                        end - *buf ) );
}

/*
 * Read the ServerKeyExhange parameters (RFC 4492)
 *      struct {
 *          ECParameters    curve_params;
 *          ECPoint         public;
 *      } ServerECDHParams;
 */
int mbedtls_ecdh_read_params( mbedtls_ecdh_context *ctx,
                              const unsigned char **buf,
                              const unsigned char *end )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_ecp_group_id grp_id;
    ECDH_VALIDATE_RET( ctx != NULL );
    ECDH_VALIDATE_RET( buf != NULL );
    ECDH_VALIDATE_RET( *buf != NULL );
    ECDH_VALIDATE_RET( end != NULL );

    if ((ret = mbedtls_ecp_tls_read_group_id(&grp_id, buf, end - *buf)) != 0)
    {
        return (ret);
    }

    if ((ret = mbedtls_ecdh_setup(ctx, grp_id)) != 0)
    {
        return (ret);
    }

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_read_params_internal(ctx, buf, end));
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        case MBEDTLS_ECDH_VARIANT_EVEREST:
            return( mbedtls_everest_read_params( &ctx->ctx.everest_ecdh,
                                                 buf, end) );
#endif
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            return( ecdh_read_params_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), buf, end));
        default:
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
#endif
}

static int ecdh_get_params_internal(mbedtls_ecdh_context_mbed *ctx,
                                    const mbedtls_ecp_keypair *key,
                                    mbedtls_ecdh_side side)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    /* If it's not our key, just import the public part as Qp */
    if(side == MBEDTLS_ECDH_THEIRS)
    {
        return (mbedtls_ecp_copy(&ctx->MBEDTLS_PRIVATE(Qp), &key->MBEDTLS_PRIVATE(Q)));
    }

    /* Our key: import public (as Q) and private parts */
    if (side != MBEDTLS_ECDH_OURS)
    {
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

    if (((ret = mbedtls_ecp_copy(&ctx->MBEDTLS_PRIVATE(Q), &key->MBEDTLS_PRIVATE(Q))) != 0) ||
        ((ret = mbedtls_mpi_copy(&ctx->MBEDTLS_PRIVATE(d), &key->MBEDTLS_PRIVATE(d))) != 0) )
    {
        return (ret);
    }

    return (0);
}

/*
 * Get parameters from a keypair
 */
int mbedtls_ecdh_get_params(mbedtls_ecdh_context *ctx, const mbedtls_ecp_keypair *key, mbedtls_ecdh_side side)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECDH_VALIDATE_RET( ctx != NULL );
    ECDH_VALIDATE_RET( key != NULL );
    ECDH_VALIDATE_RET( side == MBEDTLS_ECDH_OURS ||
                       side == MBEDTLS_ECDH_THEIRS );

    if (mbedtls_ecdh_grp_id(ctx) == MBEDTLS_ECP_DP_NONE)
    {
        /* This is the first call to get_params(). Set up the context
         * for use with the group. */
        if ((ret = mbedtls_ecdh_setup(ctx, key->MBEDTLS_PRIVATE(grp).id)) != 0)
        {
            return (ret);
        }
    }
    else
    {
        /* This is not the first call to get_params(). Check that the
         * current key's group is the same as the context's, which was set
         * from the first key's group. */
        if (mbedtls_ecdh_grp_id(ctx) != key->MBEDTLS_PRIVATE(grp).id)
        {
            return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
        }
    }

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_get_params_internal(ctx, key, side));
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        case MBEDTLS_ECDH_VARIANT_EVEREST:
        {
            mbedtls_everest_ecdh_side s = side == MBEDTLS_ECDH_OURS ?
                                                   MBEDTLS_EVEREST_ECDH_OURS :
                                                   MBEDTLS_EVEREST_ECDH_THEIRS;
            return( mbedtls_everest_get_params( &ctx->ctx.everest_ecdh,
                                                key, s) );
        }
#endif
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            return (ecdh_get_params_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), key, side));
        default:
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
#endif
}

/*
 * Setup and export the client public value
 */
int mbedtls_ecdh_make_public(mbedtls_ecdh_context *ctx,
                             size_t *olen,
                             unsigned char *buf,
                             size_t blen,
                             int (*f_rng)(void *, unsigned char *, size_t),
                             void *p_rng)
{
    int ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    ECDH_VALIDATE_RET(ctx != NULL);
    size_t coordinateLen     = (ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp).pbits + 7u) / 8u;
    size_t coordinateBitsLen = ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp).pbits;
    size_t keySize           = 2u * coordinateLen;
    uint8_t *pubKey = NULL;
    do {
        pubKey= mbedtls_calloc(keySize, sizeof(uint8_t));
        if (pubKey == NULL)
        {
            break;
        }
        if (CRYPTO_InitHardware() != kStatus_Success)
        {
            mbedtls_platform_zeroize(pubKey, keySize);
            mbedtls_free(pubKey);
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
        if (ctx->isKeyInitialized == false)
        {
            if (sss_sscp_key_object_init(&ctx->key, &g_keyStore) != kStatus_SSS_Success)
            {
                (void)SSS_KEY_OBJ_FREE(&ctx->key);
                break;\
            }
        /* Allocate key handle */
            if (sss_sscp_key_object_allocate_handle(&ctx->key,
                                                 0u,
                                                 kSSS_KeyPart_Pair,
                                                 kSSS_CipherType_EC_NIST_P,
                                                 3u * coordinateLen,
                                                 SSS_KEYPROP_OPERATION_KDF) != kStatus_SSS_Success)
            {
                (void)SSS_KEY_OBJ_FREE(&ctx->key);
                break;;
            }
                ctx->isKeyInitialized = true;
        }
        if (SSS_ECP_GENERATE_KEY(&ctx->key, coordinateBitsLen) != kStatus_SSS_Success)
        {
            break;
        }
        if (SSS_KEY_STORE_GET_PUBKEY(&ctx->key,
                                      pubKey,
                                      &keySize,
                                      &coordinateBitsLen) != kStatus_SSS_Success)
        {
            break;
        }
        if ((ret = mbedtls_mpi_read_binary(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(X), pubKey, coordinateLen)) != 0)
        {
            break;
        }
        if ((ret = mbedtls_mpi_read_binary(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Y), &pubKey[coordinateLen], coordinateLen)) != 0)
        {
            break;
        }
        if ((ret = mbedtls_mpi_lset(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q).MBEDTLS_PRIVATE(Z), 1)) != 0)
        {
            break;
        }

        mbedtls_ecp_tls_write_point(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q), ctx->MBEDTLS_PRIVATE(point_format), olen, buf, blen);
        ret = 0;
    } while (0);

    if (pubKey != NULL)
    {
        mbedtls_platform_zeroize(pubKey, keySize);
        mbedtls_free(pubKey);
    }
    return ret;
}

static int ecdh_read_public_internal(mbedtls_ecdh_context_mbed *ctx, const unsigned char *buf, size_t blen)
{
    int ret;
    const unsigned char *p = buf;

    if ((ret = mbedtls_ecp_tls_read_point(&ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(Qp), &p, blen)) != 0)
    {
        return (ret);
    }

    if ((size_t)(p - buf) != blen)
    {
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }

    return (0);
}

/*
 * Parse and import the client's public value
 */
int mbedtls_ecdh_read_public(mbedtls_ecdh_context *ctx, const unsigned char *buf, size_t blen)
{
    ECDH_VALIDATE_RET(ctx != NULL);
    ECDH_VALIDATE_RET(buf != NULL);

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_read_public_internal(ctx, buf, blen));
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
 #if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        case MBEDTLS_ECDH_VARIANT_EVEREST:
            return( mbedtls_everest_read_public( &ctx->ctx.everest_ecdh,
                                                 buf, blen ) );
#endif
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            return (ecdh_read_public_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), buf, blen));
        default:
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
#endif
}


/*
 * Derive and export the shared secret
 */
int mbedtls_ecdh_calc_secret(mbedtls_ecdh_context *ctx,
                             size_t *olen,
                             unsigned char *buf,
                             size_t blen,
                             int (*f_rng)(void *, unsigned char *, size_t),
                             void *p_rng)
{
    int ret = 0;
    ECDH_VALIDATE_RET(ctx != NULL);

    sss_sscp_derive_key_t dCtx;
    size_t coordinateLen     = (ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp).pbits + 7u) / 8u;
    size_t coordinateBitsLen = ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(grp).pbits;
    size_t keySize           = SSS_ECP_KEY_SZ(coordinateLen);
    uint8_t *pubKey          = mbedtls_calloc(keySize, sizeof(uint8_t));
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_key_object_init(&ctx->peerPublicKey, &g_keyStore) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_key_object_allocate_handle(&ctx->peerPublicKey,
                                                 1u,
                                                 kSSS_KeyPart_Public,
                                                 kSSS_CipherType_EC_NIST_P,
                                                 3u * coordinateLen,
                                                 SSS_KEYPROP_OPERATION_KDF) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((ret = mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Qp).MBEDTLS_PRIVATE(X), pubKey, coordinateLen)) != 0)
    {
    }
    else if ((ret = mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Qp).MBEDTLS_PRIVATE(Y), &pubKey[coordinateLen], coordinateLen)) != 0)
    {
    }
    else if (SSS_KEY_STORE_SET_KEY(&ctx->peerPublicKey,
                                   (const uint8_t *)pubKey,
                                   keySize,
                                   coordinateBitsLen,
                                   kSSS_KeyPart_Public)!= kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_key_object_init(&ctx->sharedSecret, &g_keyStore) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_key_object_allocate_handle(&ctx->sharedSecret,
                                                 2u,
                                                 kSSS_KeyPart_Default,
                                                 kSSS_CipherType_AES,
                                                 coordinateLen,
                                                 SSS_KEYPROP_OPERATION_NONE
                                                 ) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_derive_key_context_init(&dCtx, &g_sssSession, &ctx->key, kAlgorithm_SSS_ECDH,
                                              kMode_SSS_ComputeSharedSecret) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_asymmetric_dh_derive_key(&dCtx, &ctx->peerPublicKey, &ctx->sharedSecret) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if (sss_sscp_key_store_get_key(&g_keyStore, &ctx->sharedSecret, pubKey, &coordinateLen, &coordinateBitsLen,
                                        (sss_key_part_t)NULL) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((ret = mbedtls_mpi_read_binary(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(z), pubKey, coordinateLen)) != 0)
    {
    }
    else
    {
        ret = 0;
    }
    (void)sss_sscp_derive_key_context_free(&dCtx);
    (void)SSS_KEY_OBJ_FREE(&ctx->peerPublicKey);
    (void)SSS_KEY_OBJ_FREE(&ctx->sharedSecret);

    mbedtls_platform_zeroize(pubKey, keySize);
    mbedtls_free(pubKey);
    return ret;
}

/* test suite functions*/
#if defined(MBEDTLS_SELF_TEST)
static int ecdh_calc_secret_internal(mbedtls_ecdh_context_mbed *ctx,
                                     size_t *olen,
                                     unsigned char *buf,
                                     size_t blen,
                                     int (*f_rng)(void *, unsigned char *, size_t),
                                     void *p_rng,
                                     int restart_enabled)
{
    int ret;
    if (ctx == NULL || ctx->MBEDTLS_PRIVATE(grp).pbits == 0)
    {
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }
    (void)restart_enabled;
    if ((ret = mbedtls_ecdh_compute_shared(&ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(z), &ctx->MBEDTLS_PRIVATE(Qp), &ctx->MBEDTLS_PRIVATE(d), f_rng, p_rng)) != 0)
    {
        return (ret);
    }

    if (mbedtls_mpi_size(&ctx->MBEDTLS_PRIVATE(z)) > blen)
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    *olen = ctx->MBEDTLS_PRIVATE(grp).pbits / 8 + ((ctx->MBEDTLS_PRIVATE(grp).pbits % 8) != 0);
    return mbedtls_mpi_write_binary(&ctx->MBEDTLS_PRIVATE(z), buf, *olen);
}

/*
 * Derive and export the shared secret
 */
int mbedtls_ecdh_calc_secret_sw(mbedtls_ecdh_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng)
{
    int restart_enabled = 0;
    ECDH_VALIDATE_RET(ctx != NULL);
    ECDH_VALIDATE_RET(olen != NULL);
    ECDH_VALIDATE_RET(buf != NULL);

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_calc_secret_internal(ctx, olen, buf, blen, f_rng, p_rng, restart_enabled));
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            return (ecdh_calc_secret_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), olen, buf, blen, f_rng, p_rng, restart_enabled));
        default:
            return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }
#endif
}

static int ecdh_make_public_internal(mbedtls_ecdh_context_mbed *ctx,
                                     size_t *olen,
                                     int point_format,
                                     unsigned char *buf,
                                     size_t blen,
                                     int (*f_rng)(void *, unsigned char *, size_t),
                                     void *p_rng,
                                     int restart_enabled)
{
    int ret;

    if (ctx->MBEDTLS_PRIVATE(grp).pbits == 0)
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    (void)restart_enabled;

    if ((ret = mbedtls_ecdh_gen_public(&ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(d), &ctx->MBEDTLS_PRIVATE(Q), f_rng, p_rng)) != 0)
        return (ret);

    return mbedtls_ecp_tls_write_point(&ctx->MBEDTLS_PRIVATE(grp), &ctx->MBEDTLS_PRIVATE(Q), point_format, olen, buf, blen);
}

/*
 * Setup and export the client public value
 */
int mbedtls_ecdh_make_public_sw(mbedtls_ecdh_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng)
{
    int restart_enabled = 0;
    ECDH_VALIDATE_RET(ctx != NULL);
    ECDH_VALIDATE_RET(olen != NULL);
    ECDH_VALIDATE_RET(buf != NULL);
    ECDH_VALIDATE_RET(f_rng != NULL);

#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    return (ecdh_make_public_internal(ctx, olen, ctx->MBEDTLS_PRIVATE(point_format), buf, blen, f_rng, p_rng, restart_enabled));
#else
    switch (ctx->MBEDTLS_PRIVATE(var))
    {
        case MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0:
            return (ecdh_make_public_internal(&ctx->MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh), olen, ctx->MBEDTLS_PRIVATE(point_format), buf, blen, f_rng, p_rng,
                                              restart_enabled));
        default:
            return MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    }
#endif
}

static int myrand(void *rng_state, unsigned char *output, size_t len)
{
    size_t use_len;
    int rnd;

    if (rng_state != NULL)
        rng_state = NULL;

    while (len > 0U)
    {
        use_len = len;
        if (use_len > sizeof(int))
        {
            use_len = sizeof(int);
        }
        rnd = rand();
        (void)memcpy(output, (unsigned char *)&rnd, use_len);
        output += use_len;
        len -= use_len;
    }

    return (0);
}

int mbedtls_ecdh_self_test(int verbose)
{
    int ret = 0;
    uint8_t buf[100];
    mbedtls_ecdh_context ecdhClient, ecdhServer;
    const mbedtls_ecp_curve_info *curve_info = mbedtls_ecp_curve_list();
    size_t olen;

    for (uint32_t i = 0; curve_info->grp_id != MBEDTLS_ECP_DP_NONE; curve_info++, i++)
    {
        if (verbose != 0)
        {
            (void)mbedtls_printf("  ECDH %s - #%u: ", curve_info->name, i + 1u);
        }
        mbedtls_ecdh_init(&ecdhClient);
        mbedtls_ecdh_init(&ecdhServer);

        if ((ret = mbedtls_ecdh_setup(&ecdhClient, curve_info->grp_id)) != 0 ||
            (ret = mbedtls_ecdh_make_public(&ecdhClient, &olen, buf, sizeof(buf), myrand, NULL)) != 0)
        {
            if (verbose != 0)
            {
                (void)mbedtls_printf("failed\n");
            }
            return ret;
        }
        if ((ret = mbedtls_ecdh_setup(&ecdhServer, curve_info->grp_id)) != 0 ||
            (ret = mbedtls_ecdh_make_public_sw(&ecdhServer, &olen, buf, sizeof(buf), myrand, NULL)) != 0)
        {
            if (verbose != 0)
            {
                (void)mbedtls_printf("failed\n");
            }
            return ret;
        }

        (void)mbedtls_ecp_copy(&ecdhServer.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Qp), &ecdhClient.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q));
        (void)mbedtls_ecp_copy(&ecdhClient.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Qp), &ecdhServer.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(Q));

        ret = mbedtls_ecdh_calc_secret(&ecdhClient, &olen, buf, sizeof(buf), myrand, NULL);
        ret = mbedtls_ecdh_calc_secret_sw(&ecdhServer, &olen, buf, sizeof(buf), myrand, NULL);

        if (ret != 0 || (ret = memcmp(ecdhClient.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(z).MBEDTLS_PRIVATE(p), ecdhServer.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(z).MBEDTLS_PRIVATE(p),sizeof(mbedtls_mpi_uint) * ecdhClient.MBEDTLS_PRIVATE(ctx).MBEDTLS_PRIVATE(mbed_ecdh).MBEDTLS_PRIVATE(z).MBEDTLS_PRIVATE(n))) != 0)
        {
            if (verbose != 0)
            {
                (void)mbedtls_printf("failed\n");
            }

            return ret;
        }
        mbedtls_ecdh_free(&ecdhServer);
        mbedtls_ecdh_free(&ecdhClient);

        if (verbose != 0)
        {
            (void)mbedtls_printf("passed\n");
        }
        if (verbose != 0)
        {
            (void)mbedtls_printf("\n");
        }
    }

    return ret;
}
#endif /* MBEDTLS_SELF_TEST */

#endif /* MBEDTLS_MCUX_SSS_ECDH && MBEDTLS_ECDH_ALT && MBEDTLS_ECDH_C */

/******************************************************************************/
/*************************** CCM *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_SSS_CCM) && defined(MBEDTLS_CCM_ALT) && defined(MBEDTLS_CCM_C)
#include "mbedtls/ccm.h"

#define CCM_VALIDATE_RET(cond) MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_CCM_BAD_INPUT)
#define CCM_VALIDATE(cond)     MBEDTLS_INTERNAL_VALIDATE(cond)

#define CCM_ENCRYPT 0
#define CCM_DECRYPT 1

/*
 * Initialize context
 */
void mbedtls_ccm_init(mbedtls_ccm_context *ctx)
{
    CCM_VALIDATE(ctx != NULL);
    (void)memset(ctx, 0, sizeof(mbedtls_ccm_context));
}

int mbedtls_ccm_setkey(mbedtls_ccm_context *ctx,
                       mbedtls_cipher_id_t cipher,
                       const unsigned char *key,
                       unsigned int keybits)
{
    int ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    uint8_t ramKey[32];
    (void)memcpy(ramKey, key, (keybits + 7u) / 8u);
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((sss_sscp_key_object_init(&ctx->key, &g_keyStore)) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((sss_sscp_key_object_allocate_handle(&ctx->key, 1u, kSSS_KeyPart_Default, kSSS_CipherType_AES,
                                                  (keybits + 7u) / 8u, SSS_KEYPROP_OPERATION_AEAD)) !=
             kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else if ((sss_sscp_key_store_set_key(&g_keyStore, &ctx->key, ramKey, (keybits + 7u) / 8u, keybits,
                                         kSSS_KeyPart_Default)) != kStatus_SSS_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*
 * Free context
 */
void mbedtls_ccm_free(mbedtls_ccm_context *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
    }
    else if (SSS_KEY_OBJ_FREE(&ctx->key) != kStatus_SSS_Success)
    {
    }
    else
    {
        /* Intentional empty */
    }
    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_ccm_context));
}

/*
 * Authenticated encryption or decryption
 */
#if !defined(MBEDTLS_CCM_CRYPT_ALT)
/* CCM selftest fails on ARM Cortex M with IAR 8.11 with common subexpression elimination optimalization enabled */
#if defined(__ICCARM__)
#pragma optimize = no_cse
#endif
static int ccm_auth_crypt(mbedtls_ccm_context *ctx,
                          int mode,
                          size_t length,
                          const unsigned char *iv,
                          size_t iv_len,
                          const unsigned char *add,
                          size_t add_len,
                          const unsigned char *input,
                          unsigned char *output,
                          unsigned char *tag,
                          size_t tag_len)
{
    int ret = -100;
    sss_sscp_aead_t aeadCtx;
    size_t tlen        = tag_len;
    sss_mode_t sssMode = kMode_SSS_Encrypt;
    do
    {
        if (mode == CCM_ENCRYPT)
        {
            sssMode = kMode_SSS_Encrypt;
        }
        else if (mode == CCM_DECRYPT)
        {
            sssMode = kMode_SSS_Decrypt;
        }
        else
        {
            ret = MBEDTLS_ERR_CCM_BAD_INPUT;
            break;
        }

        if (CRYPTO_InitHardware() != kStatus_Success)
        {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            break;
        }
        if (sss_sscp_aead_context_init(&aeadCtx, &g_sssSession, &ctx->key, kAlgorithm_SSS_AES_CCM, sssMode) !=
              kStatus_SSS_Success)
        {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            break;
        }
        /* RUN AEAD */
        if (sss_sscp_aead_one_go(&aeadCtx, input, output, length,
                                 (uint8_t *)(uintptr_t)iv, iv_len,
                                 add, add_len,
                                 tag, &tlen) != kStatus_SSS_Success)
        {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            /* do not break; */
        }
        /* Free AEAD context it its init worked whether AEAD operation succeeded or not */
        if (sss_sscp_aead_context_free(&aeadCtx) != kStatus_SSS_Success)
        {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            break;
        }
        else
        {
            ret = ret != MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
    } while (0);

    return ret;

}

/*
 * Authenticated encryption
 */
int mbedtls_ccm_star_encrypt_and_tag(mbedtls_ccm_context *ctx,
                                     size_t length,
                                     const unsigned char *iv,
                                     size_t iv_len,
                                     const unsigned char *add,
                                     size_t add_len,
                                     const unsigned char *input,
                                     unsigned char *output,
                                     unsigned char *tag,
                                     size_t tag_len)
{
    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(iv != NULL);
    CCM_VALIDATE_RET(add_len == 0 || add != NULL);
    CCM_VALIDATE_RET(length == 0 || input != NULL);
    CCM_VALIDATE_RET(length == 0 || output != NULL);
    CCM_VALIDATE_RET(tag_len == 0 || tag != NULL);
    return (ccm_auth_crypt(ctx, CCM_ENCRYPT, length, iv, iv_len, add, add_len, input, output, tag, tag_len));
}

int mbedtls_ccm_encrypt_and_tag(mbedtls_ccm_context *ctx,
                                size_t length,
                                const unsigned char *iv,
                                size_t iv_len,
                                const unsigned char *add,
                                size_t add_len,
                                const unsigned char *input,
                                unsigned char *output,
                                unsigned char *tag,
                                size_t tag_len)
{
    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(iv != NULL);
    CCM_VALIDATE_RET(add_len == 0 || add != NULL);
    CCM_VALIDATE_RET(length == 0 || input != NULL);
    CCM_VALIDATE_RET(length == 0 || output != NULL);
    CCM_VALIDATE_RET(tag_len == 0 || tag != NULL);
    if (tag_len == 0u)
    {
        return (MBEDTLS_ERR_CCM_BAD_INPUT);
    }

    return (mbedtls_ccm_star_encrypt_and_tag(ctx, length, iv, iv_len, add, add_len, input, output, tag, tag_len));
}

/*
 * Authenticated decryption
 */
int mbedtls_ccm_star_auth_decrypt(mbedtls_ccm_context *ctx,
                                  size_t length,
                                  const unsigned char *iv,
                                  size_t iv_len,
                                  const unsigned char *add,
                                  size_t add_len,
                                  const unsigned char *input,
                                  unsigned char *output,
                                  const unsigned char *tag,
                                  size_t tag_len)
{
    int ret;

    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(iv != NULL);
    CCM_VALIDATE_RET(add_len == 0 || add != NULL);
    CCM_VALIDATE_RET(length == 0 || input != NULL);
    CCM_VALIDATE_RET(length == 0 || output != NULL);
    CCM_VALIDATE_RET(tag_len == 0 || tag != NULL);

    ret = ccm_auth_crypt(ctx, CCM_DECRYPT, length, iv, iv_len, add, add_len, input, output,
                         (unsigned char *)(uintptr_t)tag, tag_len);
    if (ret != 0)
    {
        return (ret);
    }
    return (0);
}

int mbedtls_ccm_auth_decrypt(mbedtls_ccm_context *ctx,
                             size_t length,
                             const unsigned char *iv,
                             size_t iv_len,
                             const unsigned char *add,
                             size_t add_len,
                             const unsigned char *input,
                             unsigned char *output,
                             const unsigned char *tag,
                             size_t tag_len)
{
    CCM_VALIDATE_RET(ctx != NULL);
    CCM_VALIDATE_RET(iv != NULL);
    CCM_VALIDATE_RET(add_len == 0 || add != NULL);
    CCM_VALIDATE_RET(length == 0 || input != NULL);
    CCM_VALIDATE_RET(length == 0 || output != NULL);
    CCM_VALIDATE_RET(tag_len == 0 || tag != NULL);

    if (tag_len == 0u)
    {
        return (MBEDTLS_ERR_CCM_BAD_INPUT);
    }

    return (mbedtls_ccm_star_auth_decrypt(ctx, length, iv, iv_len, add, add_len, input, output, tag, tag_len));
}


#endif /* MBEDTLS_CCM_CRYPT_ALT */

#endif /* MBEDTLS_MCUX_SSS_CCM && MBEDTLS_CCM_ALT && MBEDTLS_CCM_C */

/******************************************************************************/
/*************************** AES *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_SSS_AES) && defined(MBEDTLS_AES_ALT) && defined(MBEDTLS_AES_C)

#include <mbedtls/aes.h>

/* Parameter validation macros based on platform_util.h */
#define AES_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_AES_BAD_INPUT_DATA )
#define AES_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    AES_VALIDATE( ctx != NULL );

    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_aes_context ) );
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
void mbedtls_aes_xts_init( mbedtls_aes_xts_context *ctx )
{
    AES_VALIDATE( ctx != NULL );

    mbedtls_aes_init( &ctx->crypt );
    mbedtls_aes_init( &ctx->tweak );
}

void mbedtls_aes_xts_free( mbedtls_aes_xts_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_aes_free( &ctx->crypt );
    mbedtls_aes_free( &ctx->tweak );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    switch (keybits)
    {
        case 128:
            ctx->keySize = 16;
            break;
        case 192:
            ctx->keySize = 24;
            break;
        case 256:
            ctx->keySize = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }

    memcpy(ctx->key, key, ctx->keySize);

    return (0);
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    return mbedtls_aes_setkey_enc(ctx, key, keybits);
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
static int mbedtls_aes_xts_decode_keys( const unsigned char *key,
                                        unsigned int keybits,
                                        const unsigned char **key1,
                                        unsigned int *key1bits,
                                        const unsigned char **key2,
                                        unsigned int *key2bits )
{
    const unsigned int half_keybits = keybits / 2;
    const unsigned int half_keybytes = half_keybits / 8;

    switch( keybits )
    {
        case 256: break;
        case 512: break;
        default : return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    *key1bits = half_keybits;
    *key2bits = half_keybits;
    *key1 = &key[0];
    *key2 = &key[half_keybytes];

    return 0;
}

int mbedtls_aes_xts_setkey_enc( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( key != NULL );

    ret = mbedtls_aes_xts_decode_keys( key, keybits, &key1, &key1bits,
                                       &key2, &key2bits );
    if( ret != 0 )
        return( ret );

    /* Set the tweak key. Always set tweak key for the encryption mode. */
    ret = mbedtls_aes_setkey_enc( &ctx->tweak, key2, key2bits );
    if( ret != 0 )
        return( ret );

    /* Set crypt key for encryption. */
    return mbedtls_aes_setkey_enc( &ctx->crypt, key1, key1bits );
}

int mbedtls_aes_xts_setkey_dec( mbedtls_aes_xts_context *ctx,
                                const unsigned char *key,
                                unsigned int keybits)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( key != NULL );

    ret = mbedtls_aes_xts_decode_keys( key, keybits, &key1, &key1bits,
                                       &key2, &key2bits );
    if( ret != 0 )
        return( ret );

    /* Set the tweak key. Always set tweak key for encryption. */
    ret = mbedtls_aes_setkey_enc( &ctx->tweak, key2, key2bits );
    if( ret != 0 )
        return( ret );

    /* Set crypt key for decryption. */
    return mbedtls_aes_setkey_dec( &ctx->crypt, key1, key1bits );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    sss_sscp_symmetric_t aesCtx;
    sss_sscp_object_t sssKey;

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    if ((sss_sscp_key_object_init(&sssKey, &g_keyStore)) != kStatus_SSS_Success)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if ((SSS_KEY_ALLOCATE_HANDLE(&sssKey, 1u, /* key id */
                                 kSSS_KeyPart_Default, kSSS_CipherType_AES, ctx->keySize, SSS_KEYPROP_OPERATION_AES)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }
    if ((SSS_KEY_STORE_SET_KEY(&sssKey, ctx->key, ctx->keySize, (ctx->keySize << 3), kSSS_KeyPart_Default)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if ((sss_sscp_symmetric_context_init(&aesCtx, &g_sssSession, &sssKey, kAlgorithm_SSS_AES_ECB, kMode_SSS_Encrypt)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    /* RUN AES */
    if ((sss_sscp_cipher_one_go(&aesCtx, NULL, 0, input, output, 16)) != kStatus_SSS_Success)
    {
        
        (void)sss_sscp_symmetric_context_free(&aesCtx);
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    /* Free AES context whether AES operation succeeded or not */
    if (sss_sscp_symmetric_context_free(&aesCtx) != kStatus_SSS_Success)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }
    
    (void)SSS_KEY_OBJ_FREE(&sssKey);

    return 0;
}


/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    sss_sscp_symmetric_t aesCtx;
    sss_sscp_object_t sssKey;

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    if ((sss_sscp_key_object_init(&sssKey, &g_keyStore)) != kStatus_SSS_Success)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if ((SSS_KEY_ALLOCATE_HANDLE(&sssKey, 1u, /* key id */
                                 kSSS_KeyPart_Default, kSSS_CipherType_AES, ctx->keySize, SSS_KEYPROP_OPERATION_AES)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }
    if ((SSS_KEY_STORE_SET_KEY(&sssKey, ctx->key, ctx->keySize, (ctx->keySize << 3), kSSS_KeyPart_Default)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if ((sss_sscp_symmetric_context_init(&aesCtx, &g_sssSession, &sssKey, kAlgorithm_SSS_AES_ECB, kMode_SSS_Decrypt)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    /* RUN AES */
    if ((sss_sscp_cipher_one_go(&aesCtx, NULL, 0, input, output, 16)) != kStatus_SSS_Success)
    {
        
        (void)sss_sscp_symmetric_context_free(&aesCtx);
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    /* Free AES context whether AES operation succeeded or not */
    if (sss_sscp_symmetric_context_free(&aesCtx) != kStatus_SSS_Success)
    {
        
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }
    
    (void)SSS_KEY_OBJ_FREE(&sssKey);

    return 0;
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );

#if defined(MBEDTLS_AESNI_C) && defined(MBEDTLS_HAVE_X86_64)
    if( mbedtls_aesni_has_support( MBEDTLS_AESNI_AES ) )
        return( mbedtls_aesni_crypt_ecb( ctx, mode, input, output ) );
#endif

#if defined(MBEDTLS_PADLOCK_C) && defined(MBEDTLS_HAVE_X86)
    if( aes_padlock_ace )
    {
        if( mbedtls_padlock_xcryptecb( ctx, mode, input, output ) == 0 )
            return( 0 );

        // If padlock data misaligned, we just fall back to
        // unaccelerated mode
        //
    }
#endif

    if( mode == MBEDTLS_AES_ENCRYPT )
        return( mbedtls_internal_aes_encrypt( ctx, input, output ) );
    else
        return( mbedtls_internal_aes_decrypt( ctx, input, output ) );
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT || mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    if (length % 16)
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);

    sss_sscp_symmetric_t aesCtx;
    sss_sscp_object_t sssKey;

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if ((sss_sscp_key_object_init(&sssKey, &g_keyStore)) != kStatus_SSS_Success)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if ((SSS_KEY_ALLOCATE_HANDLE(&sssKey, 1u, /* key id */
                                 kSSS_KeyPart_Default, kSSS_CipherType_AES, ctx->keySize, SSS_KEYPROP_OPERATION_AES)) !=
        kStatus_SSS_Success)
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }
    if ((SSS_KEY_STORE_SET_KEY(&sssKey, ctx->key, ctx->keySize, (ctx->keySize << 3), kSSS_KeyPart_Default)) !=
        kStatus_SSS_Success)
    {
        (void)SSS_KEY_OBJ_FREE(&sssKey);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if (mode == MBEDTLS_AES_DECRYPT)
    {
        uint8_t tmp[16];
        memcpy(tmp, input + length - 16, 16);

        if ((sss_sscp_symmetric_context_init(&aesCtx, &g_sssSession, &sssKey, kAlgorithm_SSS_AES_CBC,
                                             kMode_SSS_Decrypt)) != kStatus_SSS_Success)
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }

        /* RUN AES */
        if ((sss_sscp_cipher_one_go(&aesCtx, iv, 16, input, output, 16)) != kStatus_SSS_Success)
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }

        /* Free AES context it its init worked whether AES operation succeeded or not */
        if (sss_sscp_symmetric_context_free(&aesCtx) != kStatus_SSS_Success)
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }

        memcpy(iv, tmp, 16);
    }
    else
    {
        if ((sss_sscp_symmetric_context_init(&aesCtx, &g_sssSession, &sssKey, kAlgorithm_SSS_AES_CBC,
                                             kMode_SSS_Encrypt)) != kStatus_SSS_Success)
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }

        /* RUN AES */
        if ((sss_sscp_cipher_one_go(&aesCtx, iv, 16, input, output, 16)) != kStatus_SSS_Success)
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }

        /* Free AES context it its init worked whether AES operation succeeded or not */
        if (sss_sscp_symmetric_context_free(&aesCtx) != kStatus_SSS_Success)
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }

        memcpy(iv, output + length - 16, 16);
    }

    (void)SSS_KEY_OBJ_FREE(&sssKey);

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_XTS)

/* Endianess with 64 bits values */
#ifndef GET_UINT64_LE
#define GET_UINT64_LE(n,b,i)                            \
{                                                       \
    (n) = ( (uint64_t) (b)[(i) + 7] << 56 )             \
        | ( (uint64_t) (b)[(i) + 6] << 48 )             \
        | ( (uint64_t) (b)[(i) + 5] << 40 )             \
        | ( (uint64_t) (b)[(i) + 4] << 32 )             \
        | ( (uint64_t) (b)[(i) + 3] << 24 )             \
        | ( (uint64_t) (b)[(i) + 2] << 16 )             \
        | ( (uint64_t) (b)[(i) + 1] <<  8 )             \
        | ( (uint64_t) (b)[(i)    ]       );            \
}
#endif

#ifndef PUT_UINT64_LE
#define PUT_UINT64_LE(n,b,i)                            \
{                                                       \
    (b)[(i) + 7] = (unsigned char) ( (n) >> 56 );       \
    (b)[(i) + 6] = (unsigned char) ( (n) >> 48 );       \
    (b)[(i) + 5] = (unsigned char) ( (n) >> 40 );       \
    (b)[(i) + 4] = (unsigned char) ( (n) >> 32 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
}
#endif

typedef unsigned char mbedtls_be128[16];

/*
 * GF(2^128) multiplication function
 *
 * This function multiplies a field element by x in the polynomial field
 * representation. It uses 64-bit word operations to gain speed but compensates
 * for machine endianess and hence works correctly on both big and little
 * endian machines.
 */
static void mbedtls_gf128mul_x_ble( unsigned char r[16],
                                    const unsigned char x[16] )
{
    uint64_t a, b, ra, rb;

    GET_UINT64_LE( a, x, 0 );
    GET_UINT64_LE( b, x, 8 );

    ra = ( a << 1 )  ^ 0x0087 >> ( 8 - ( ( b >> 63 ) << 3 ) );
    rb = ( a >> 63 ) | ( b << 1 );

    PUT_UINT64_LE( ra, r, 0 );
    PUT_UINT64_LE( rb, r, 8 );
}

/*
 * AES-XTS buffer encryption/decryption
 */
int mbedtls_aes_crypt_xts( mbedtls_aes_xts_context *ctx,
                           int mode,
                           size_t length,
                           const unsigned char data_unit[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t blocks = length / 16;
    size_t leftover = length % 16;
    unsigned char tweak[16];
    unsigned char prev_tweak[16];
    unsigned char tmp[16];

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    AES_VALIDATE_RET( data_unit != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    /* Data units must be at least 16 bytes long. */
    if( length < 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* NIST SP 800-38E disallows data units larger than 2**20 blocks. */
    if( length > ( 1 << 20 ) * 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* Compute the tweak. */
    ret = mbedtls_aes_crypt_ecb( &ctx->tweak, MBEDTLS_AES_ENCRYPT,
                                 data_unit, tweak );
    if( ret != 0 )
        return( ret );

    while( blocks-- )
    {
        size_t i;

        if( leftover && ( mode == MBEDTLS_AES_DECRYPT ) && blocks == 0 )
        {
            /* We are on the last block in a decrypt operation that has
             * leftover bytes, so we need to use the next tweak for this block,
             * and this tweak for the lefover bytes. Save the current tweak for
             * the leftovers and then update the current tweak for use on this,
             * the last full block. */
            memcpy( prev_tweak, tweak, sizeof( tweak ) );
            mbedtls_gf128mul_x_ble( tweak, tweak );
        }

        for( i = 0; i < 16; i++ )
            tmp[i] = input[i] ^ tweak[i];

        ret = mbedtls_aes_crypt_ecb( &ctx->crypt, mode, tmp, tmp );
        if( ret != 0 )
            return( ret );

        for( i = 0; i < 16; i++ )
            output[i] = tmp[i] ^ tweak[i];

        /* Update the tweak for the next block. */
        mbedtls_gf128mul_x_ble( tweak, tweak );

        output += 16;
        input += 16;
    }

    if( leftover )
    {
        /* If we are on the leftover bytes in a decrypt operation, we need to
         * use the previous tweak for these bytes (as saved in prev_tweak). */
        unsigned char *t = mode == MBEDTLS_AES_DECRYPT ? prev_tweak : tweak;

        /* We are now on the final part of the data unit, which doesn't divide
         * evenly by 16. It's time for ciphertext stealing. */
        size_t i;
        unsigned char *prev_output = output - 16;

        /* Copy ciphertext bytes from the previous block to our output for each
         * byte of cyphertext we won't steal. At the same time, copy the
         * remainder of the input for this final round (since the loop bounds
         * are the same). */
        for( i = 0; i < leftover; i++ )
        {
            output[i] = prev_output[i];
            tmp[i] = input[i] ^ t[i];
        }

        /* Copy ciphertext bytes from the previous block for input in this
         * round. */
        for( ; i < 16; i++ )
            tmp[i] = prev_output[i] ^ t[i];

        ret = mbedtls_aes_crypt_ecb( &ctx->crypt, mode, tmp, tmp );
        if( ret != 0 )
            return ret;

        /* Write the result back to the previous block, overriding the previous
         * output we copied. */
        for( i = 0; i < 16; i++ )
            prev_output[i] = tmp[i] ^ t[i];
    }

    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
#if !defined(MBEDTLS_AES_CRYPT_CFB_ALT)
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    int c;
    size_t n;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    AES_VALIDATE_RET( iv_off != NULL );
    AES_VALIDATE_RET( iv != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    n = *iv_off;

    if( n > 15 )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    if( mode == MBEDTLS_AES_DECRYPT )
    {
        while( length-- )
        {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

            c = *input++;
            *output++ = (unsigned char)( c ^ iv[n] );
            iv[n] = (unsigned char) c;

            n = ( n + 1 ) & 0x0F;
        }
    }
    else
    {
        while( length-- )
        {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

            iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );

            n = ( n + 1 ) & 0x0F;
        }
    }

    *iv_off = n;

    return( 0 );
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
    unsigned char c;
    unsigned char ov[17];

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    AES_VALIDATE_RET( iv != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );
    while( length-- )
    {
        memcpy( ov, iv, 16 );
        mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

    return( 0 );
}
#endif /* !MBEDTLS_AES_CRYPT_CFB_ALT */
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int mbedtls_aes_crypt_ofb( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *iv_off,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = 0;
    size_t n;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( iv_off != NULL );
    AES_VALIDATE_RET( iv != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    n = *iv_off;

    if( n > 15 )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    while( length-- )
    {
        if( n == 0 )
        {
            ret = mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
            if( ret != 0 )
                goto exit;
        }
        *output++ =  *input++ ^ iv[n];

        n = ( n + 1 ) & 0x0F;
    }

    *iv_off = n;

exit:
    return( ret );
}
#endif /* MBEDTLS_CIPHER_MODE_OFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
#if !defined(MBEDTLS_AES_CRYPT_CTR_ALT)
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    int c, i;
    size_t n;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( nc_off != NULL );
    AES_VALIDATE_RET( nonce_counter != NULL );
    AES_VALIDATE_RET( stream_block != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    n = *nc_off;

    if ( n > 0x0F )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

    while( length-- )
    {
        if( n == 0 ) {
            mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block );

            for( i = 16; i > 0; i-- )
                if( ++nonce_counter[i - 1] != 0 )
                    break;
        }
        c = *input++;
        *output++ = (unsigned char)( c ^ stream_block[n] );

        n = ( n + 1 ) & 0x0F;
    }

    *nc_off = n;

    return( 0 );
}
#endif /* !MBEDTLS_AES_CRYPT_CTR_ALT */
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_MCUX_SSS_AES && MBEDTLS_AES_ALT && MBEDTLS_AES_C */

#endif /* FSL_FEATURE_EDGELOCK */
