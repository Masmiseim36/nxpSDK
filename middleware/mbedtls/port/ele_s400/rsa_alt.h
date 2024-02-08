/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_RSA_ALT_H
#define MBEDTLS_RSA_ALT_H


#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_PKCS1_V21_ALT)

int rsa_rsassa_pss_sign(mbedtls_rsa_context *ctx,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng,
                        int mode,
                        mbedtls_md_type_t md_alg,
                        unsigned int hashlen,
                        const unsigned char *hash,
                        int saltlen,
                        unsigned char *sig);

#endif /* MBEDTLS_RSA_ALT */

#ifdef __cplusplus
}
#endif

#endif /* rsa_alt.h */
