/**
 * \file ecdh.h
 *
 * \brief This file contains ECDH definitions and functions.
 *
 * The Elliptic Curve Diffie-Hellman (ECDH) protocol is an anonymous
 * key agreement protocol allowing two parties to establish a shared
 * secret over an insecure channel. Each party must have an
 * elliptic-curve public–private key pair.
 *
 * For more information, see <em>NIST SP 800-56A Rev. 2: Recommendation for
 * Pair-Wise Key Establishment Schemes Using Discrete Logarithm
 * Cryptography</em>.
 */
/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
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
 *  This file is part of Mbed TLS (https://tls.mbed.org)
 */

/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_ECDH_ALT_H
#define MBEDTLS_ECDH_ALT_H

#include "mbedtls/ecp.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_MCUX_SSS_ECDH)

#if !defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
/**
 * Defines the ECDH implementation used.
 *
 * Later versions of the library may add new variants, therefore users should
 * not make any assumptions about them.
 */
typedef enum
{
    MBEDTLS_ECDH_VARIANT_NONE = 0,   /*!< Implementation not defined. */
    MBEDTLS_ECDH_VARIANT_MBEDTLS_2_0,/*!< The default Mbed TLS implementation */
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
    MBEDTLS_ECDH_VARIANT_EVEREST     /*!< Everest implementation */
#endif
} mbedtls_ecdh_variant;

/**
 * The context used by the default ECDH implementation.
 *
 * Later versions might change the structure of this context, therefore users
 * should not make any assumptions about the structure of
 * mbedtls_ecdh_context_mbed.
 */
typedef struct mbedtls_ecdh_context_mbed
{
    mbedtls_ecp_group MBEDTLS_PRIVATE(grp);   /*!< The elliptic curve used. */
    mbedtls_mpi MBEDTLS_PRIVATE(d);           /*!< The private key. */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Q);     /*!< The public key. */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Qp);    /*!< The value of the public key of the peer. */
    mbedtls_mpi MBEDTLS_PRIVATE(z);           /*!< The shared secret. */
#if defined(MBEDTLS_ECP_RESTARTABLE)
    mbedtls_ecp_restart_ctx MBEDTLS_PRIVATE(rs); /*!< The restart context for EC computations. */
#endif
} mbedtls_ecdh_context_mbed;
#endif
/**
 *
 * \warning         Performing multiple operations concurrently on the same
 *                  ECDSA context is not supported; objects of this type
 *                  should not be shared between multiple threads.
 * \brief           The ECDH context structure.
 */
typedef struct mbedtls_ecdh_context
{
#if defined(MBEDTLS_ECDH_LEGACY_CONTEXT)
    mbedtls_ecp_group MBEDTLS_PRIVATE(grp);   /*!< The elliptic curve used. */
    mbedtls_mpi MBEDTLS_PRIVATE(d);           /*!< The private key. */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Q);     /*!< The public key. */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Qp);    /*!< The value of the public key of the peer. */
    mbedtls_mpi MBEDTLS_PRIVATE(z);           /*!< The shared secret. */
    int MBEDTLS_PRIVATE(point_format);        /*!< The format of point export in TLS messages. */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Vi);    /*!< The blinding value. */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Vf);    /*!< The unblinding value. */
    mbedtls_mpi MBEDTLS_PRIVATE(_d);          /*!< The previous \p d. */
#if defined(MBEDTLS_ECP_RESTARTABLE)
    int MBEDTLS_PRIVATE(restart_enabled);        /*!< The flag for restartable mode. */
    mbedtls_ecp_restart_ctx MBEDTLS_PRIVATE(rs); /*!< The restart context for EC computations. */
#endif /* MBEDTLS_ECP_RESTARTABLE */
#else
    uint8_t MBEDTLS_PRIVATE(point_format);       /*!< The format of point export in TLS messages
                                  as defined in RFC 4492. */
    mbedtls_ecp_group_id MBEDTLS_PRIVATE(grp_id);/*!< The elliptic curve used. */
    mbedtls_ecdh_variant MBEDTLS_PRIVATE(var);   /*!< The ECDH implementation/structure used. */
    union
    {
        mbedtls_ecdh_context_mbed   MBEDTLS_PRIVATE(mbed_ecdh);
#if defined(MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED)
        mbedtls_ecdh_context_everest MBEDTLS_PRIVATE(everest_ecdh);
#endif
    } MBEDTLS_PRIVATE(ctx);                      /*!< Implementation-specific context. The
                                  context in use is specified by the \c var
                                  field. */
#if defined(MBEDTLS_ECP_RESTARTABLE)
    uint8_t MBEDTLS_PRIVATE(restart_enabled);    /*!< The flag for restartable mode. Functions of
                                  an alternative implementation not supporting
                                  restartable mode must return
                                  MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED error
                                  if this flag is set. */
#endif /* MBEDTLS_ECP_RESTARTABLE */
#endif /* MBEDTLS_ECDH_LEGACY_CONTEXT */
    sss_sscp_object_t key;
    sss_sscp_object_t peerPublicKey;
    sss_sscp_object_t sharedSecret;
    bool isKeyInitialized;
} mbedtls_ecdh_context;

#if defined(MBEDTLS_SELF_TEST)
int mbedtls_ecdh_make_public_sw(mbedtls_ecdh_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng);

int mbedtls_ecdh_calc_secret_sw(mbedtls_ecdh_context *ctx,
                                size_t *olen,
                                unsigned char *buf,
                                size_t blen,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng);

int mbedtls_ecdh_self_test(int verbose);
#endif /* MBEDTLS_SELF_TEST */
#endif /* MBEDTLS_MCUX_SSS_ECDH */

#ifdef __cplusplus
}
#endif

#endif /* ecdh_alt.h */
