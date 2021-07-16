/* Copyright 2020 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __PKCS11_MBEDTLS_UTILS_H__
#define __PKCS11_MBEDTLS_UTILS_H__

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/base64.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/oid.h"
#include "mbedtls/pk.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"

int pk_use_ecparams(const mbedtls_asn1_buf *params, mbedtls_ecp_group *grp);
int pk_get_ecparams(unsigned char **p, const unsigned char *end, mbedtls_asn1_buf *params);

#endif //__PKCS11_MBEDTLS_UTILS_H__