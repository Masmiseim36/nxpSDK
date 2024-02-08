/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H


#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_CCM_ALT)

/**
 * \brief    The CCM context-type definition. The CCM context is passed
 *           to the APIs called.
 */
typedef struct mbedtls_ccm_context {
    uint8_t key[32];
    size_t keySize;
}
mbedtls_ccm_context;

#endif /* MBEDTLS_CCM_ALT */

#ifdef __cplusplus
}
#endif

#endif /* ccm_alt.h */
