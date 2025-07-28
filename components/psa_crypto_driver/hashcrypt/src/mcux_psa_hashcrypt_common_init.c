/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcux_psa_hashcrypt_init.h"
#include "mcux_psa_hashcrypt_common_init.h"
#include "mcux_psa_hashcrypt_entropy.h"

mcux_mutex_t rng_hwcrypto_mutex;

psa_status_t hashcrypt_common_init(void)
{
    psa_status_t status;
    status_t hashcrypt_status = CRYPTO_InitHardware_Hashcrypt();
    status = hashcrypt_to_psa_status(hashcrypt_status);

    if (status != PSA_SUCCESS) {
        return status;
    }

    /* Init RNG peripheral */
    status = mcux_psa_hashcrypt_entropy_init(&rng_hwcrypto_mutex);

    return status;
}

psa_status_t hashcrypt_common_free(void)
{
    status_t hashcrypt_status = CRYPTO_DeinitHardware_Hashcrypt();

    /* Deinit RNG peripheral */
    mcux_psa_hashcrypt_entropy_deinit();

    return hashcrypt_to_psa_status(hashcrypt_status);
}
