/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include MBEDTLS_CONFIG_FILE
#include "psa/crypto.h"

/* Entropy poll callback for a hardware source */
#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    /* No need for thread safety here, this is done by TFM PSA NS API */
    if (psa_generate_random(output, len) != PSA_SUCCESS)
    {
        return kStatus_Fail;
    }

    *olen = len;
    return kStatus_Success;
}
#endif /* MBEDTLS_ENTROPY_HARDWARE_ALT */
