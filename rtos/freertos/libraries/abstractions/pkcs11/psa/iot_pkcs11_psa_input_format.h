/*
 *  Copyright (C) 2006-2020, Arm Limited, All Rights Reserved
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
 */

/*
 * This file is derivative of amazon-freertos\libraries\3rdparty\mbedtls\library\
 * pkparse.c(amazon-freertos commit 74875b1d2)
 */

#ifndef __IOT_PKCS11_PSA_INPUT_FORMAT_H__
#define __IOT_PKCS11_PSA_INPUT_FORMAT_H__

/* mbedTLS includes. */
#include "mbedtls/pk.h"
#include "mbedtls/asn1.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/oid.h"

#define pkcs11DER_ENCODED_OID_P256_LEGNTH    19

/* Get a PK algorithm identifier
 *
 *  AlgorithmIdentifier  ::=  SEQUENCE  {
 *       algorithm               OBJECT IDENTIFIER,
 *       parameters              ANY DEFINED BY algorithm OPTIONAL  }
 */
int pk_get_pk_alg( unsigned char **p,
                          const unsigned char *end,
                          mbedtls_pk_type_t *pk_alg, mbedtls_asn1_buf *params );

/*
 * Get the uncompressed representation defined by SEC1 §2.3.3
 * as the content of an ECPoint.
 */
int get_public_key_ECPoint( const unsigned char *key,
                            size_t keylen,
                            unsigned char **startaddr,
                            size_t *length );

#endif /* ifndef __IOT_PKCS11_PSA_INPUT_FORMAT_H__ */
