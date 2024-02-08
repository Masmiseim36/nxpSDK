/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_GCM_ALT_H
#define MBEDTLS_GCM_ALT_H


#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_GCM_ALT)

/**
 * \brief          The GCM context structure.
 */
typedef struct mbedtls_gcm_context {
    uint8_t key[32];
    size_t keySize;
}
mbedtls_gcm_context;



#endif /* MBEDTLS_GCM_ALT */

#ifdef __cplusplus
}
#endif

#endif /* gcm_alt.h */
