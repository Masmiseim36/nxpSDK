/**
 * \file ele_s400/ecc_opaque/ecdsa_alt.c
 *
 * \brief This file contains alternative ECDSA definitions and functions.
 *
 * \warning This implementation is not compatible with some of the other
 * MbedTLS features (e.g. PK module key material loading/cert parsing for RSA).
 * It is only intended to showcase P-256 opaque key handling via the EdgeLock
 * Enclave (ELE).
 *
 */
/*
 *  Elliptic curve DSA
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* Guarding this file, so it can only be used when it's really meant to be */
#if defined(NXP_ELE_ECC_OPAQUE_KEY)

#if defined(MBEDTLS_ECDSA_C)

#include "ele_crypto.h"
#include "ele_mbedtls.h"

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc calloc
#define mbedtls_free   free
#endif
#include "ele_crypto.h"
#include "ele_mbedtls.h"
#include "mbedtls/platform_util.h"

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


#if defined(MBEDTLS_ECDSA_ALT)

extern ele_ctx_t g_ele_ctx;

/******************** Common APIs ****************************************/

/*
 * Initialize context
 */
void mbedtls_ecdsa_init(mbedtls_ecdsa_context *ctx)
{
    ECDSA_VALIDATE(ctx != NULL);
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

    mbedtls_ecp_group_free( &ctx->grp );
    mbedtls_ecp_point_free( &ctx->Q );

}

int mbedtls_ecdsa_from_keypair( mbedtls_ecdsa_context *ctx, const mbedtls_ecp_keypair *key )
{
    int ret;
    ECDSA_VALIDATE_RET( ctx != NULL );
    ECDSA_VALIDATE_RET( key != NULL );

    if( ( ret = mbedtls_ecp_group_copy( &ctx->grp, &key->grp ) ) != 0 ||
        ( ret = mbedtls_mpi_copy( &ctx->d, &key->d ) ) != 0 ||
        ( ret = mbedtls_ecp_copy( &ctx->Q, &key->Q ) ) != 0 )
    {
        mbedtls_ecdsa_free( ctx );
    }

    /* Set internal Key ID reference */
    ctx->key_id = key->key_id;

    return( ret );
}
/*********************** End of Common *****************************************/

/*********************** Keygen APIs *******************************************/
#if defined(MBEDTLS_ECDSA_GENKEY_ALT)
/*
 * Generate key pair
 */
