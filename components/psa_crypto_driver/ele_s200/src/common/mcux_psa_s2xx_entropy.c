/*
 * Copyright 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** \file mcux_psa_s2xx_entropy.c
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

#include "fsl_sss_mgmt.h"
#include "fsl_sss_sscp.h"
#include "fsl_sscp_mu.h"

#include "mcux_psa_s2xx_entropy.h"

/** \defgroup psa_entropy PSA driver entry points for entropy collection
 *
 *  Entry points for entropy collection from the TRNG source as described by the
 *  PSA Cryptoprocessor Driver interface specification. The TRNG
 *  operates by sampling the output of a fast free-running ring oscillator in a
 *  different (slower) clock domain
 *
 *  @{
 */
psa_status_t ele_get_entropy(uint32_t flags, size_t *estimate_bits, uint8_t *output, size_t output_size)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    sss_sscp_rng_t ctx  = {0u};

    if (output == NULL)
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    if (estimate_bits == NULL)
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    if (output_size == 0u)
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    /* No failure up to this point. Reset status and handle the rest with
     * calls to the RNG HW.
     */
    status = PSA_SUCCESS;

    if (mcux_mutex_lock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_SERVICE_FAILURE;
    }

    if (kStatus_SSS_Success != sss_sscp_rng_context_init(&g_ele_ctx.sssSession, &ctx, 0u))
    {
        status = PSA_ERROR_HARDWARE_FAILURE;
    }
    if (kStatus_SSS_Success != sss_sscp_rng_get_random(&ctx, output, output_size))
    {
        status = PSA_ERROR_HARDWARE_FAILURE;
    }
    if (kStatus_SSS_Success != sss_sscp_rng_free(&ctx))
    {
        status = PSA_ERROR_HARDWARE_FAILURE;
    }

    if (mcux_mutex_unlock(&ele_hwcrypto_mutex) != 0)
    {
        return PSA_ERROR_SERVICE_FAILURE;
    }

    if (status == PSA_SUCCESS)
    {
        *estimate_bits = output_size * 8u;
    }

exit:
    return status;
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
    size_t estimate_bits = 0u;
    int status           = ele_get_entropy(0u, &estimate_bits, output, len);

    *olen = estimate_bits / 8u;

    return status;
}
