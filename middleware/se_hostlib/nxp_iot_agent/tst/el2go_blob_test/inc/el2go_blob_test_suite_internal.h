/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef __EL2GO_BLOB_TEST_SUITE_INTERNAL_H__
#define __EL2GO_BLOB_TEST_SUITE_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

// These are placeholder blobs
// Refer to el2go_blob_test.readme for how to generate device specific ones

static const uint8_t INTERNAL_AES128_CIPHER_CTR[] = {0};

static const uint8_t INTERNAL_AES128_CIPHER_CBC[] = {0};

static const uint8_t INTERNAL_AES128_CIPHER_ECB[] = {0};

static const uint8_t INTERNAL_AES128_AEAD_GCM[] = {0};

static const uint8_t INTERNAL_AES128_MAC_CMAC[] = {0};

static const uint8_t INTERNAL_AES256_CIPHER_CTR[] = {0};

static const uint8_t INTERNAL_AES256_CIPHER_CBC[] = {0};

static const uint8_t INTERNAL_AES256_CIPHER_ECB[] = {0};

static const uint8_t INTERNAL_AES256_AEAD_GCM[] = {0};

static const uint8_t INTERNAL_AES256_MAC_CMAC[] = {0};

static const uint8_t INTERNAL_NISTP256_SIGVERMSG_ECDSASHA256[] = {0};

static const uint8_t INTERNAL_NISTP256_SIGVERHASH_ECDSASHA256[] = {0};

static const uint8_t INTERNAL_NISTP256_SIGVERHASH_ECDSAANY[] = {0};

static const uint8_t INTERNAL_HMAC256_MAC_HMACSHA256[] = {0};

static const uint8_t INTERNAL_HMAC256_KDF_HKDFSHA256[] = {0};

#ifdef __cplusplus
}
#endif

#endif /* __EL2GO_BLOB_TEST_SUITE_INTERNAL_H__ */
