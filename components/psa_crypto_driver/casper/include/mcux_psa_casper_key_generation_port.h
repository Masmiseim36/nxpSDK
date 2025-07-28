/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#ifndef MCUX_PSA_CASPER_KEY_GENERATION_PORT_H
#define MCUX_PSA_CASPER_KEY_GENERATION_PORT_H

/** \file mcux_psa_casper_init.h
 *
 * This file contains the declaration of the entry points associated to
 * driver initialisation and de-initialisation procedures.
 *
 */

#include "mbedtls/ecp.h"
#include "mbedtls/error.h"
#include "mbedtls/private_access.h"

#include <psa/crypto.h>
#include "mbedtls/psa_util.h"
#include "psa_crypto_random_impl.h"

#include "mcux_psa_casper_ecp_port.h"

#ifdef __cplusplus
extern "C" {
#endif

psa_status_t casper_mbedtls_psa_ecp_generate_key(const psa_key_attributes_t *attributes,
                                                 uint8_t *key_buffer,
                                                 size_t key_buffer_size,
                                                 size_t *key_buffer_length);

#ifdef __cplusplus
}
#endif

#endif /* MCUX_PSA_CASPER_INIT_H */
