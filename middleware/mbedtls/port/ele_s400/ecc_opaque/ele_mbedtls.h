/**
 * \file ele_s400/ecc_opaque/ele_mbedtls.h
 *
 * This is an alternate file to the regular ele_s400/ele_mbedtls.h,
 * containing declarations specific to the handling of opaque
 * keys for P-256 curves.
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
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Guarding this file, so it can only be used when it's really meant to be */
#if defined(NXP_ELE_ECC_OPAQUE_KEY)

#ifndef ELE_MBEDTLS_H
#define ELE_MBEDTLS_H

#include "fsl_common.h"

#if defined(MIMXRT1189_cm33_SERIES)
#define S3MU MU_RT_S3MUA
#elif defined(MIMXRT1189_cm7_SERIES)
#define S3MU MU_APPS_S3MUA
#else
#error "No valid SoC defined"
#endif /* MIMXRT1189_cm33_SERIES | MIMXRT1189_cm7_SERIES */

#include "ele_crypto.h"

/*! @brief ELE Chunks container structure. */
typedef struct _mbedtls_ele_chunks_t
{
    uint32_t MasterChunk[MASTER_CHUNK_SIZE / sizeof(uint32_t)]; /*!< Master storage chunk. Always 100 Bytes */
    uint32_t *KeyStoreChunk;  /*!< Key store chunk destination pointer */
    uint32_t *KeyGroupChunk; /*!< Key group chunk destination pointer */
    size_t KeyStoreSize;     /*!< Key store chunk size, set by SW if HEAP for key store destination is used */
    size_t KeyGroupSize;     /*!< Key group chunk size, set by SW if HEAP for key group destination is used */
    uint32_t KeyStoreChunkData[(896u / sizeof(uint32_t))]; /*!< Key store chunk destination (for one ECC P256 keypair it's 896 Bytes) */
    uint32_t KeyGroupChunkData[(152u / sizeof(uint32_t))]; /*!< Key group chunk destination (for one ECC P256 keypair it's 152 Bytes) */
    uint32_t ECDSA_KeyID;         /*!< Randomly generated ELE KEY ID reference */
} mbedtls_ele_chunks_t;

typedef struct
{
    uint32_t session_handle;
    uint32_t key_store_handle;
    uint32_t storage_handle;
    uint32_t data_storage_handle;
    uint32_t key_management_handle;
    uint32_t signature_gen_handle;
    uint32_t signature_verif_handle;
    uint32_t hash_handle;
    bool is_fw_loaded;
    bool is_keystore_opened;
    uint32_t cipher_handle;
    uint32_t mac_handle;
    uint32_t key_store_nonce;
    uint32_t key_store_id;
    uint32_t key_group_id;
    mbedtls_ele_chunks_t keystore_chunks;
} ele_ctx_t;

#define ELE_KEYSTORE_ID         (0x12345678u)
#define ELE_KEYGROUP_ID         (0x00000001u)
#define ELE_KEYSTORE_NONCE      (0xabcdef12u)
#define ELE_KEY_LIFECYCLE       (0x00000001u) /* 1 = OPEN, 2 = Closed, 4 = Closed & Locked */
#define ELE_KEYSTORE_MONOTONIC  false /* False = no antirollback protection,                 */
                                      /* True = anti-rollback protection enabled (Fuse burn!) */

#if defined(MBEDTLS_THREADING_C)
#include "threading_alt.h"
/* MUTEX FOR HW Modules*/
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_ele_mutex;
#endif /* defined(MBEDTLS_THREADING_C) */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Application init for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic init for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_InitHardware(void);

/*!
 * @brief Application deinit for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic deinit for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_DeinitHardware(void);


status_t mbedtls_mcux_rng_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ELE_MBEDTLS_H */

#endif /* NXP_ELE_ECC_OPAQUE_KEY */
