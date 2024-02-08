/**
 * \file ele_s400/ecc_opaque/ecdsa_alt.h
 *
 * \brief This file contains alternative ECDSA definitions and functions.
 *
 * The Elliptic Curve Digital Signature Algorithm (ECDSA) is defined in
 * <em>Standards for Efficient Cryptography Group (SECG):
 * SEC1 Elliptic Curve Cryptography</em>.
 * The use of ECDSA for TLS is defined in <em>RFC-4492: Elliptic Curve
 * Cryptography (ECC) Cipher Suites for Transport Layer Security (TLS)</em>.
 *
 * \warning This implementation is not compatible with some of the other
 * MbedTLS features (e.g. PK module key material loading/cert parsing for RSA).
 * It is only intended to showcase P-256 opaque key handling via the EdgeLock
 * Enclave (ELE).
 *
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
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_ECDSA_ALT_H
#define MBEDTLS_ECDSA_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* Guarding this file, so it can only be used when it's really meant to be */
#if defined(NXP_ELE_ECC_OPAQUE_KEY)

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"
#include "ele_crypto.h"

#if defined(MBEDTLS_ECDSA_ALT)
/**
 * \brief           The ECDSA context structure.
 *
 * \warning         Performing multiple operations concurrently on the same
 *                  ECDSA context is not supported; objects of this type
 *                  should not be shared between multiple threads.
 */
typedef struct mbedtls_ecdsa_context
{
    mbedtls_ecp_group grp; /*!<  Elliptic curve and base point              */
    mbedtls_mpi d;         /*!<  our secret value (not used here)           */
    mbedtls_ecp_point Q;   /*!<  our public value                           */
    uint32_t key_id;       /*!<  Key ID reference used by ELE               */
} mbedtls_ecdsa_context;
#endif /* MBEDTLS_ECDSA_ALT */

#endif /* NXP_ELE_ECC_OPAQUE_KEY */

#endif /* MBEDTLS_ECDSA_ALT_H */