int mbedtls_ecdsa_genkey(mbedtls_ecdsa_context *ctx,
                         mbedtls_ecp_group_id gid,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng)
{
    int ret = 0;
    status_t result = kStatus_Fail;

    MBEDTLS_INTERNAL_VALIDATE_RET((key != NULL), MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    ret = mbedtls_ecp_group_load(&ctx->grp, gid);
    if (ret != 0)
    {
        return (ret);
    }
    size_t keyLen     = (ctx->grp.pbits) / 8u;
    size_t pubKeyLen  = keyLen * 2;

    uint8_t *pubKey   = mbedtls_calloc(pubKeyLen, sizeof(uint8_t));
    MBEDTLS_INTERNAL_VALIDATE_RET((pubKey != NULL), MBEDTLS_ERR_ECP_ALLOC_FAILED);

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        mbedtls_free(pubKey);
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Check if we have already generated key inside MbedTLS Keystore structure */
    if(g_ele_ctx.keystore_chunks.ECDSA_KeyID != 0u)
    {
        mbedtls_free(pubKey);
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Check if keystore already created */
    if(g_ele_ctx.is_keystore_opened == false)
    {
        /****************** Create Key Store  *********************************/
        ele_keystore_t keystoreParam;
        keystoreParam.id            = g_ele_ctx.key_store_id;
        keystoreParam.nonce         = g_ele_ctx.key_store_nonce;
        keystoreParam.max_updates   = 0xff;
        keystoreParam.min_mac_check = false;
        keystoreParam.min_mac_len   = 0u;
        result = ELE_CreateKeystore(S3MU, g_ele_ctx.session_handle, &keystoreParam, &g_ele_ctx.key_store_handle);
        if (result != kStatus_Success)
        {
            mbedtls_free(pubKey);
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        /****************** Key Management Open *******************************/
        result = ELE_OpenKeyService(S3MU, g_ele_ctx.key_store_handle, &g_ele_ctx.key_management_handle);
        if (result != kStatus_Success)
        {
            mbedtls_free(pubKey);
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        g_ele_ctx.is_keystore_opened = true;
    }

    uint16_t keySize = 0;

    /* Keypair generation */
    ele_gen_key_t NISTkeyGenParam;
    NISTkeyGenParam.key_type      = kKeyType_ECC_KEY_PAIR_SECP_R1_NIST;
    NISTkeyGenParam.key_lifetime  = kKey_Persistent;
    NISTkeyGenParam.key_lifecycle = ELE_KEY_LIFECYCLE;
    NISTkeyGenParam.key_usage     = kKeyUsage_SignHash;
    NISTkeyGenParam.key_size      = ctx->grp.pbits;
    NISTkeyGenParam.permitted_alg = kPermitted_ECDSA_SHA256;
    NISTkeyGenParam.pub_key_addr  = (uint32_t)pubKey;
    NISTkeyGenParam.pub_key_size  = pubKeyLen;
    NISTkeyGenParam.key_group     = (uint16_t)g_ele_ctx.key_group_id;

    if (ELE_GenerateKey(S3MU, g_ele_ctx.key_management_handle, &NISTkeyGenParam, &ctx->key_id, &keySize, false, false) != kStatus_Success)
    {
        mbedtls_free(pubKey);
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Load public key in CTX */
    ret = mbedtls_mpi_read_binary(&ctx->Q.X, pubKey, keyLen);
    ret += mbedtls_mpi_read_binary(&ctx->Q.Y, &pubKey[keyLen], keyLen);
    ret += mbedtls_mpi_lset(&ctx->Q.Z, 1);

    /* Set internal Key ID reference in CTX for later use */
    if (ret == 0)
    {
        ctx->d.s = MBEDTLS_ECDSA_MPI_S_HAVE_OBJECT;
        ctx->d.n = MBEDTLS_ECDSA_MPI_N_HAVE_OBJECT;
        ctx->d.p = (mbedtls_mpi_uint *)(uintptr_t)&ctx->key_id;
    }
    else
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Export chunks into global ELE CTX structure to be exported into buffer later by PK Write and eventualy into NVM by user */
    /* This keystore structure can hold only on ECC key inside */
    g_ele_ctx.keystore_chunks.KeyStoreChunk = g_ele_ctx.keystore_chunks.KeyStoreChunkData; // Using global CTX for data
    g_ele_ctx.keystore_chunks.KeyGroupChunk = g_ele_ctx.keystore_chunks.KeyGroupChunkData; // Using global CTX for data

    if (ELE_ExportChunks(S3MU, g_ele_ctx.key_management_handle, true, g_ele_ctx.key_group_id, ELE_KEYSTORE_MONOTONIC,(ele_chunks_t*)&g_ele_ctx.keystore_chunks) != kStatus_Success)
    {
        mbedtls_free(pubKey);
        if(g_ele_ctx.keystore_chunks.KeyStoreSize != 0u)
            mbedtls_free(g_ele_ctx.keystore_chunks.KeyStoreChunk);
        if(g_ele_ctx.keystore_chunks.KeyGroupSize != 0u)
            mbedtls_free(g_ele_ctx.keystore_chunks.KeyGroupChunk);

        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    else
    {
        g_ele_ctx.keystore_chunks.ECDSA_KeyID = ctx->key_id;
    }


    mbedtls_platform_zeroize(pubKey, pubKeyLen);
    mbedtls_free(pubKey);
    return ret;
}
#endif /* MBEDTLS_ECDSA_GENKEY_ALT */
/*********************** End of Keygen APIs ************************************/

/*********************** Sign APIs *********************************************/
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

    size_t coordinateLen   = (grp->pbits + 7u) / 8u;
    size_t signatureSize   = 2u * coordinateLen;
    uint8_t *signature     = mbedtls_calloc(signatureSize, sizeof(uint8_t));

    if(signature == NULL)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (grp->N.p == NULL)
    {
        mbedtls_free(signature);
        return (MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
    }
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        mbedtls_free(signature);
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    if(g_ele_ctx.signature_gen_handle == 0u)
    {
        if (ELE_OpenSignService(S3MU, g_ele_ctx.key_store_handle, &g_ele_ctx.signature_gen_handle) != kStatus_Success)
        {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
    }

    uint32_t signSize = 0;

    ele_sign_t NISTsignGenParam;
    NISTsignGenParam.key_id     = g_ele_ctx.keystore_chunks.ECDSA_KeyID;
    NISTsignGenParam.msg        = (const uint8_t *)buf;
    NISTsignGenParam.msg_size   = blen;
    NISTsignGenParam.signature  = signature;
    NISTsignGenParam.sig_size   = signatureSize;
    NISTsignGenParam.scheme     = kSig_ECDSA_SHA256;
    NISTsignGenParam.input_flag = false; // Hash message as input

    if (ELE_Sign(S3MU, g_ele_ctx.signature_gen_handle, &NISTsignGenParam, &signSize) != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }
    else
    {
        ret = 0;
    }

    if ((ret = mbedtls_mpi_read_binary(r, signature, coordinateLen)) != 0)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }
    else if ((ret = mbedtls_mpi_read_binary(s, &signature[coordinateLen], coordinateLen)) != 0)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }
    else
    {
        ret = 0;
    }


exit:
    mbedtls_free(signature);
    return (ret);
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
#endif /* defined(MBEDTLS_ECDSA_SIGN_ALT) */
/*********************** End of Sign APIs **************************************/


/*********************** Verify APIs *******************************************/
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
    bool result_nist = false;
    ele_verify_t NISTverifyParam;

    size_t coordinateLen   = (grp->pbits + 7u) / 8u;
    size_t signatureSize   = 2u * coordinateLen;
    size_t pubKeyLen       = 2u * coordinateLen;
    uint8_t *signature     = mbedtls_calloc(signatureSize, sizeof(uint8_t));
    uint8_t *pubKey        = mbedtls_calloc(pubKeyLen, sizeof(uint8_t));

    if(signature == NULL || pubKey == NULL)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* Init HW */
    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* Convert signature into array */
    if (((ret = mbedtls_mpi_write_binary(r, signature, coordinateLen)) != 0) ||
        ((ret = mbedtls_mpi_write_binary(s, &signature[coordinateLen], coordinateLen)) != 0 ))
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* Convert public key into array */
    if (((ret = mbedtls_mpi_write_binary(&Q->X, pubKey, coordinateLen)) != 0) ||
        ((ret = mbedtls_mpi_write_binary(&Q->Y, &pubKey[coordinateLen], coordinateLen)) != 0))
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }

    /* Open verify service */
    if(g_ele_ctx.signature_verif_handle == 0u)
    {
        if (ELE_OpenVerifyService(S3MU, g_ele_ctx.session_handle, &g_ele_ctx.signature_verif_handle) != kStatus_Success)
        {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        }
    }

    NISTverifyParam.pub_key           = (const uint8_t *)pubKey;
    NISTverifyParam.key_size          = pubKeyLen;
    NISTverifyParam.msg               = (const uint8_t *)buf;
    NISTverifyParam.msg_size          = blen;
    NISTverifyParam.signature         = signature;
    NISTverifyParam.sig_size          = signatureSize;
    NISTverifyParam.keypair_type      = kKeyType_ECC_PUB_KEY_SECP_NIST;
    NISTverifyParam.scheme            = kSig_ECDSA_SHA256;
    NISTverifyParam.input_flag        = false; // Message digest as input
    NISTverifyParam.key_security_size = 256u;
    NISTverifyParam.internal          = false;

    if (ELE_Verify(S3MU, g_ele_ctx.signature_verif_handle, &NISTverifyParam, &result_nist) != kStatus_Success)
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    }
    else if(result_nist == true)
    {
        ret = 0;
    }
    else
    {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

exit:
    if(signature != NULL)
        mbedtls_free(signature);
    if(pubKey != NULL)
        mbedtls_free(pubKey);
    return (ret);
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
/*********************** End of Verify APIs ************************************/

#endif /* defined(MBEDTLS_ECDSA_ALT) */

#endif /* MBEDTLS_ECDSA_C */

#endif /* NXP_ELE_ECC_OPAQUE_KEY */
