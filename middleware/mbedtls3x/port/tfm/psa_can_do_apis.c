/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include MBEDTLS_CONFIG_FILE
#include "psa/crypto.h"

int psa_can_do_hash(psa_algorithm_t hash_alg)
{
    (void) hash_alg;
    /* Workaround for the legacy CryptoCell driver requiring hash during init */
    return 1; //psa_get_drivers_initialized();
}
