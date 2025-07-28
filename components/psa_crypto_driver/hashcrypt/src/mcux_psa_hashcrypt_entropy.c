/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** \file mcux_psa_hashcrypt_entropy.c
 *
 * This file contains the implementation of the entry points associated
 * to the entropy capability as described by the PSA Cryptoprocessor
 * Driver interface specification
 *
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mcux_psa_hashcrypt_entropy.h"
#include "fsl_adapter_rng.h"

static mcux_mutex_t *s_mutex = NULL;

psa_status_t hal_rng_to_psa_status(hal_rng_status_t status)
{
    psa_status_t res;

    switch (status) {
        case kStatus_HAL_RngSuccess:
            res = PSA_SUCCESS;
            break;
        case kStatus_HAL_RngInvalidArgumen:
            res = PSA_ERROR_INVALID_ARGUMENT;
            break;
        default:
            res = PSA_ERROR_HARDWARE_FAILURE;
            break;
    }

    return res;
}

psa_status_t mcux_psa_hashcrypt_entropy_init(mcux_mutex_t *mutex)
{

    hal_rng_status_t status = HAL_RngInit();

    if ((status == kStatus_HAL_RngSuccess) || (status == KStatus_HAL_RngNotSupport))
    {
        s_mutex = mutex;

        return PSA_SUCCESS;
    }

    return hal_rng_to_psa_status(status);
}

void mcux_psa_hashcrypt_entropy_deinit(void)
{
    HAL_RngDeinit();

    s_mutex = NULL;
}

/** \defgroup psa_entropy PSA driver entry points for entropy collection
 *
 *  Entry points for entropy collection from the TRNG source as described by the
 *  PSA Cryptoprocessor Driver interface specification. The TRNG
 *  operates by sampling the output of a fast free-running ring oscillator in a
 *  different (slower) clock domain
 *
 *  @{
 */
psa_status_t mcux_psa_hashcrypt_entropy_get(uint32_t flags,
                                         size_t *estimate_bits,
                                         uint8_t *output,
                                         size_t output_size)
{
    status_t result   = kStatus_Success;
    psa_status_t err  = PSA_ERROR_CORRUPTION_DETECTED;

    if (output == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (estimate_bits == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    *estimate_bits = 0u;

    if (output_size == 0u) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (mcux_mutex_lock(s_mutex) != 0) {
        return PSA_ERROR_BAD_STATE;
    }

    result = HAL_RngHwGetData((uint8_t *) output, output_size);
    if (result == KStatus_HAL_RngNotSupport)
    {
        result = HAL_RngGetData((uint8_t *) output, output_size);
    }
    err = hal_rng_to_psa_status(result);

    if (mcux_mutex_unlock(s_mutex) != 0) {
        return PSA_ERROR_BAD_STATE;
    }

    if (err == PSA_SUCCESS) {
        *estimate_bits = output_size * 8u;
    }

    return err;
}
/** @} */ // end of psa_entropy

/*
 * FixMe: This function is required to integrate into Mbed TLS as the PSA
 * subsystem does not yet support entropy entry points. See the header
 * entropy_poll.h for details. This needs to be revised once Mbed TLS adds
 * support for entropy.
 */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    size_t estimate_bits  = 0u;
    psa_status_t status = mcux_psa_hashcrypt_entropy_get(0u, &estimate_bits, output, len);

    *olen = estimate_bits / 8u;

    return status;
}
