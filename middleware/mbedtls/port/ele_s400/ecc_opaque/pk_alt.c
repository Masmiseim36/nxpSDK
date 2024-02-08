/**
 * \file ele_s400/ecc_opaque/pk_alt.c
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
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* Guarding this file, so it can only be used when it's really meant to be */
#if defined(NXP_ELE_ECC_OPAQUE_KEY)

#include "ele_crypto.h"
#include "ele_mbedtls.h"

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/pk.h"

#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc calloc
#define mbedtls_free   free
#endif

#include "mbedtls/platform_util.h"

#if defined(MBEDTLS_PK_KEY_ALT)

/* Used as values s and n of mbedtls_mpi object to indicate that P contain pointer to key object. */
#define MBEDTLS_MPI_S_HAVE_OBJECT (155)
#define MBEDTLS_MPI_N_HAVE_OBJECT (1u)

typedef struct mbedtls_ecp_context
{
    mbedtls_ecp_group grp; /*!<  Elliptic curve and base point     */
    mbedtls_mpi d;         /*!<  our secret value                  */
    mbedtls_ecp_point Q;   /*!<  our public value                  */
    uint32_t key_id;       /*!<  Key ID reference used by ELE      */
} mbedtls_ecp_context;

extern ele_ctx_t g_ele_ctx;

/* Since we are storing global CTX variable holding encrypted blob, we are not interested in PK ctx with private key */
/* All needed chunks and ECC key ID is stored in buf and can be loaded into ELE by PK parse API */
int mbedtls_pk_write_key_der( mbedtls_pk_context *key, unsigned char *buf, size_t size )
{
    MBEDTLS_INTERNAL_VALIDATE_RET((buf != NULL), MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    if( size == 0 )
       return MBEDTLS_ERR_ASN1_BUF_TOO_SMALL;

    size_t keyBlobLen;

    /* Check if we already generated ECC keypair in ELE */
    if(g_ele_ctx.is_keystore_opened != true || g_ele_ctx.keystore_chunks.ECDSA_KeyID == 0u)
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;

    /* private key is size of one coordinate and public key 2 + blob overhead of 24*/
    keyBlobLen = sizeof(mbedtls_ele_chunks_t);
    (void)key;

    if (keyBlobLen > size)
       return MBEDTLS_ERR_ASN1_BUF_TOO_SMALL;

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /* Copy ELE blob structure into buf */
    memcpy(buf, (void*)&g_ele_ctx.keystore_chunks, keyBlobLen);

    return keyBlobLen;
}

int mbedtls_pk_parse_key( mbedtls_pk_context *pk,
                  const unsigned char *key, size_t keylen,
                  const unsigned char *pwd, size_t pwdlen )
{
    int ret = 0;
    size_t key_size = 0;

   MBEDTLS_INTERNAL_VALIDATE_RET((pk != NULL), MBEDTLS_ERR_ECP_BAD_INPUT_DATA);
   MBEDTLS_INTERNAL_VALIDATE_RET((key != NULL), MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    if( keylen == 0 )
        return MBEDTLS_ERR_PK_KEY_INVALID_FORMAT;

    /* Setup and allocate PK structure, using MBEDTLS_PK_ECKEY because of X509 module. */
    /* Both CTXs are compatible */
    //mbedtls_pk_setup(pk, mbedtls_pk_info_from_type(MBEDTLS_PK_ECDSA));
    mbedtls_pk_setup(pk, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));

    //mbedtls_ecdsa_context *keyCtx = (mbedtls_ecdsa_context *) mbedtls_pk_ec( *pk );
    mbedtls_ecp_context *keyCtx = (mbedtls_ecp_context *)pk->pk_ctx;
    mbedtls_ecp_group_load(&keyCtx->grp, MBEDTLS_ECP_DP_SECP256R1);

    size_t keyLen = (keyCtx->grp.pbits) / 8u;

    mbedtls_ele_chunks_t *chunks_ptr = (mbedtls_ele_chunks_t*)key;

    uint8_t pubKey[64] = {0u};

    if (CRYPTO_InitHardware() != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    /****************** Import Master storage chunk ***********************/
    if (ELE_StorageMasterImport(S3MU, g_ele_ctx.storage_handle, chunks_ptr->MasterChunk) != kStatus_Success)
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;

    /****************** Open Key Store ************************************/

    ele_keystore_t keystoreParam;
    keystoreParam.id            = g_ele_ctx.key_store_id;
    keystoreParam.nonce         = g_ele_ctx.key_store_nonce;
    keystoreParam.max_updates   = 0xff;
    keystoreParam.min_mac_check = false;
    keystoreParam.min_mac_len   = 0u;

    if (ELE_OpenKeystore(S3MU, g_ele_ctx.session_handle, &keystoreParam, &g_ele_ctx.key_store_handle, chunks_ptr->KeyStoreChunkData, chunks_ptr->KeyStoreSize) != kStatus_Success)
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;

    /****************** Key Management Open *******************************/
    if (ELE_OpenKeyService(S3MU, g_ele_ctx.key_store_handle, &g_ele_ctx.key_management_handle) != kStatus_Success)
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;

    /****************** Key Management Chunk Import ***********************/
    if (ELE_ManageKeyGroup(S3MU, g_ele_ctx.key_management_handle, g_ele_ctx.key_group_id, kELE_keyMngImport, chunks_ptr->KeyGroupChunkData, chunks_ptr->KeyGroupSize) !=  kStatus_Success)
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;

    /****************** Reconstruct Public Key ****************************/
    if (ELE_GeneratePubKey(S3MU, g_ele_ctx.key_store_handle, chunks_ptr->ECDSA_KeyID, (uint32_t*)&pubKey, sizeof(pubKey), (uint16_t*)&key_size) !=  kStatus_Success)
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;

    /***************** Copy ELE Chunks into internal global CTX ***********/

    memcpy((void*)&g_ele_ctx.keystore_chunks, key, keylen);
    g_ele_ctx.keystore_chunks.ECDSA_KeyID = chunks_ptr->ECDSA_KeyID;

    if (ret == 0)
    {
        ret = mbedtls_mpi_read_binary(&keyCtx->Q.X, pubKey, keyLen);
        ret += mbedtls_mpi_read_binary(&keyCtx->Q.Y, &pubKey[keyLen], keyLen);
        ret += mbedtls_mpi_lset(&keyCtx->Q.Z, 1);
    }

    if (ret == 0)
    {
        keyCtx->key_id = chunks_ptr->ECDSA_KeyID;
        keyCtx->d.s = MBEDTLS_MPI_S_HAVE_OBJECT;
        keyCtx->d.n = MBEDTLS_MPI_N_HAVE_OBJECT;
        keyCtx->d.p = (mbedtls_mpi_uint *)(uintptr_t)&keyCtx->key_id;

    }


    return ret;
}
#endif /* MBEDTLS_PK_KEY_ALT */

#endif /* NXP_ELE_ECC_OPAQUE_KEY */
